// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GCUtils_Math.generated.h"



/**
 * A collection of common math helpers
 */
UCLASS()
class GAMECORE_API UGCBlueprintFunctionLibrary_MathHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Gets the radius of a collision shape's bounding sphere
     */
    static float GetCollisionShapeBoundingSphereRadius(const FCollisionShape& CollisionShape);

    /**
     * Gets the radius of a box's bounding sphere
     */
    UFUNCTION(BlueprintPure, Category = "MathHelpers|GeometryMath")
        static float GetBoxBoundingSphereRadius(const FVector& BoxExtent);


    /**
     * Given two bounding directions A and B and a test direction, does the direction lie between them?
     * Only can return true if the three directions are coplanar.
     */
    UFUNCTION(BlueprintPure, Category = "MathHelpers|VectorMath")
        static bool DirectionIsBetween(const FVector& InA, const FVector& InB, const bool bInInclusive, const FVector& InDirection, const float InErrorTolerance = 1.e-4f); // InErrorTolerance = KINDA_SMALL_NUMBER

    /**
     * Given a segment and a point, does that point lie on the segment?
     */
    UFUNCTION(BlueprintPure, Category = "MathHelpers|VectorMath")
        static bool PointLiesOnSegment(const FVector& InSegmentStart, const FVector& InSegmentEnd, const FVector& InPoint, const float InErrorTolerance = 1.e-4f); // InErrorTolerance = KINDA_SMALL_NUMBER

    /**
     * Given three points, do they exist on the same line?
     */
    UFUNCTION(BlueprintPure, Category = "MathHelpers|VectorMath")
        static bool PointsAreCollinear(const TArray<FVector>& InPoints, const float InErrorTolerance = 1.e-4f); // InErrorTolerance = KINDA_SMALL_NUMBER

    /**
     * Given a triangle and a point, does that point lie on the triangle?
     */
    UFUNCTION(BlueprintPure, Category = "MathHelpers|VectorMath")
        static bool PointLiesOnTriangle(const FVector& InA, const FVector& InB, const FVector& InC, const FVector& InPoint, const float InErrorTolerance = 1.e-4f); // InErrorTolerance = KINDA_SMALL_NUMBER


    /**
     * Gets the direction from the Location to the point that AimDir is looking at.
     * E.g. having a weapon's muzzle aim towards the Player's look point.
     */
    static FVector GetLocationAimDirection(const UWorld* InWorld, const FCollisionQueryParams& Params, const FVector& AimPoint, const FVector& AimDir, const float& MaxRange, const FVector& Location);

    /** Performs linear interpolation on any number of values */
    template <class T>
    static T LerpMultiple(const TArray<T>& InValues, const float InAlpha)
    {
        if (InValues.Num() <= 0)
        {
            UE_LOG(/*LogGCMathHelpers*/LogTemp, Warning, TEXT("%s() was not given any values to lerp between! Returning a default value."), ANSI_TO_TCHAR(__FUNCTION__)); // NOTE: could not figure out how to use the private LogGCMathHelpers log category in this templated function so we are just using LogTemp
            return T();
        }

        // Scale the alpha by the number of lerps
        const int32 NumberOfLerps = (InValues.Num() - 1);
        const float ScaledAlpha = (InAlpha * NumberOfLerps);

        // Get the indexes of A and B to lerp between
        int32 IndexOfA = FMath::FloorToInt(ScaledAlpha);
        int32 IndexOfB = FMath::CeilToInt(ScaledAlpha);

        // Check for alpha outside of 0 and 1
        {
            const int32 LastIndex = InValues.Num() - 1;
            if (IndexOfB > LastIndex)
            {
                IndexOfB = LastIndex;
                IndexOfA = LastIndex - 1;
                IndexOfA = FMath::Max(IndexOfA, 0); // keep it in range
            }
            else if (IndexOfA < 0)
            {
                IndexOfA = 0;
                IndexOfB = 1;
                IndexOfB = FMath::Min(IndexOfB, LastIndex); // keep it in range
            }
        }

        // Get the alpha relative to this A and B (the decimal part of our ScaledAlpha)
        const float LerpAlpha = ScaledAlpha - IndexOfA;

        // Perform the lerp
        return FMath::Lerp<T>(InValues[IndexOfA], InValues[IndexOfB], LerpAlpha);
    }
};
