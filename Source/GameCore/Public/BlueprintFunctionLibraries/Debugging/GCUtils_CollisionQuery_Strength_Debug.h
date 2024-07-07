// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"

struct FPenetrationSceneCastWithExitHitsUsingStrengthResult;
struct FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult;

/**
 * Draw debug functions for GCBlueprintFunctionLibrary_StrengthCollisionQueries
 */
namespace GCUtils::CollisionQuery::Strength::Debug
{
    /** Draws line representing this scene cast, representing strength in color */
    GAMECORE_API void DrawStrengthDebugLine(const UWorld* InWorld, const FPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const bool bInPersistentLines = false, const float InLifeTime = -1.f, const uint8 InDepthPriority = 0, const float InThickness = 0.f, const float InSegmentsLength = 10.f, const float InSegmentsSpacingLength = 0.f, const FLinearColor& InFullStrengthColor = FLinearColor::Green, const FLinearColor& InNoStrengthColor = FLinearColor::Red);
    /** Draws text representing this scene cast, indicating strength at significant points */
    GAMECORE_API void DrawStrengthDebugText(const UWorld* InWorld, const FPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const float InLifeTime = -1.f, const FLinearColor& InFullStrengthColor = FLinearColor::Green, const FLinearColor& InNoStrengthColor = FLinearColor::Red);
    /** Draws casted collision shape at significant points */
    GAMECORE_API void DrawCollisionShapeDebug(const UWorld* InWorld, const FPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const bool bInPersistentLines = false, const float InLifeTime = -1.f, const uint8 InDepthPriority = 0, const float InThickness = 0.f, const FLinearColor& InFullStrengthColor = FLinearColor::Green, const FLinearColor& InNoStrengthColor = FLinearColor::Red);


    /** Draws line representing this scene cast, representing strength in color */
    GAMECORE_API void DrawStrengthDebugLine(const UWorld* InWorld, const FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const bool bInPersistentLines = false, const float InLifeTime = -1.f, const uint8 InDepthPriority = 0, const float InThickness = 0.f, const float InSegmentsLength = 10.f, const float InSegmentsSpacingLength = 0.f, const FLinearColor& InFullStrengthColor = FLinearColor::Green, const FLinearColor& InNoStrengthColor = FLinearColor::Red);
    /** Draws text representing this scene cast, indicating strength at significant points */
    GAMECORE_API void DrawStrengthDebugText(const UWorld* InWorld, const FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const float InLifeTime = -1.f, const FLinearColor& InFullStrengthColor = FLinearColor::Green, const FLinearColor& InNoStrengthColor = FLinearColor::Red);
    /** Draws casted collision shape at significant points */
    GAMECORE_API void DrawCollisionShapeDebug(const UWorld* InWorld, const FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const bool bInPersistentLines = false, const float InLifeTime = -1.f, const uint8 InDepthPriority = 0, const float InThickness = 0.f, const FLinearColor& InFullStrengthColor = FLinearColor::Green, const FLinearColor& InNoStrengthColor = FLinearColor::Red);
}
