// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_CollisionQuery_Strength_Debug.h"

#include "GCUtils_CollisionQuery_Strength.h"
#include "GCUtils_DebugDrawing.h"
#include "DrawDebugHelpers.h"

namespace GCUtils::CollisionQuery::Strength::Debug
{
    static FLinearColor GetDebugColorForStrength(const float InStrength, const float InInitialStrength, const FLinearColor& InFullStrengthColor = FLinearColor::Green, const FLinearColor& InNoStrengthColor = FLinearColor::Red);
}

void GCUtils::CollisionQuery::Strength::Debug::DrawStrengthDebugLine(const UWorld* InWorld, const FPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const bool bInPersistentLines, const float InLifeTime, const uint8 InDepthPriority, const float InThickness, const float InSegmentsLength, const float InSegmentsSpacingLength, const FLinearColor& InFullStrengthColor, const FLinearColor& InNoStrengthColor)
{
#if ENABLE_DRAW_DEBUG
    const float SceneCastTravelDistance = InResult.StrengthSceneCastInfo.DistanceToStop;

    const float NumberOfLineSegments = FMath::CeilToInt(SceneCastTravelDistance / (InSegmentsLength + InSegmentsSpacingLength));
    for (int32 i = 0; i < NumberOfLineSegments; ++i)
    {
        float DistanceToLineSegmentStart = (InSegmentsLength + InSegmentsSpacingLength) * i;
        float DistanceToLineSegmentEnd = DistanceToLineSegmentStart + InSegmentsLength;
        if (DistanceToLineSegmentEnd > SceneCastTravelDistance)
        {
            DistanceToLineSegmentEnd = SceneCastTravelDistance;
        }

        const FVector LineSegmentStart = InResult.StrengthSceneCastInfo.StartLocation + (InResult.StrengthSceneCastInfo.CastDirection * DistanceToLineSegmentStart);
        const FVector LineSegmentEnd = InResult.StrengthSceneCastInfo.StartLocation + (InResult.StrengthSceneCastInfo.CastDirection * DistanceToLineSegmentEnd);


        // Get the strength at the line segment start
        float StrengthAtLineSegmentStart;
        {
            // NOTE: we use the term "Time" as in the ratio from FPenetrationSceneCastWithExitHitsUsingStrengthResult::StartLocation to FPenetrationSceneCastWithExitHitsUsingStrengthResult::StopLocation. This is not the same as FHitResult::Time!
            float TimeOnOrBeforeLineSegmentStart = 0.f;
            float StrengthOnOrBeforeLineSegmentStart = InResult.StrengthSceneCastInfo.StartStrength;
            float TimeOnOrAfterLineSegmentStart = 1.f;
            float StrengthOnOrAfterLineSegmentStart = InResult.StrengthSceneCastInfo.StopStrength;
            for (const FStrengthHitResult& Hit : InResult.HitResults)
            {
                if (Hit.Distance <= DistanceToLineSegmentStart)
                {
                    // This hit is directly on or before the line segment start
                    TimeOnOrBeforeLineSegmentStart = Hit.Time / InResult.StrengthSceneCastInfo.TimeAtStop;
                    StrengthOnOrBeforeLineSegmentStart = Hit.Strength;
                    continue;
                }
                if (Hit.Distance >= DistanceToLineSegmentStart)
                {
                    // This hit is directly on or after the line segment start
                    TimeOnOrAfterLineSegmentStart = Hit.Time / InResult.StrengthSceneCastInfo.TimeAtStop;
                    StrengthOnOrAfterLineSegmentStart = Hit.Strength;
                    break;
                }
            }

            const float TimeOfLineSegmentStart = DistanceToLineSegmentStart / SceneCastTravelDistance;
            StrengthAtLineSegmentStart = FMath::Lerp(StrengthOnOrBeforeLineSegmentStart, StrengthOnOrAfterLineSegmentStart, TimeOfLineSegmentStart / TimeOnOrAfterLineSegmentStart);
        }

        // Find the hits in between the line segment start and end
        TArray<FStrengthHitResult> HitsWithinLineSegment;
        for (const FStrengthHitResult& Hit : InResult.HitResults)
        {
            if (Hit.Distance <= DistanceToLineSegmentStart)
            {
                continue;
            }
            if (Hit.Distance >= DistanceToLineSegmentEnd)
            {
                break;
            }

            HitsWithinLineSegment.Add(Hit); // hit in between segment
        }


        // Draw the debug line(s)
        if (HitsWithinLineSegment.Num() <= 0)
        {
            const FColor StrengthDebugColor = GetDebugColorForStrength(StrengthAtLineSegmentStart, InInitialStrength, InFullStrengthColor, InNoStrengthColor).ToFColor(true);
            DrawDebugLine(InWorld, LineSegmentStart, LineSegmentEnd, StrengthDebugColor, false, InLifeTime, 0, InThickness);
        }
        else // there are hits (penetrations) within this segment so we will draw multible lines for this segment to give more accurate colors
        {
            // Debug line from the line segment start to the first hit
            {
                const FVector DebugLineStart = LineSegmentStart;
                const FVector DebugLineEnd = HitsWithinLineSegment[0].Location;
                const FColor StrengthDebugColor = GetDebugColorForStrength(StrengthAtLineSegmentStart, InInitialStrength, InFullStrengthColor, InNoStrengthColor).ToFColor(true);
                DrawDebugLine(InWorld, DebugLineStart, DebugLineEnd, StrengthDebugColor, false, InLifeTime, 0, InThickness);
            }

            // Debug lines from hit to hit
            for (int32 j = 0; j < HitsWithinLineSegment.Num(); ++j)
            {
                if (HitsWithinLineSegment.IsValidIndex(j + 1) == false)
                {
                    break;
                }

                const FVector DebugLineStart = HitsWithinLineSegment[j].Location;
                const FVector DebugLineEnd = HitsWithinLineSegment[j + 1].Location;
                const FColor StrengthDebugColor = GetDebugColorForStrength(HitsWithinLineSegment[j].Strength, InInitialStrength, InFullStrengthColor, InNoStrengthColor).ToFColor(true);
                DrawDebugLine(InWorld, DebugLineStart, DebugLineEnd, StrengthDebugColor, false, InLifeTime, 0, InThickness);
            }

            // Debug line from the last hit to the line segment end
            {
                const FVector DebugLineStart = HitsWithinLineSegment.Last().Location;
                const FVector DebugLineEnd = LineSegmentEnd;
                const FColor StrengthDebugColor = GetDebugColorForStrength(HitsWithinLineSegment.Last().Strength, InInitialStrength, InFullStrengthColor, InNoStrengthColor).ToFColor(true);
                DrawDebugLine(InWorld, HitsWithinLineSegment.Last().Location, LineSegmentEnd, StrengthDebugColor, false, InLifeTime, 0, InThickness);
            }
        }
    }
#endif // ENABLE_DRAW_DEBUG
}

void GCUtils::CollisionQuery::Strength::Debug::DrawStrengthDebugText(const UWorld* InWorld, const FPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const float InLifeTime, const FLinearColor& InFullStrengthColor, const FLinearColor& InNoStrengthColor)
{
#if ENABLE_DRAW_DEBUG
    TArray<TPair<FVector, float>> LocationsWithStrengths;

    LocationsWithStrengths.Emplace(InResult.StrengthSceneCastInfo.StartLocation, InResult.StrengthSceneCastInfo.StartStrength);
    for (const FStrengthHitResult& Hit : InResult.HitResults)
    {
        LocationsWithStrengths.Emplace(Hit.Location, Hit.Strength);
    }
    LocationsWithStrengths.Emplace(InResult.StrengthSceneCastInfo.StopLocation, InResult.StrengthSceneCastInfo.StopStrength);


    const FVector OffsetDirection = FVector::UpVector;
    for (const TPair<FVector, float>& LocationWithStrength : LocationsWithStrengths)
    {
        const FColor StrengthDebugColor = GetDebugColorForStrength(LocationWithStrength.Value, InInitialStrength, InFullStrengthColor, InNoStrengthColor).ToFColor(true);

        const FVector StringLocation = LocationWithStrength.Key/* + (OffsetDirection * 10.f)*/;
        const FString DebugString = FString::Printf(TEXT("%.2f"), LocationWithStrength.Value);
        DrawDebugString(InWorld, StringLocation, DebugString, nullptr, StrengthDebugColor, InLifeTime);
    }
#endif // ENABLE_DRAW_DEBUG
}

void GCUtils::CollisionQuery::Strength::Debug::DrawCollisionShapeDebug(const UWorld* InWorld, const FPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const bool bInPersistentLines, const float InLifeTime, const uint8 InDepthPriority, const float InThickness, const FLinearColor& InFullStrengthColor, const FLinearColor& InNoStrengthColor)
{
#if ENABLE_DRAW_DEBUG
    TArray<TPair<FVector, float>> LocationsWithStrengths;

    LocationsWithStrengths.Emplace(InResult.StrengthSceneCastInfo.StartLocation, InResult.StrengthSceneCastInfo.StartStrength);
    for (const FStrengthHitResult& Hit : InResult.HitResults)
    {
        LocationsWithStrengths.Emplace(Hit.Location, Hit.Strength);
    }
    LocationsWithStrengths.Emplace(InResult.StrengthSceneCastInfo.StopLocation, InResult.StrengthSceneCastInfo.StopStrength);


    const FVector OffsetDirection = FVector::UpVector;
    for (const TPair<FVector, float>& LocationWithStrength : LocationsWithStrengths)
    {
        const FColor StrengthDebugColor = GetDebugColorForStrength(LocationWithStrength.Value, InInitialStrength, InFullStrengthColor, InNoStrengthColor).ToFColor(true);

        const FVector ShapeLocation = LocationWithStrength.Key;
        DebugDrawing::DrawDebugCollisionShape(InWorld, ShapeLocation, InResult.StrengthSceneCastInfo.CollisionShapeCasted, InResult.StrengthSceneCastInfo.CollisionShapeCastedRotation, StrengthDebugColor, 16, bInPersistentLines, InLifeTime, InDepthPriority, InThickness);
    }
#endif // ENABLE_DRAW_DEBUG
}

void GCUtils::CollisionQuery::Strength::Debug::DrawStrengthDebugLine(const UWorld* InWorld, const FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const bool bInPersistentLines, const float InLifeTime, const uint8 InDepthPriority, const float InThickness, const float InSegmentsLength, const float InSegmentsSpacingLength, const FLinearColor& InFullStrengthColor, const FLinearColor& InNoStrengthColor)
{
#if ENABLE_DRAW_DEBUG
    for (const FPenetrationSceneCastWithExitHitsUsingStrengthResult& PenetrationSceneCastWithExitHitsUsingStrengthResult : InResult.PenetrationSceneCastWithExitHitsUsingStrengthResults)
    {
        DrawStrengthDebugLine(InWorld, PenetrationSceneCastWithExitHitsUsingStrengthResult, InInitialStrength, bInPersistentLines, InLifeTime, InDepthPriority, InThickness, InSegmentsLength, InSegmentsSpacingLength, InFullStrengthColor, InNoStrengthColor);
    }
#endif // ENABLE_DRAW_DEBUG
}

void GCUtils::CollisionQuery::Strength::Debug::DrawStrengthDebugText(const UWorld* InWorld, const FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const float InLifeTime, const FLinearColor& InFullStrengthColor, const FLinearColor& InNoStrengthColor)
{
#if ENABLE_DRAW_DEBUG
    FVector PreviousRicochetTextOffsetDirection = FVector::ZeroVector;
    for (int32 i = 0; i < InResult.PenetrationSceneCastWithExitHitsUsingStrengthResults.Num(); ++i)
    {
        const FPenetrationSceneCastWithExitHitsUsingStrengthResult& PenetrationSceneCastWithExitHitsUsingStrengthResult = InResult.PenetrationSceneCastWithExitHitsUsingStrengthResults[i];

        FVector RicochetTextOffsetDirection;

        // Collect locations and strengths
        TArray<TPair<FVector, float>> LocationsWithStrengths;
        LocationsWithStrengths.Emplace(PenetrationSceneCastWithExitHitsUsingStrengthResult.StrengthSceneCastInfo.StartLocation, PenetrationSceneCastWithExitHitsUsingStrengthResult.StrengthSceneCastInfo.StartStrength);
        for (const FStrengthHitResult& Hit : PenetrationSceneCastWithExitHitsUsingStrengthResult.HitResults)
        {
            LocationsWithStrengths.Emplace(Hit.Location, Hit.Strength);

            if (Hit.bIsRicochet)
            {
                // This will be the axis that we shift our debug string locations for the pre-ricochet and post-ricochet strengths
                RicochetTextOffsetDirection = FVector::CrossProduct(PenetrationSceneCastWithExitHitsUsingStrengthResult.StrengthSceneCastInfo.CastDirection, Hit.ImpactNormal).GetSafeNormal();
            }
        }
        LocationsWithStrengths.Emplace(PenetrationSceneCastWithExitHitsUsingStrengthResult.StrengthSceneCastInfo.StopLocation, PenetrationSceneCastWithExitHitsUsingStrengthResult.StrengthSceneCastInfo.StopStrength);

        // Debug them
        const float RicochetTextOffsetAmount = 30.f;
        for (int j = 0; j < LocationsWithStrengths.Num(); ++j)
        {
            if (LocationsWithStrengths.IsValidIndex(j + 1) && LocationsWithStrengths[j] == LocationsWithStrengths[j + 1])
            {
                // Skip duplicates
                continue;
            }

            const FColor StrengthDebugColor = GetDebugColorForStrength(LocationsWithStrengths[j].Value, InInitialStrength, InFullStrengthColor, InNoStrengthColor).ToFColor(true);
            FVector StringLocation = LocationsWithStrengths[j].Key;

            if (j == LocationsWithStrengths.Num() - 1) // if we are the stop location
            {
                // If we are about to ricochet
                if (PenetrationSceneCastWithExitHitsUsingStrengthResult.HitResults.Num() > 0 && PenetrationSceneCastWithExitHitsUsingStrengthResult.HitResults.Last().bIsRicochet)
                {
                    // Offset the pre-ricochet strength debug location upwards
                    StringLocation += (-RicochetTextOffsetDirection * RicochetTextOffsetAmount);

                    // If there is not a post-ricochet line after this
                    if (i == InResult.PenetrationSceneCastWithExitHitsUsingStrengthResults.Num() - 1)
                    {
                        // Then just debug the post-ricochet strength here
                        const FColor PostQueryStrengthDebugColor = GetDebugColorForStrength(InResult.StrengthSceneCastInfo.StopStrength, InInitialStrength, InFullStrengthColor, InNoStrengthColor).ToFColor(true);
                        const FVector PostQueryStringLocation = InResult.StrengthSceneCastInfo.StopLocation + (RicochetTextOffsetDirection * RicochetTextOffsetAmount);
                        const FString PostQueryDebugString = FString::Printf(TEXT("%.2f"), InResult.StrengthSceneCastInfo.StopStrength);
                        DrawDebugString(InWorld, PostQueryStringLocation, PostQueryDebugString, nullptr, PostQueryStrengthDebugColor, InLifeTime);
                    }
                }
            }
            else if (j == 0) // if we are the start location
            {
                // If we came from a ricochet
                if (InResult.PenetrationSceneCastWithExitHitsUsingStrengthResults.IsValidIndex(i - 1) && InResult.PenetrationSceneCastWithExitHitsUsingStrengthResults[i - 1].HitResults.Num() > 0 && InResult.PenetrationSceneCastWithExitHitsUsingStrengthResults[i - 1].HitResults.Last().bIsRicochet)
                {
                    // Offset the post-ricochet strength debug location downwards
                    StringLocation += (PreviousRicochetTextOffsetDirection * RicochetTextOffsetAmount);
                }
            }

            // Debug this location's strength
            const FString DebugString = FString::Printf(TEXT("%.2f"), LocationsWithStrengths[j].Value);
            DrawDebugString(InWorld, StringLocation, DebugString, nullptr, StrengthDebugColor, InLifeTime);
        }

        PreviousRicochetTextOffsetDirection = RicochetTextOffsetDirection;
    }
#endif // ENABLE_DRAW_DEBUG
}

void GCUtils::CollisionQuery::Strength::Debug::DrawCollisionShapeDebug(const UWorld* InWorld, const FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const bool bInPersistentLines, const float InLifeTime, const uint8 InDepthPriority, const float InThickness, const FLinearColor& InFullStrengthColor, const FLinearColor& InNoStrengthColor)
{
#if ENABLE_DRAW_DEBUG
    for (const FPenetrationSceneCastWithExitHitsUsingStrengthResult& PenetrationSceneCastWithExitHitsUsingStrengthResult : InResult.PenetrationSceneCastWithExitHitsUsingStrengthResults)
    {
        DrawCollisionShapeDebug(InWorld, PenetrationSceneCastWithExitHitsUsingStrengthResult, InInitialStrength, false, InLifeTime, 0, 0, InFullStrengthColor, InNoStrengthColor);
    }

    // Draw collision shape debug for stop location on a ricochet. We are relying on the penetration casts to draw our stop locations, but if the query stops from a ricochet nerf, there won't be a next scene cast, so it's up to us to draw the end
    if (InResult.PenetrationSceneCastWithExitHitsUsingStrengthResults.Num() > 0)
    {
        if (InResult.StrengthSceneCastInfo.StopLocation == InResult.PenetrationSceneCastWithExitHitsUsingStrengthResults.Last().StrengthSceneCastInfo.StopLocation)
        {
            const FColor StrengthDebugColor = GetDebugColorForStrength(InResult.StrengthSceneCastInfo.StopStrength, InInitialStrength, InFullStrengthColor, InNoStrengthColor).ToFColor(true);

            const FVector ShapeDebugLocation = InResult.StrengthSceneCastInfo.StopLocation + (InResult.PenetrationSceneCastWithExitHitsUsingStrengthResults.Last().StrengthSceneCastInfo.CastDirection * SceneCastStartWallAvoidancePadding);
            DebugDrawing::DrawDebugCollisionShape(InWorld, ShapeDebugLocation, InResult.StrengthSceneCastInfo.CollisionShapeCasted, InResult.StrengthSceneCastInfo.CollisionShapeCastedRotation, StrengthDebugColor, 16, bInPersistentLines, InLifeTime, InDepthPriority, InThickness);
        }
    }
#endif // ENABLE_DRAW_DEBUG
}

FLinearColor GCUtils::CollisionQuery::Strength::Debug::GetDebugColorForStrength(const float InStrength, const float InInitialStrength, const FLinearColor& InFullStrengthColor, const FLinearColor& InNoStrengthColor)
{
    return FLinearColor::LerpUsingHSV(InFullStrengthColor, InNoStrengthColor, 1 - (InStrength / InInitialStrength));
}
