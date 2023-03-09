// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintFunctionLibraries/CollisionQuery/GCBlueprintFunctionLibrary_CollisionQueries.h"

#include "GCBlueprintFunctionLibrary_StrengthCollisionQueries.generated.h"



/**
 * Aditional hit info required for strength queries
 */
USTRUCT()
struct GAMECORE_API FStrengthHitResult : public FExitAwareHitResult
{
	GENERATED_BODY()

	FStrengthHitResult()
		: FExitAwareHitResult()
		, TraveledDistanceBeforeThisTrace(0.f)
		, RicochetNumber(0)
		, bIsRicochet(false)
		, Strength(0.f)
	{
	}
	FStrengthHitResult(const FExitAwareHitResult& HitResult)
		: FExitAwareHitResult(HitResult)
		, TraveledDistanceBeforeThisTrace(0.f)
		, RicochetNumber(0)
		, bIsRicochet(false)
		, Strength(0.f)
	{
	}

	/** Represents the distance the query traveled before getting to this specific trace */
	float TraveledDistanceBeforeThisTrace;
	/** How many times the query ricocheted before this hit */
	int32 RicochetNumber;
	/** We hit a ricochetable surface */
	uint8 bIsRicochet : 1;
	/** Current strength at the location of hit */
	float Strength;

	/** Gets the total distance the query traveled until this hit's location */
	float GetTotalDistanceTraveledToThisHit() const
	{
		return TraveledDistanceBeforeThisTrace + Distance;
	}
};

/**
 * Describes a scene cast that uses strength. Since we have multible strength-related queries, this data is needed by each of them.
 */
USTRUCT()
struct GAMECORE_API FStrengthSceneCastInfo
{
	GENERATED_BODY()

	FStrengthSceneCastInfo()
		: CollisionShapeCasted(FCollisionShape())
		, CollisionShapeCastedRotation(FQuat::Identity)
		, CastDirection(FVector::ZeroVector)
		, StartLocation(FVector::ZeroVector)
		, StartStrength(0.f)
		, StopLocation(FVector::ZeroVector)
		, StopStrength(0.f)
		, DistanceToStop(0.f)
		, TimeAtStop(0.f)
	{
	}

	/** The shape we used for the query (line trace if ECollisionShape::Line) */
	FCollisionShape CollisionShapeCasted;
	/** Casted shape's rotation */
	FQuat CollisionShapeCastedRotation;
	/** The direction casted in */
	FVector CastDirection;
	/** The location where this scene cast began */
	FVector StartLocation;
	/** The strength this scene cast started with */
	float StartStrength;
	/** The location where we were stopped */
	FVector StopLocation;
	/** The strength when we stopped */
	float StopStrength;
	/** Distance from StartLocation to StopLocation */
	float DistanceToStop;
	/** The time where we were stopped */
	float TimeAtStop;
};

/**
 * Struct describing a PenetrationSceneCastWithExitHitsUsingStrength()
 */
USTRUCT()
struct GAMECORE_API FPenetrationSceneCastWithExitHitsUsingStrengthResult
{
	GENERATED_BODY()

	FPenetrationSceneCastWithExitHitsUsingStrengthResult()
		: StrengthSceneCastInfo(FStrengthSceneCastInfo())
		, HitResults(TArray<FStrengthHitResult>())
	{
	}

	/** Info about the scene cast that uses strength */
	FStrengthSceneCastInfo StrengthSceneCastInfo;
	/** Hit results in this scene cast */
	TArray<FStrengthHitResult> HitResults;
};
/**
 * Struct describing a RicochetingPenetrationSceneCastWithExitHitsUsingStrength()
 */
USTRUCT()
struct GAMECORE_API FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult
{
	GENERATED_BODY()

	FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult()
		: StrengthSceneCastInfo(FStrengthSceneCastInfo())
		, PenetrationSceneCastWithExitHitsUsingStrengthResults(TArray<FPenetrationSceneCastWithExitHitsUsingStrengthResult>())
	{
	}

	/** Info about the scene cast that uses strength */
	FStrengthSceneCastInfo StrengthSceneCastInfo;
	/** Penetration with strength scene casts. The initial and ricochets. */
	TArray<FPenetrationSceneCastWithExitHitsUsingStrengthResult> PenetrationSceneCastWithExitHitsUsingStrengthResults;
};

/**
 *	A collection of specialized scene casts that rely on strength to keep it traveling. These are given an initial strength and lose strength from provided strength nerfs. The scene cast is stopped the moment its strength runs out.
 *
 *	Penetrations - Penetrations nerf the strength via the per cm nerf stack - adding to it on entrance and removing from it on exit.
 *	Ricochets - Ricochets nerf the strength in a one-off fashion.
 */
UCLASS()
class GAMECORE_API UGCBlueprintFunctionLibrary_StrengthCollisionQueries : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static const TFunctionRef<float(const FHitResult&)>& DefaultGetPerCmPenetrationNerf;
	static const TFunctionRef<float(const FHitResult&)>& DefaultGetRicochetNerf;
	static const TFunctionRef<bool(const FHitResult&)>& DefaultIsHitRicochetable;


	//  BEGIN Custom query
	/**
	 * Given an initial strength, perform a scene cast, applying strength nerfs to the query as it penetrates through blocking hits.
	 *
	 * @param  InInitialStrength              Initial strength of the scene cast.
	 * @param  InOutPerCmNerfStack            Stack of values that nerf the query's strength per cm. Top of stack represents the most recent nerf (in penetration terminology, the most recent/inner object currently being penetrated).
	 * @param  InWorld                        The world to scene cast in
	 * @param  OutResult                      Struct that fully describes this query
	 * @param  InStart                        Start location of the scene cast
	 * @param  InEnd                          End location of the scene cast
	 * @param  InRotation                     Rotation of the collision shape (needed for sweeps)
	 * @param  InTraceChannel                 The trace channel for this scene cast
	 * @param  InCollisionShape               Generic collision shape for sweeps/traces (FCollisionShape::LineShape for a line trace)
	 * @param  InCollisionQueryParams         Additional parameters used for the scene cast
	 * @param  GetPerCmPenetrationNerf        TFunction where caller indicates a per cm strength nerf to apply when entering geometry given a hit
	 * @param  IsHitImpenetrable              TFunction where caller indicates whether provided HitResult should stop us
	 * @return The impenetrable hit if we hit one
	 */
	static FStrengthHitResult* PenetrationSceneCastWithExitHitsUsingStrength(const float InInitialStrength, TArray<float>& InOutPerCmNerfStack, const UWorld* InWorld, FPenetrationSceneCastWithExitHitsUsingStrengthResult& OutResult, const FVector& InStart, const FVector& InEnd, const FQuat& InRotation, const ECollisionChannel InTraceChannel, const FCollisionShape& InCollisionShape, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam,
		const TFunctionRef<float(const FHitResult&)>& GetPerCmPenetrationNerf = DefaultGetPerCmPenetrationNerf,
		const TFunctionRef<bool(const FHitResult&)>& IsHitImpenetrable = UGCBlueprintFunctionLibrary_CollisionQueries::DefaultIsHitImpenetrable);
	static FStrengthHitResult* PenetrationSceneCastWithExitHitsUsingStrength(const float InInitialStrength, const float InRangeFalloffNerf, const UWorld* InWorld, FPenetrationSceneCastWithExitHitsUsingStrengthResult& OutResult, const FVector& InStart, const FVector& InEnd, const FQuat& InRotation, const ECollisionChannel InTraceChannel, const FCollisionShape& InCollisionShape, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam,
		const TFunctionRef<float(const FHitResult&)>& GetPerCmPenetrationNerf = DefaultGetPerCmPenetrationNerf,
		const TFunctionRef<bool(const FHitResult&)>& IsHitImpenetrable = UGCBlueprintFunctionLibrary_CollisionQueries::DefaultIsHitImpenetrable);
	//  END Custom query


	//  BEGIN Custom query
	/**
	 * Penetrating scene cast that can ricochet, using PenetrationSceneCastWithExitHitsUsingStrength() for each cast.
	 *
	 * @param  OutResult                  Struct that fully describes this query
	 * @param  InStart                    Start location of the scene cast
	 * @param  InDirection                The direction to scene cast
	 * @param  InDistanceCap              The max distance to travel (performance wise, length of the cast will be this large and get smaller as we travel from ricochets)
	 * @param  GetRicochetNerf            TFunction where caller indicates strength nerf to apply when hitting a ricochetable hit
	 * @param  IsHitRicochetable          TFunction where caller indicates whether we should ricochet off of the HitResult
	 */
	static void RicochetingPenetrationSceneCastWithExitHitsUsingStrength(const float InInitialStrength, TArray<float>& InOutPerCmNerfStack, const UWorld* InWorld, FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult& OutResult, const FVector& InStart, const FVector& InDirection, const float InDistanceCap, const FQuat& InRotation, const ECollisionChannel InTraceChannel, const FCollisionShape& InCollisionShape, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam, const int32 InRicochetCap = -1,
		const TFunctionRef<float(const FHitResult&)>& GetPerCmPenetrationNerf = DefaultGetPerCmPenetrationNerf,
		const TFunctionRef<float(const FHitResult&)>& GetRicochetNerf = DefaultGetRicochetNerf,
		const TFunctionRef<bool(const FHitResult&)>& IsHitRicochetable = DefaultIsHitRicochetable);
	static void RicochetingPenetrationSceneCastWithExitHitsUsingStrength(const float InInitialStrength, const float InRangeFalloffNerf, const UWorld* InWorld, FRicochetingPenetrationSceneCastWithExitHitsUsingStrengthResult& OutResult, const FVector& InStart, const FVector& InDirection, const float InDistanceCap, const FQuat& InRotation, const ECollisionChannel InTraceChannel, const FCollisionShape& InCollisionShape, const FCollisionQueryParams& InCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& InCollisionResponseParams = FCollisionResponseParams::DefaultResponseParam, const int32 InRicochetCap = -1,
		const TFunctionRef<float(const FHitResult&)>& GetPerCmPenetrationNerf = DefaultGetPerCmPenetrationNerf,
		const TFunctionRef<float(const FHitResult&)>& GetRicochetNerf = DefaultGetRicochetNerf,
		const TFunctionRef<bool(const FHitResult&)>& IsHitRicochetable = DefaultIsHitRicochetable);
	//  END Custom query


private:
	static float NerfStrengthPerCm(float& InOutStrength, const float InDistanceToTravel, const float InNerfPerCm);
};
