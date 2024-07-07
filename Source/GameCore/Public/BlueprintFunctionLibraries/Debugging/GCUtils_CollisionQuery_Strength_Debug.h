// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GCUtils_CollisionQuery_Strength_Debug.generated.h"


struct FPenetrationSceneCastWithExitHitsUsingStrengthResult;
struct FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult;



/**
 * Draw debug functions for GCBlueprintFunctionLibrary_StrengthCollisionQueries
 */
UCLASS()
class GAMECORE_API UGCBlueprintFunctionLibrary_DrawDebugHelpersStrengthCollisionQueries : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Draws line representing this scene cast, representing strength in color */
    static void DrawStrengthDebugLine(const UWorld* InWorld, const FPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const bool bInPersistentLines = false, const float InLifeTime = -1.f, const uint8 InDepthPriority = 0, const float InThickness = 0.f, const float InSegmentsLength = 10.f, const float InSegmentsSpacingLength = 0.f, const FLinearColor& InFullStrengthColor = FLinearColor::Green, const FLinearColor& InNoStrengthColor = FLinearColor::Red);
    /** Draws text representing this scene cast, indicating strength at significant points */
    static void DrawStrengthDebugText(const UWorld* InWorld, const FPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const float InLifeTime = -1.f, const FLinearColor& InFullStrengthColor = FLinearColor::Green, const FLinearColor& InNoStrengthColor = FLinearColor::Red);
    /** Draws casted collision shape at significant points */
    static void DrawCollisionShapeDebug(const UWorld* InWorld, const FPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const bool bInPersistentLines = false, const float InLifeTime = -1.f, const uint8 InDepthPriority = 0, const float InThickness = 0.f, const FLinearColor& InFullStrengthColor = FLinearColor::Green, const FLinearColor& InNoStrengthColor = FLinearColor::Red);


    /** Draws line representing this scene cast, representing strength in color */
    static void DrawStrengthDebugLine(const UWorld* InWorld, const FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const bool bInPersistentLines = false, const float InLifeTime = -1.f, const uint8 InDepthPriority = 0, const float InThickness = 0.f, const float InSegmentsLength = 10.f, const float InSegmentsSpacingLength = 0.f, const FLinearColor& InFullStrengthColor = FLinearColor::Green, const FLinearColor& InNoStrengthColor = FLinearColor::Red);
    /** Draws text representing this scene cast, indicating strength at significant points */
    static void DrawStrengthDebugText(const UWorld* InWorld, const FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const float InLifeTime = -1.f, const FLinearColor& InFullStrengthColor = FLinearColor::Green, const FLinearColor& InNoStrengthColor = FLinearColor::Red);
    /** Draws casted collision shape at significant points */
    static void DrawCollisionShapeDebug(const UWorld* InWorld, const FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult& InResult, const float InInitialStrength, const bool bInPersistentLines = false, const float InLifeTime = -1.f, const uint8 InDepthPriority = 0, const float InThickness = 0.f, const FLinearColor& InFullStrengthColor = FLinearColor::Green, const FLinearColor& InNoStrengthColor = FLinearColor::Red);

private:
    static FLinearColor GetDebugColorForStrength(const float InStrength, const float InInitialStrength, const FLinearColor& InFullStrengthColor = FLinearColor::Green, const FLinearColor& InNoStrengthColor = FLinearColor::Red);
};
