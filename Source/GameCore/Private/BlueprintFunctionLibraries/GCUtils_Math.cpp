// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueprintFunctionLibraries/GCUtils_Math.h"

float GCUtils::Math::GetCollisionShapeBoundingSphereRadius(const FCollisionShape& CollisionShape)
{
    switch (CollisionShape.ShapeType)
    {
        case ECollisionShape::Box:
        {
            const FVector BoxExtent = FVector(CollisionShape.Box.HalfExtentX * 2, CollisionShape.Box.HalfExtentY * 2, CollisionShape.Box.HalfExtentZ * 2);
            return GetBoxBoundingSphereRadius(BoxExtent);
        }
        case  ECollisionShape::Sphere:
        {
            return CollisionShape.Sphere.Radius;
        }
        case ECollisionShape::Capsule:
        {
            return CollisionShape.Capsule.HalfHeight;
        }
        case ECollisionShape::Line:
        {
            // A LineShape is just a point
            return 0.f;
        }
    }

    return 0.f;
}

float GCUtils::Math::GetBoxBoundingSphereRadius(const FVector& BoxExtent)
{
    const float LengthSquared = (BoxExtent.X * BoxExtent.X);
    const float WidthSquared  = (BoxExtent.Y * BoxExtent.Y);
    const float HeightSquared = (BoxExtent.Z * BoxExtent.Z);

    float Diameter = FMath::Sqrt(LengthSquared + WidthSquared + HeightSquared); // 3d pythagorean theorem

    // Or you can get the distance across opposite corners
    // float Diameter = FVector::Distance(FVector(0.f, 0.f, 0.f), BoxExtent)

    return Diameter / 2;
}

bool GCUtils::Math::DirectionIsBetween(const FVector& InA, const FVector& InB, const bool bInInclusive, const FVector& InDirection, const float InErrorTolerance)
{
    // Get the normals
    //
    // InA ^
    //     |        InDirection
    //     |          ^
    //     | NormalA /
    //     |        /
    //     |       /
    //     |      /
    //     |     /
    //     |    /
    //     |   /  NormalB
    //     |  /
    //     | /
    //     |/_____________>
    //                    InB
    //
    const FVector NormalA = FVector::CrossProduct(InA, InDirection).GetSafeNormal();
    const FVector NormalB = FVector::CrossProduct(InDirection, InB).GetSafeNormal();

    // If we are inclusive, check if the direction is on one of the bounding directions
    if (bInInclusive)
    {
        // Cross product of zero means that the direction is on the bound
        if (NormalA.IsNearlyZero(InErrorTolerance) || NormalB.IsNearlyZero(InErrorTolerance))
        {
            return true;
        }
    }

    // If the two normals are not opposites, then we are within the bounding directions
    const bool bSameNormals = FMath::IsNearlyEqual(FVector::DotProduct(NormalA, NormalB), 1.f, InErrorTolerance);
    return bSameNormals;
}

bool GCUtils::Math::PointLiesOnSegment(const FVector& InSegmentStart, const FVector& InSegmentEnd, const FVector& InPoint, const float InErrorTolerance)
{
    if (PointsAreCollinear({ InSegmentStart, InSegmentEnd, InPoint }, InErrorTolerance))
    {
        // The point is on the line that start and end is on

        const FVector StartToPoint = (InPoint - InSegmentStart);
        const FVector EndToPoint = (InPoint - InSegmentEnd);
        return (FVector::DotProduct(StartToPoint, EndToPoint) <= 0); // true if they are opposite directions
    }

    // Point is not on the segment
    return false;
}

bool GCUtils::Math::PointsAreCollinear(const TArray<FVector>& InPoints, const float InErrorTolerance)
{
    if (InPoints.Num() <= 2)
    {
        // Two points are always collinear
        return true;
    }

    const FVector LineDirection = (InPoints[1] - InPoints[0]).GetSafeNormal();

    for (int32 i = 2; i < InPoints.Num(); ++i) // skip the first two points
    {
        const FVector DirectionToPoint = (InPoints[i] - InPoints[0]).GetSafeNormal();

        const bool bSameDirection = FMath::IsNearlyEqual(FVector::DotProduct(DirectionToPoint, LineDirection), 1, InErrorTolerance);
        const bool bOppositeDirection = FMath::IsNearlyEqual(FVector::DotProduct(DirectionToPoint, LineDirection), -1, InErrorTolerance);
        const bool bParallel = (bSameDirection || bOppositeDirection);
        if (!bParallel)
        {
            // Not collinear
            return false;
        }
    }

    // All points lie on the same line
    return true;
}

bool GCUtils::Math::PointLiesOnTriangle(const FVector& InA, const FVector& InB, const FVector& InC, const FVector& InPoint, const float InErrorTolerance)
{
    // Whether the point is within A's angle
    const FVector AToPoint = (InPoint - InA);
    const FVector AToB = (InB - InA);
    const FVector AToC = (InC - InA);
    const bool bPointIsBetweenBAndC = DirectionIsBetween(AToB, AToC, true, AToPoint, InErrorTolerance);

    // Whether the point is within B's angle
    const FVector BToPoint = (InPoint - InB);
    const FVector BToA = (InA - InB);
    const FVector BToC = (InC - InB);
    const bool bPointIsBetweenAAndC = DirectionIsBetween(BToA, BToC, true, BToPoint, InErrorTolerance);

    if (bPointIsBetweenBAndC && bPointIsBetweenAAndC)
    {
        // Point is on the triangle
        //
        //          C
        //         / \
        //        /   \
        //       / o   \
        //      /       \
        //     /         \
        //    A _________ B
        //
        return true;
    }

    // Point is not on the triangle
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

FVector GCUtils::Math::GetLocationAimDirection(const UWorld* World, const FCollisionQueryParams& QueryParams, const FVector& AimPoint, const FVector& AimDir, const float& MaxRange, const FVector& Location)
{
    if (Location.Equals(AimPoint))
    {
        // The Location is the same as the AimPoint so we can skip the camera trace and just return the AimDir as the muzzle's direction
        return AimDir;
    }

    // Line trace from the Location to the point that AimDir is looking at
    FCollisionQueryParams CollisionQueryParams = QueryParams;
    CollisionQueryParams.bIgnoreTouches = true;

    FVector TraceEnd = AimPoint + (AimDir * MaxRange);

    FHitResult HitResult;
    const bool bSuccess = World->LineTraceSingleByChannel(HitResult, AimPoint, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

    if (!bSuccess)
    {
        // AimDir is not looking at anything for our Location to point to
        return (TraceEnd - Location).GetSafeNormal();
    }

    // Return the direction from the Location to the point that the Player is looking at
    return (HitResult.Location - Location).GetSafeNormal();
}
