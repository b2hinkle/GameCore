// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGCUtils_HitResult, Log, All);

/**
 * A collection of helpful functions related to Hit Results.
 * Helpful for getting certain data from Hit Results and more.
 */
namespace GCUtils::HitResult
{
    /** Returns true if the two given hits were from the same trace */
    GAMECORE_API bool AreHitsFromSameTrace(const FHitResult& HitA, const FHitResult& HitB);

    /**
     * Avoids having to run a square root distance calculation to calculate a hit's trace's length.
     * If your hit has a known Time value of 0, you minus well just call FVector::Distance() since that is this function's fallback calculation for this case.
     */
    GAMECORE_API float CheapCalculateTraceLength(const FHitResult& InHit);

    /**
     * Adjust a hit result's TraceStart and TraceEnd while keeping its trace-related data (time and distance) consistent with the modification.
     * It's rare when you would want to change a HitResult's gathered data, but this becomes useful when creating custom collision queries where you need to adjust data of the hits to more accuratly describe your query.
     *
     * @param  InOutHit                 Hit to modify
     * @param  InTimeAtNewTraceStart    Time of where the new TraceStart location is. Lessthan/Greaterthan 0 to grow/shrink the length of the trace, respectively.
     * @param  InTimeAtNewTraceEnd      Time of where the new TraceEnd location is.   Greaterthan/Lessthan 1 to grow/shrink the length of the trace, respectively.
     */
    GAMECORE_API void AdjustTraceDataBySlidingTraceStartAndEndByTime(FHitResult& InOutHit, const float InTimeAtNewTraceStart, const float InTimeAtNewTraceEnd);
}
