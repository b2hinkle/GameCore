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
        const UWorld& world,
        const FVector& center,
        const FCollisionShape& collisionShape,
        const FQuat& rotation,
        const FColor& color,
        const int32 numSegments = 16,
        const bool shouldLinesPersist = false,
        const float lifetime = -1.f,
        const uint8 depthPriority = 0u,
        const float thickness = 0.f);

    /**
     * @brief Draw a debug line formed by many smaller, spaced segments to appear as a dotted line.
     */
    GAMECORE_API void DrawDebugLineDotted(
        const UWorld* inWorld,
        const FVector& inStart,
        const FVector& inEnd,
        const FColor& inColor,
        const bool inShouldLinesPersist = false,
        const float inLifetime = -1.f,
        const uint8 inDepthPriority = 0u,
        const float inThickness = 0.f,
        const FVector::FReal inSegmentsLength = 10.0,
        const FVector::FReal inSegmentsSpacingLength = 10.0);

    /**
     * @brief Draw a debug point with an arrowhead pointing in a direction. Overload that
     *        provides an easy, automatic arrowhead length and angle based on the given thickness.
     */
    GAMECORE_API void DrawDebugArrowPoint(
        const UWorld* inWorld,
        const FVector& inPoint,
        const FVector& inDirection,
        const FVector::FReal inArrowLength,
        const FColor& inColor,
        const bool inShouldLinesPersist = false,
        const float inLifetime = -1.f,
        const uint8 inDepthPriority = 0u,
        const float inThickness = 0.f);
    /**
     * @brief Draw a debug point with an arrowhead pointing in a direction.
     */
    GAMECORE_API void DrawDebugArrowPoint(
        const UWorld* inWorld,
        const FVector::FReal inArrowheadLength,
        const FVector::FReal inArrowheadAngleRadians,
        const FVector& inPoint,
        const FVector& inDirection,
        const FVector::FReal inArrowLength,
        const FColor& inColor,
        const bool inShouldLinesPersist = false,
        const float inLifetime = -1.f,
        const uint8 inDepthPriority = 0u,
        const float inThickness = 0.f);

    /**
     * @brief Draw a debug line with an arrowhead pointing beyond the endpoint. Overload that
     *        provides an easy, automatic arrowhead length and angle based on the given thickness.
     */
    GAMECORE_API void DrawDebugArrowLine(
        const UWorld* inWorld,
        const FVector& inStart,
        const FVector& inEnd,
        const FColor& inColor,
        const bool inShouldLinesPersist = false,
        const float inLifetime = -1.f,
        const uint8 inDepthPriority = 0u,
        const float inThickness = 0.f);
    /**
     * @brief Draw a debug line with an arrowhead pointing beyond the endpoint.
     */
    GAMECORE_API void DrawDebugArrowLine(
        const UWorld* inWorld,
        const FVector::FReal inArrowheadLength,
        const FVector::FReal inArrowheadAngleRadians,
        const FVector& inStart,
        const FVector& inEnd,
        const FColor& inColor,
        const bool inShouldLinesPersist = false,
        const float inLifetime = -1.f,
        const uint8 inDepthPriority = 0u,
        const float inThickness = 0.f);
}
#endif // UE_ENABLE_DEBUG_DRAWING
