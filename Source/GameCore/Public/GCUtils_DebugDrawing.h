// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#if UE_ENABLE_DEBUG_DRAWING
/**
 *
 */
namespace GCUtils::DebugDrawing
{
    /**
     * @brief Draw a debug shape to represent the passed in `FCollisionShape`.
     */
    GAMECORE_API void DrawDebugCollisionShape(
        const UWorld* inWorld,
        const FVector& inCenter,
        const FCollisionShape& inCollisionShape,
        const FQuat& inRotation,
        const FColor& inColor,
        const int32 inNumSegments = 16,
        const bool inShouldLinesPersistent = false,
        const float inLifetime = -1.f,
        const uint8 inDepthPriority = 0u,
        const float inThickness = 0.f);

    /**
     * @brief Draw a debug line formed by many smaller, spaced segments to appear as a dotted line.
     */
    GAMECORE_API void DrawDebugLineDotted(
        const UWorld* inWorld,
        const FVector& inStart,
        const FVector& inEnd,
        const FColor& inColor,
        const bool inShouldLinesPersistent = false,
        const float inLifetime = -1.f,
        const uint8 inDepthPriority = 0u,
        const float inThickness = 0.f,
        const FVector::FReal inSegmentsLength = 10.0,
        const FVector::FReal inSegmentsSpacingLength = 10.0);
}
#endif // UE_ENABLE_DEBUG_DRAWING
