// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"

/**
 *
 */
namespace GCUtils::DrawDebug
{
    /** Accepts a generic FCollisionShape to draw */
    GAMECORE_API void DrawDebugCollisionShape(const UWorld* InWorld, const FVector& InCenter, const FCollisionShape& InCollisionShape, const FQuat& InRotation, const FColor& InColor, const int32 InSegments = 16, const bool bInPersistentLines = false, const float InLifeTime = -1.f, const uint8 InDepthPriority = 0, const float InThickness = 0.f);

    GAMECORE_API void DrawDebugLineDotted(const UWorld* InWorld, const FVector& InStart, const FVector& InEnd, const FColor& InColor, const bool bInPersistentLines = false, const float InLifeTime = -1.f, const uint8 InDepthPriority = 0, const float InThickness = 0.f, const float InSegmentsLength = 10.f, const float InSegmentsSpacingLength = 10.f);
}
