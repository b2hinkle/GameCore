// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_HitResult.h"

bool GCUtils::HitResult::AreHitsFromSameTrace(const FHitResult& HitA, const FHitResult& HitB)
{
    const bool bSameTraceStart = (HitA.TraceStart == HitB.TraceStart);
    const bool bSameTraceEnd = (HitA.TraceEnd == HitB.TraceEnd);


    const bool bSameTrace = (bSameTraceStart && bSameTraceEnd);
    return bSameTrace;
}

float GCUtils::HitResult::CheapCalculateTraceLength(const FHitResult& InHit)
{
    if (InHit.TraceStart == InHit.TraceEnd)
    {
        return 0.f;
    }

    if (InHit.Time == 0.f)
    {
        UE_LOG(LogGCHitResultHelpers, Verbose, TEXT("%s() Cannot cheaply calculate trace length from a hit result with time of 0. Fall back on normal (more expensive) method for calculating"), ANSI_TO_TCHAR(__FUNCTION__));
        return FVector::Distance(InHit.TraceStart, InHit.TraceEnd);
    }

    // Return the hit's distance scaled up by the trace time
    return InHit.Distance * (1 / InHit.Time);
}

void GCUtils::HitResult::AdjustTraceDataBySlidingTraceStartAndEndByTime(FHitResult& InOutHit, const float InTimeAtNewTraceStart, const float InTimeAtNewTraceEnd)
{
    const float OriginalTraceLength = CheapCalculateTraceLength(InOutHit);

    // Set new trace points
    {
        const float TimeAtOldTraceStart = 0;
        const float TraceStartAdjustmentDistance = (InTimeAtNewTraceStart - TimeAtOldTraceStart) * OriginalTraceLength;
        const FVector ForwardsTraceDirection = (InOutHit.TraceEnd - InOutHit.TraceStart).GetSafeNormal();
        InOutHit.TraceStart = InOutHit.TraceStart + (ForwardsTraceDirection * TraceStartAdjustmentDistance);

        const float TimeAtOldTraceEnd = 1;
        const float TraceEndAdjustmentDistance = (TimeAtOldTraceEnd - InTimeAtNewTraceEnd) * OriginalTraceLength;
        const FVector BackwardsTraceDirection = -ForwardsTraceDirection;
        InOutHit.TraceEnd = InOutHit.TraceEnd + (BackwardsTraceDirection * TraceEndAdjustmentDistance);
    }

    // Subtract both multipliers to get the the multiplier that brings old trace length to the new trace length
    const float OldTraceLengthToNewTraceLengthMultiplier = (InTimeAtNewTraceEnd - InTimeAtNewTraceStart);
    const float NewTraceLength = FMath::Abs(OriginalTraceLength * OldTraceLengthToNewTraceLengthMultiplier );

    // Update trace-related data
    {
        InOutHit.Time -= InTimeAtNewTraceStart; // move our hit time with the new start time
        InOutHit.Time /= OldTraceLengthToNewTraceLengthMultiplier ; // then adjust it with the length multiplier

        InOutHit.Distance = NewTraceLength * InOutHit.Time;
    }


    if (InOutHit.Time < 0.f || InOutHit.Time > 1.f)
    {
        UE_LOG(LogGCHitResultHelpers, Error, TEXT("%s(): The new TraceStart and TraceEnd has put the hit out of range of the trace"), ANSI_TO_TCHAR(__FUNCTION__));
        check(0);
    }
}
