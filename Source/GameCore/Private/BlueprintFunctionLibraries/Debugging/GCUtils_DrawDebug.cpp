// Fill out your copyright notice in the Description page of Project Settings.

#include "BlueprintFunctionLibraries/Debugging/GCUtils_DrawDebug.h"

#include "DrawDebugHelpers.h"

void GCUtils::DrawDebug::DrawDebugCollisionShape(const UWorld* InWorld, const FVector& InCenter, const FCollisionShape& InCollisionShape, const FQuat& InRotation, const FColor& InColor, const int32 InSegments, const bool bInPersistentLines, const float InLifeTime, const uint8 InDepthPriority, const float InThickness)
{
#if ENABLE_DRAW_DEBUG
    switch (InCollisionShape.ShapeType)
    {
        case ECollisionShape::Box:
        {
            DrawDebugBox(InWorld, InCenter, InCollisionShape.GetExtent(), InRotation, InColor, bInPersistentLines, InLifeTime, InDepthPriority, InThickness);
            break;
        }
        case  ECollisionShape::Sphere:
        {
            DrawDebugSphere(InWorld, InCenter, InCollisionShape.GetSphereRadius(), InSegments, InColor, bInPersistentLines, InLifeTime, InDepthPriority, InThickness);
            break;
        }
        case ECollisionShape::Capsule:
        {
            // NOTE: Segments is not used here because DrawDebugCapsule() hard codes it
            DrawDebugCapsule(InWorld, InCenter, InCollisionShape.GetCapsuleHalfHeight(), InCollisionShape.GetCapsuleRadius(), InRotation, InColor, bInPersistentLines, InLifeTime, InDepthPriority, InThickness);
            break;
        }
        case ECollisionShape::Line:
        {
            DrawDebugPoint(InWorld, InCenter, InThickness * 10, InColor, bInPersistentLines, InLifeTime, InDepthPriority);
            break;
        }
    }
#endif // ENABLE_DRAW_DEBUG
}

void GCUtils::DrawDebug::DrawDebugLineDotted(const UWorld* InWorld, const FVector& InStart, const FVector& InEnd, const FColor& InColor, const bool bInPersistentLines, const float InLifeTime, const uint8 InDepthPriority, const float InThickness, const float InSegmentsLength, const float InSegmentsSpacingLength)
{
#if ENABLE_DRAW_DEBUG
    const FVector Direction = (InEnd - InStart).GetSafeNormal();
    const float FullLength = FVector::Distance(InStart, InEnd);
    const float NumberOfLineSegments = FMath::CeilToInt(FullLength / (InSegmentsLength + InSegmentsSpacingLength));
    for (int32 i = 0; i < NumberOfLineSegments; ++i)
    {
        float DistanceToLineSegmentStart = (InSegmentsLength + InSegmentsSpacingLength) * i;
        float DistanceToLineSegmentEnd = DistanceToLineSegmentStart + InSegmentsLength;

        if (DistanceToLineSegmentEnd > FullLength)
        {
            DistanceToLineSegmentEnd = FullLength;
        }

        const FVector LineSegmentStart = InStart + (Direction * DistanceToLineSegmentStart);
        const FVector LineSegmentEnd = InStart + (Direction * DistanceToLineSegmentEnd);

        DrawDebugLine(InWorld, LineSegmentStart, LineSegmentEnd, InColor, bInPersistentLines, InLifeTime, InDepthPriority, InThickness);
    }
#endif // ENABLE_DRAW_DEBUG
}
