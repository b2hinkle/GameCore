// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GCBlueprintFunctionLibrary_CollisionQueries.generated.h"

/**
 * Extension of FHitResult for indicating whether it's an entrance or an exit
 */
USTRUCT()
struct GAMECORE_API FExitAwareHitResult : public FHitResult
{
	GENERATED_BODY()

	FExitAwareHitResult()
		: FHitResult()
		, bIsExitHit(false)
	{
	}
	FExitAwareHitResult(const FHitResult& HitResult)
		: FHitResult(HitResult)
		, bIsExitHit(false)
	{
	}

	uint8 bIsExitHit : 1;
};

/**
 *	- Exit hit scene casting -
 *	Exit hits are useful in cases where you need the other side of the geometry that was hit. We achieve this by performing a second scene cast in the opposite direction. This can get expensive as your query length is effectively doubling. To mitigate this, a built-in optimization is provided to minimize the backward query length down to its shortest guaranteed working length.
 *
 *	- Penetration scene casting -
 *	Penetration through blocking hits is useful for when you are querying with a trace channel but do not want to be stopped by the trace channel's blocking hits. This is a good way to observe what types of hit responses are within your query's area.
 *	Given a situation where you need a scene cast that goes through walls (e.g. a bullet), you may make a trace channel that overlaps walls. However, if you want to keep the distinction between physical walls and trigger boxes, for example, getting overlap responses for both won't help. Instead, you want an overlap response for the trigger box and a blocking response for the wall. This penetration feature makes this distinction possible while penetrating through the blocking hits.
 */
UCLASS()
class GAMECORE_API UGCBlueprintFunctionLibrary_CollisionQueries : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static const float SceneCastStartWallAvoidancePadding;
	static const TFunctionRef<bool(const FHitResult&)>& DefaultIsHitImpenetrable;

	/**
	 * FCollisionShape scene cast.
	 * This keeps the scene cast generic to sweeps and linetraces, allowing our custom queries to support both sweeps and linetraces without duplicate code.
	 */
	static bool SceneCastMultiByChannel(const UWorld* InWorld, TArray<FHitResult>& OutHits, const FVector& InStart, const FVector& InEnd, const FQuat& InRotation, const ECollisionChannel InTraceChannel, const FCollisionShape& InCollisionShape, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam);


	//  BEGIN Custom query
	/**
	 * Scene cast with penetrations that outputs entrance and exit hits in order of the forwards direction
	 * 
	 * @param  OutHits                              Array of entrance and exit hits (overlap and blocking) that were found until IsHitImpenetrable condition is met
	 * @param  bOptimizeBackwardsSceneCastLength    Only recommend using this if you're not starting the scene cast inside of geometry, otherwise the exits of any gemometry you're starting inside of may not be found. However, you still could possibly get away with it if you are doing a very lengthy scene cast, because you are more likely to hit an entrance past the exit of the geometry that you started in. If true, will minimize the backwards scene cast length to start no further than the exit of the furthest entrance.
	 * @return TRUE if hit and stopped at a blocking hit.
	 */
	static bool SceneCastMultiWithExitHits(const UWorld* InWorld, TArray<FExitAwareHitResult>& OutHits, const FVector& InStart, const FVector& InEnd, const FQuat& InRotation, const ECollisionChannel InTraceChannel, const FCollisionShape& InCollisionShape, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam, const bool bOptimizeBackwardsSceneCastLength = false, const bool bDrawDebugForBackwardsStart = false);
	static bool LineTraceMultiWithExitHits(const UWorld* InWorld, TArray<FExitAwareHitResult>& OutHits, const FVector& InTraceStart, const FVector& InTraceEnd, const ECollisionChannel InTraceChannel, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam, const bool bOptimizeBackwardsSceneCastLength = false, const bool bDrawDebugForBackwardsStart = false);
	static bool SweepMultiWithExitHits(const UWorld* InWorld, TArray<FExitAwareHitResult>& OutHits, const FVector& InSweepStart, const FVector& InSweepEnd, const FQuat& InRotation, const ECollisionChannel InTraceChannel, const FCollisionShape& InCollisionShape, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam, const bool bOptimizeBackwardsSceneCastLength = false, const bool bDrawDebugForBackwardsStart = false);
	//  END Custom query



	//  BEGIN Custom query
	/**
	 *  Scene cast that penetrates everything except for what the caller says in IsHitImpenetrable() TFunction
	 *  
	 *  The engine's way of handling collisions is good, but is limiting in some situations. We want to run a scene cast where
	 *  blocking hits don't actually block it, creating a sort of "penetration" feature. The reason we can't just use
	 *  a collision trace channel who's default response is overlap is because doing so removes the distinction between overlaps
	 *  and blocking hits (e.g. no way to know what is a triggerbox and what is a physical wall).
	 *  This is effectively a linetrace/sweep multi that is not stopped by blocking hits and instead gets stopped by whatever condition the
	 *  caller provides while still preserving the hits' collision responses.
	 *  
	 *  @param  InWorld                      The world to scene cast in
	 *  @param  OutHits                      Array of hits (overlap and blocking) that were found until IsHitImpenetrable condition is met
	 *  @param  InStart                      Start location of the scene cast
	 *  @param  InEnd                        End location of the scene cast
	 *  @param  InRotation                   Rotation of the collision shape (needed for sweeps)
	 *  @param  InTraceChannel               The trace channel for this scene cast. Unlike the default UWorld sweeps/traces, blocking hits for this channel don't stop us, but instead are used only for preserving the CollisionResponse of each hit.
	 *  @param  InCollisionShape             Generic collision shape for sweeps/traces (FCollisionShape::LineShape for a line trace)
	 *  @param  InCollisionQueryParams       Additional parameters used for the scene cast
	 *  @param  InCollisionResponseParams    List of this scene cast's responses to certain collision channels
	 *  @param  IsHitImpenetrable            TFunction where caller indicates whether provided HitResult should stop us. Since we penetrate blocking hits, caller might want to define when to stop.
	 *  @return The impenetrable hit if we hit one
	 */
	static FHitResult* PenetrationSceneCast(const UWorld* InWorld, TArray<FHitResult>& OutHits, const FVector& InStart, const FVector& InEnd, const FQuat& InRotation, const ECollisionChannel InTraceChannel, const FCollisionShape& InCollisionShape, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam,
		const TFunctionRef<bool(const FHitResult&)>& IsHitImpenetrable = DefaultIsHitImpenetrable);
	static FHitResult* PenetrationLineTrace(const UWorld* InWorld, TArray<FHitResult>& OutHits, const FVector& InTraceStart, const FVector& InTraceEnd, const ECollisionChannel InTraceChannel, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam,
		const TFunctionRef<bool(const FHitResult&)>& IsHitImpenetrable = DefaultIsHitImpenetrable);
	static FHitResult* PenetrationSweep(const UWorld* InWorld, TArray<FHitResult>& OutHits, const FVector& InSweepStart, const FVector& InSweepEnd, const FQuat& InRotation, const ECollisionChannel InTraceChannel, const FCollisionShape& InCollisionShape, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam,
		const TFunctionRef<bool(const FHitResult&)>& IsHitImpenetrable = DefaultIsHitImpenetrable);
	//  END Custom query



	//  BEGIN Custom query
	/**
	 * Scene cast that also gives us the exit hits using SceneCastMultiWithExitHits() while also providing penetrating functionality
	 * 
	 * @param  IsHitImpenetrable         TFunction where caller indicates whether provided HitResult should stop us. Since we penetrate blocking hits, caller might want to define when to stop.
	 * @return The impenetrable hit if we hit one (will always be an entrance hit)
	 */
	static FExitAwareHitResult* PenetrationSceneCastWithExitHits(const UWorld* InWorld, TArray<FExitAwareHitResult>& OutHits, const FVector& InStart, const FVector& InEnd, const FQuat& InRotation, const ECollisionChannel InTraceChannel, const FCollisionShape& InCollisionShape, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam,
		const TFunctionRef<bool(const FHitResult&)>& IsHitImpenetrable = DefaultIsHitImpenetrable,
		const bool bOptimizeBackwardsSceneCastLength = false,
		const bool bDrawDebugForBackwardsStart = false);
	static FExitAwareHitResult* PenetrationLineTraceWithExitHits(const UWorld* InWorld, TArray<FExitAwareHitResult>& OutHits, const FVector& InTraceStart, const FVector& InTraceEnd, const ECollisionChannel InTraceChannel, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam,
		const TFunctionRef<bool(const FHitResult&)>& IsHitImpenetrable = DefaultIsHitImpenetrable,
		const bool bOptimizeBackwardsSceneCastLength = false,
		const bool bDrawDebugForBackwardsStart = false);
	static FExitAwareHitResult* PenetrationSweepWithExitHits(const UWorld* InWorld, TArray<FExitAwareHitResult>& OutHits, const FVector& InSweepStart, const FVector& InSweepEnd, const FQuat& InRotation, const ECollisionChannel InTraceChannel, const FCollisionShape& InCollisionShape, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam,
		const TFunctionRef<bool(const FHitResult&)>& IsHitImpenetrable = DefaultIsHitImpenetrable,
		const bool bOptimizeBackwardsSceneCastLength = false,
		const bool bDrawDebugForBackwardsStart = false);
	//  END Custom query



	/**
	 * Determine the resulting response for a query hitting a body instance.
	 * 
	 * Takes in a body instance to test against (e.g. a Primitive Component's).
	 * Takes in a query's trace channel and response params to use (e.g. the visibility trace channel).
	 * Returns the resulting response of the query hitting the body instance.
	 */
	static ECollisionResponse GetCollisionResponseForQueryOnBodyInstance(const FBodyInstance& InBodyInstance, const ECollisionChannel InQueryCollisionChannel, const FCollisionResponseParams& InQueryCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam);

private:
	/**
	 * Modifies existing HitResults to respond appropriately to the caller's ECollisionChannel, FCollisionQueryParams, and FCollisionResponseParams.
	 * Outputs modified hits and potentially removes some.
	 * 
	 * @param  InOutHits                    Hits to modify
	 * @param  InTraceChannel               The collision channel in which the hits will conform to (e.g. setting FHitResult::bBlockingHit to true because of the hit component's response to our trace channel)
	 * @param  InCollisionQueryParams       The collision query params in which the hits will conform to (e.g. removing blocking hits because of FCollisionQueryParams::bIgnoreBlocks)
	 * @param  InCollisionResponseParams    The trace's response params
	 */
	static void ChangeHitsResponseData(TArray<FHitResult>& InOutHits, const ECollisionChannel InTraceChannel, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam);


	/** Returns the start point of our backwards scene cast based on information from the forwards cast */
	static FVector DetermineBackwardsSceneCastStart(const TArray<FHitResult>& InForwardsHitResults, const FVector& InForwardsStart, const FVector& InForwardsEnd, const FHitResult* InHitStoppedAt, const bool bOptimizeBackwardsSceneCastLength, const float InSweepShapeBoundingSphereRadius = 0.f);

	/** Modify data of backwards scene cast to be relevant to the forwards scene cast */
	static void MakeBackwardsHitsDataRelativeToForwadsSceneCast(TArray<FHitResult>& InOutBackwardsHitResults, const TArray<FHitResult>& InForwardsHitResults);

	/** Given entrance and exit hit results, output a combined array of them in order */
	static void OrderHitResultsInForwardsDirection(TArray<FExitAwareHitResult>& OutOrderedHitResults, const TArray<FHitResult>& InEntranceHitResults, const TArray<FHitResult>& InExitHitResults, const FVector& InForwardsDirection);



	/** Backwards scene cast start location visualization */
	static void DrawDebugForBackwardsStart(const UWorld* InWorld, const FCollisionShape& InCollisionShape, const FQuat& InRotation, const FVector& InBackwardsStart, const FVector& InBackwardsDir);
};
