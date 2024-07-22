// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_Math.h"

DEFINE_LOG_CATEGORY(LogGCUtils_Math);

namespace GCUtils::Math
{
    template <bool shouldBeInclusive>
    static bool IsDirectionBetween(
        const FVector& inBoundDirA,
        const FVector& inBoundDirB,
        const FVector& inTestDir,
        const FVector::FReal inErrorTolerance);
}

float GCUtils::Math::GetCollisionShapeBoundingSphereRadius(const FCollisionShape& inCollisionShape)
{
    switch (inCollisionShape.ShapeType)
    {
    case ECollisionShape::Box:
        return GetBoxExtentBoundingSphereRadius(
            FVector3f(
                inCollisionShape.Box.HalfExtentX * 2.f,
                inCollisionShape.Box.HalfExtentY * 2.f,
                inCollisionShape.Box.HalfExtentZ * 2.f
                )
            );
    case ECollisionShape::Sphere:
        return inCollisionShape.Sphere.Radius;
    case ECollisionShape::Capsule:
        return inCollisionShape.Capsule.HalfHeight;
    case ECollisionShape::Line:
        return 0.f; // A LineShape is just a point.
    }

    return 0.f;
}

constexpr float GCUtils::Math::GetBoxExtentBoundingSphereRadius(const FVector3f& inBoxExtent)
{
    const float diameter = GetVectorSize(inBoxExtent);
    return diameter / 2.f;
}

bool GCUtils::Math::IsDirectionBetweenInclusive(
    const FVector& inBoundDirA,
    const FVector& inBoundDirB,
    const FVector& inTestDir,
    const FVector::FReal inErrorTolerance)
{
    constexpr bool shouldBeInclusive = true;
    return IsDirectionBetween<shouldBeInclusive>(inBoundDirA, inBoundDirB, inTestDir, inErrorTolerance);
}

bool GCUtils::Math::IsDirectionBetweenExclusive(
    const FVector& inBoundDirA,
    const FVector& inBoundDirB,
    const FVector& inTestDir,
    const FVector::FReal inErrorTolerance)
{
    constexpr bool shouldBeInclusive = false;
    return IsDirectionBetween<shouldBeInclusive>(inBoundDirA, inBoundDirB, inTestDir, inErrorTolerance);
}

bool GCUtils::Math::DoesPointLieOnSegment(
    const FVector& inEndpointA,
    const FVector& inEndpointB,
    const FVector& inTestPoint,
    const FVector::FReal inErrorTolerance)
{
    if (!ArePointsCollinear({ inEndpointA, inEndpointB, inTestPoint }, inErrorTolerance))
    {
        // The test point is not on the line that the two endpoints are on.
        return false;
    }

    const FVector endpointAToTestPoint = inTestPoint - inEndpointA;
    const FVector endpointBToTestPoint = inTestPoint - inEndpointB;

    // Return true if the two deltas point in opposite directions.
    return FVector::DotProduct(endpointAToTestPoint, endpointBToTestPoint) <= 0;
}

bool GCUtils::Math::ArePointsCollinear(
    const TArrayView<const FVector>& inPoints,
    const FVector::FReal inErrorTolerance)
{
    if (inPoints.Num() <= 2)
    {
        // Two points or less are considered collinear.
        return true;
    }

    // The first two points make our line.
    const FVector lineDirection = (inPoints[1] - inPoints[0]).GetSafeNormal();

    // For each of the rest of the points, see if any are not on the line.
    for (int32 i = 2; i < inPoints.Num(); ++i)
    {
        const FVector directionFromStartPointToCurrentPoint = (inPoints[i] - inPoints[0]).GetSafeNormal();

        const FVector::FReal dotOfCurrentDirectionAndLineDirection = FVector::DotProduct(directionFromStartPointToCurrentPoint, lineDirection);

        const bool isSameDirection = FMath::IsNearlyEqual(dotOfCurrentDirectionAndLineDirection, 1.0, inErrorTolerance);
        const bool isOppositeDirection = FMath::IsNearlyEqual(dotOfCurrentDirectionAndLineDirection, -1.0, inErrorTolerance);

        const bool isParallel = isSameDirection || isOppositeDirection;
        if (!isParallel)
        {
            // Not collinear.
            return false;
        }
    }

    // Collinear. All points lie on the same line.
    return true;
}

bool GCUtils::Math::DoesPointLieOnTriangle(
    const FVector& inEdgeA,
    const FVector& inEdgeB,
    const FVector& inEdgeC,
    const FVector& inTestPoint,
    const FVector::FReal inErrorTolerance)
{
    {
        const FVector edgeAToTestPoint = inTestPoint - inEdgeA;
        const FVector edgeAToEdgeB = inEdgeB - inEdgeA;
        const FVector edgeAToEdgeC = inEdgeC - inEdgeA;
        const bool isTestPointWithinAngleA = IsDirectionBetweenInclusive(edgeAToEdgeB, edgeAToEdgeC, edgeAToTestPoint, inErrorTolerance);

        if (!isTestPointWithinAngleA)
        {
            //
            // Point is not on the triangle.
            //
            //          C
            //         / \
            //      o /   \
            //       /     \
            //      /       \
            //     /         \
            //    A _________ B
            //
            return false;
        }
    }

    {
        const FVector edgeBToTestPoint = inTestPoint - inEdgeB;
        const FVector edgeBToEdgeA = inEdgeA - inEdgeB;
        const FVector edgeBToEdgeC = inEdgeC - inEdgeB;
        const bool isTestPointWithinAngleB = IsDirectionBetweenInclusive(edgeBToEdgeA, edgeBToEdgeC, edgeBToTestPoint, inErrorTolerance);

        if (!isTestPointWithinAngleB)
        {
            //
            // Point is not on the triangle.
            //
            //          C
            //         / \
            //        /   \ o
            //       /     \
            //      /       \
            //     /         \
            //    A _________ B
            //
            return true;
        }
    }

    //
    // Point is on the triangle.
    //
    //          C
    //         / \
    //        /   \
    //       / o   \
    //      /       \
    //     /         \
    //    A _________ B
    //
    return false;
}

FVector GCUtils::Math::GetLocationAimDirection(
    const UWorld& inWorld,
    FCollisionQueryParams inCollisionQueryParams,
    const FVector& inAimPoint,
    const FVector& inAimDir,
    const FVector::FReal inMaxRange,
    const FVector& inNewAimPoint)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Math::GetLocationAimDirection);

    if (inNewAimPoint.Equals(inAimPoint))
    {
        // The new aim point is the same as the aim point. We can skip the camera trace and just return the aim
        // direction as the muzzle's direction.
        return inAimDir;
    }

    // Perform line trace from `inNewAimPoint` to the point that `inAimDir` is looking at.

    inCollisionQueryParams.bIgnoreTouches = true;

    const FVector traceStart = inAimPoint;
    const FVector traceEnd = inAimPoint + (inAimDir * inMaxRange);

    constexpr ECollisionChannel traceChannel = ECollisionChannel::ECC_Visibility;

    FHitResult hitResult;
    const bool didHitBlockingHit = inWorld.LineTraceSingleByChannel(
        hitResult,
        traceStart,
        traceEnd,
        traceChannel,
        inCollisionQueryParams);

    if (!didHitBlockingHit)
    {
        // The aim direction is not looking at anything for our new aim point to point towards.
        return (traceEnd - inNewAimPoint).GetSafeNormal();
    }

    // Return the direction from the new aim point to the point that the player is looking at.
    return (hitResult.Location - inNewAimPoint).GetSafeNormal();
}

template <bool shouldBeInclusive>
bool GCUtils::Math::IsDirectionBetween(
    const FVector& inBoundDirA,
    const FVector& inBoundDirB,
    const FVector& inTestDir,
    const FVector::FReal inErrorTolerance)
{
    //
    // Get the normals.
    //
    // inBoundDirA ^
    //             |        inTestDir
    //             |          ^
    //             | normalA /
    //             |        /
    //             |       /
    //             |      /
    //             |     /
    //             |    /
    //             |   /  normalB
    //             |  /
    //             | /
    //             |/_____________>
    //                        inBoundDirB
    //

    FVector normalA = FVector::CrossProduct(inBoundDirA, inTestDir);
    if (normalA.IsNormalized() == false)
    {
        normalA.Normalize();
    }

    FVector normalB = FVector::CrossProduct(inTestDir, inBoundDirB);
    if (normalB.IsNormalized() == false)
    {
        normalB.Normalize();
    }

    if constexpr (shouldBeInclusive)
    {
        // Return true if the test direction lies on one of the bounding directions.

        if (normalA.IsNearlyZero(inErrorTolerance))
        {
            // Cross product with bounding direction A is zero. Means we are on direction A.
            return true;
        }

        if (normalB.IsNearlyZero(inErrorTolerance))
        {
            // Cross product with bounding direction B is zero. Means we are on direction B.
            return true;
        }
    }

    // Return whether the normals point in the same direction. If they are not opposites of each
    // other, that means the test direction lies between them.
    return normalA.Equals(normalB, inErrorTolerance);
}
