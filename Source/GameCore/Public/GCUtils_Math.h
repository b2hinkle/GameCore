// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"

GAMECORE_API DECLARE_LOG_CATEGORY_EXTERN(LogGCUtils_Math, Log, All);

/**
 * @brief Math utilities.
 */
namespace GCUtils::Math
{
    /**
     * @brief Get the radius of a collision shape's bounding sphere.
     */
    GAMECORE_API float GetCollisionShapeBoundingSphereRadius(const FCollisionShape& CollisionShape);

    /**
     * @brief Get the radius of a box's bounding sphere.
     * @param inBoxExtent Extents of the box.
     */
    GAMECORE_API constexpr float GetBoxExtentBoundingSphereRadius(const FVector3f& inBoxExtent);

    /**
     * @brief Constexpr version of `UE::Math::TVector<>::Size()`.
     */
    template <class T>
    GAMECORE_API constexpr T GetVectorSize(const UE::Math::TVector<T>& inVector);

    /**
     * @brief Constexpr version of `UE::Math::TVector<>::SizeSquared()`.
     */
    template <class T>
    GAMECORE_API constexpr T GetVectorSizeSquared(const UE::Math::TVector<T>& inVector);

    /**
     * @brief Determine whether a test direction lies between two bounding directions. Includes
     *        the bounds.
     * @note The three directions must be coplanar for this test to return true.
     * @param inBoundDirA One of the bounding directions.
     * @param inBoundDirB One of the bounding directions.
     * @param inTestDir The test direction.
     */
    GAMECORE_API bool IsDirectionBetweenInclusive(
        const FVector& inBoundDirA,
        const FVector& inBoundDirB,
        const FVector& inTestDir,
        const FVector::FReal inErrorTolerance = UE_DOUBLE_KINDA_SMALL_NUMBER);

    /**
     * @brief Determine whether a test direction lies between two bounding directions. Excludes
     *        the bounds.
     * @note The three directions must be coplanar for this test to return true.
     * @param inBoundDirA One of the bounding directions.
     * @param inBoundDirB One of the bounding directions.
     * @param inTestDir The test direction.
     */
    GAMECORE_API bool IsDirectionBetweenExclusive(
        const FVector& inBoundDirA,
        const FVector& inBoundDirB,
        const FVector& inDirection,
        const FVector::FReal inErrorTolerance = UE_DOUBLE_KINDA_SMALL_NUMBER);

    /**
     * @brief Determine whether a test point lies on a segment.
     * @note The three points must be collinear for this test to return true.
     * @param inEndpointA One of the endpoints of the segment.
     * @param inEndpointB One of the endpoints of the segment.
     * @param inTestPoint The test point.
     */
    GAMECORE_API bool DoesPointLieOnSegment(
        const FVector& inEndpointA,
        const FVector& inEndpointB,
        const FVector& inTestPoint,
        const FVector::FReal inErrorTolerance = UE_DOUBLE_KINDA_SMALL_NUMBER);

    /**
     * @brief Determine whether all points exist on a common line.
     * @param inPoints Array of points to test.
     */
    GAMECORE_API bool ArePointsCollinear(
        const TArrayView<const FVector>& inPoints,
        const FVector::FReal inErrorTolerance = UE_DOUBLE_KINDA_SMALL_NUMBER);

    /**
     * @brief Determine whether a test point lies on a triangle.
     * @param inEdgeA One of the corners of the triangle.
     * @param inEdgeB One of the corners of the triangle.
     * @param inEdgeC One of the corners of the triangle.
     * @param inTestPoint The test point.
     */
    GAMECORE_API bool DoesPointLieOnTriangle(
        const FVector& inEdgeA,
        const FVector& inEdgeB,
        const FVector& inEdgeC,
        const FVector& inTestPoint,
        const FVector::FReal inErrorTolerance = UE_DOUBLE_KINDA_SMALL_NUMBER);

    /**
     * @brief Get the direction from a new aim point to the point we are aiming at. E.g., to
     *        have a weapon's muzzle aim towards the player's look point.
     * @todo This is an old gameplay-related function that is useful but could use more cleanup,
     *       improvement, generalization, and documentation.
     * @param inWorld Reference to world to perform collision query.
     * @param inCollisionQueryParams Params used for the collision query.
     * @param inAimPoint Current aim point.
     * @param inAimDir Current aim direction.
     * @param inMaxRange Max range to trace for a new aim direction.
     * @param inNewAimPoint New aim point. This determines where the new aim direction will start from.
     */
    GAMECORE_API FVector GetLocationAimDirection(
        const UWorld& inWorld,
        FCollisionQueryParams inCollisionQueryParams,
        const FVector& inAimPoint,
        const FVector& inAimDir,
        const FVector::FReal inMaxRange,
        const FVector& inNewAimPoint);

    /**
     * @brief Perform linear interpolation on any number of values.
     * @param inValues Array of values to interpolate between.
     * @param inAlpha Interpolation alpha. Zero yields the first value as a result and
     *                one yields last value as a result. Values outside zero and one are clamped.
     */
    template <class TValue, class TAlpha>
    constexpr TValue LerpMultiple(const TArrayView<const TValue>& inValues, TAlpha&& inAlpha);
}

#include "GCUtils_Log.h"
#include <type_traits>

template <class T>
constexpr T GCUtils::Math::GetVectorSize(const UE::Math::TVector<T>& inVector)
{
    return FMath::Sqrt(GetVectorSizeSquared(inVector));
}

template <class T>
constexpr T GCUtils::Math::GetVectorSizeSquared(const UE::Math::TVector<T>& inVector)
{
    return FMath::Square(inVector.X) + FMath::Square(inVector.Y) + FMath::Square(inVector.Z);
}

template <class TValue, class TAlpha>
constexpr TValue GCUtils::Math::LerpMultiple(const TArrayView<const TValue>& inValues, TAlpha&& inAlpha)
{
    if (inValues.Num() <= 0)
    {
        GC_LOG_STR_NO_CONTEXT(
            LogGCUtils_Math,
            Warning,
            TEXT("Not given any values to lerp between! Returning a default value.")
            );
        return TValue{};
    }

    if (inValues.Num() == 1)
    {
        // Only one value. This is like a plot with a constant value. Return it.
        return inValues[0];
    }

    using FAlpha = std::remove_cvref_t<TAlpha>;

    // Scale the alpha by the number of lerps.
    const int32 numLerps = inValues.Num() - 1;
    FAlpha alphaScaled = Forward<TAlpha>(inAlpha) * numLerps;

    // Determine the indexes of our A and B values we want to lerp between.
    int32 aIndex = FMath::FloorToInt(alphaScaled);
    int32 bIndex = FMath::CeilToInt(alphaScaled);

    // Handle potential non-zero-to-one alpha.
    {
        const int32 lastIndex = inValues.Num() - 1;

        if (bIndex > lastIndex)
        {
            // Index of B is beyond the last value index. Cap it to the index of the last
            // value and assign index of A to the one before the last.
            bIndex = lastIndex;
            aIndex = lastIndex - 1;
        }
        else if (aIndex < 0)
        {
            // Index of A is before the first value index. Cap it to the index of the first
            // value and assign index of B to the one after the first.
            aIndex = 0;
            bIndex = 1;
        }
    }

    // Get the alpha relative to these A and B indexes. I.e., the decimal place of our scaled alpha.
    FAlpha alphaRelative = MoveTemp(alphaScaled) - aIndex;

    // Perform the lerp. This is a lerp relative to the values of A and B.
    return FMath::Lerp<TValue, FAlpha>(inValues[aIndex], inValues[bIndex], MoveTemp(alphaRelative));
}
