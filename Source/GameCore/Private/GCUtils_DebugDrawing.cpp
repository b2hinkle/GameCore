// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_DebugDrawing.h"

#include "DrawDebugHelpers.h"

#if UE_ENABLE_DEBUG_DRAWING
void GCUtils::DebugDrawing::DrawDebugCollisionShape(
    const UWorld* inWorld,
    const FVector& inCenter,
    const FCollisionShape& inCollisionShape,
    const FQuat& inRotation,
    const FColor& inColor,
    const int32 inNumSegments,
    const bool inShouldLinesPersistent,
    const float inLifetime,
    const uint8 inDepthPriority,
    const float inThickness)
{
    switch (inCollisionShape.ShapeType)
    {
    case ECollisionShape::Line:
        {
            const float size = inThickness * 10;
            ::DrawDebugPoint(
                inWorld,
                inCenter,
                size,
                inColor,
                inShouldLinesPersistent,
                inLifetime,
                inDepthPriority);
            break;
        }
    case ECollisionShape::Box:
        {
            const FVector extent = inCollisionShape.GetExtent();
            ::DrawDebugBox(
                inWorld,
                inCenter,
                extent,
                inRotation,
                inColor,
                inShouldLinesPersistent,
                inLifetime,
                inDepthPriority,
                inThickness);
            break;
        }
    case  ECollisionShape::Sphere:
        {
            const float radius = inCollisionShape.GetSphereRadius();
            ::DrawDebugSphere(
                inWorld,
                inCenter,
                radius,
                inNumSegments,
                inColor,
                inShouldLinesPersistent,
                inLifetime,
                inDepthPriority,
                inThickness);
            break;
        }
    case ECollisionShape::Capsule:
        {
            const float halfHeight = inCollisionShape.GetCapsuleHalfHeight();
            const float radius = inCollisionShape.GetCapsuleRadius();
            // Note: `inNumSegments` isn't used anywhere here because the number of segments is hardcoded by `DrawDebugCapsule()`.
            ::DrawDebugCapsule(
                inWorld,
                inCenter,
                halfHeight,
                radius,
                inRotation,
                inColor,
                inShouldLinesPersistent,
                inLifetime,
                inDepthPriority,
                inThickness);
            break;
        }
    }
}

void GCUtils::DebugDrawing::DrawDebugLineDotted(
    const UWorld* inWorld,
    const FVector& inStart,
    const FVector& inEnd,
    const FColor& inColor,
    const bool inShouldLinesPersistent,
    const float inLifetime,
    const uint8 inDepthPriority,
    const float inThickness,
    const FVector::FReal inSegmentsLength,
    const FVector::FReal inSegmentsSpacingLength)
{
    const FVector lineDirection = (inEnd - inStart).GetSafeNormal();

    const FVector::FReal lineLength = FVector::Distance(inStart, inEnd);
    const int32 numSegments = FMath::CeilToInt(lineLength / (inSegmentsLength + inSegmentsSpacingLength));

    for (int32 i = 0; i < numSegments; ++i)
    {
        FVector::FReal distanceToLineSegmentStart = (inSegmentsLength + inSegmentsSpacingLength) * i;
        FVector::FReal distanceToLineSegmentEnd = distanceToLineSegmentStart + inSegmentsLength;

        distanceToLineSegmentEnd = FMath::Min(distanceToLineSegmentEnd, lineLength);

        // TODO: We can avoid the need for a normalized `lineDirection` by just using the `inEnd - inStart` position
        // delta as an offset and scaling it based on `i`.
        const FVector lineSegmentStart = inStart + (lineDirection * distanceToLineSegmentStart);
        const FVector lineSegmentEnd = inStart + (lineDirection * distanceToLineSegmentEnd);

        ::DrawDebugLine(inWorld, lineSegmentStart, lineSegmentEnd, inColor, inShouldLinesPersistent, inLifetime, inDepthPriority, inThickness);
    }
}
#endif // UE_ENABLE_DEBUG_DRAWING
