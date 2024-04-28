// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 *
 */
namespace GCUtils::GameplayTag
{
    /**
     * @brief Combine two gameplay tag queries by an "ANY" expression.
     * @return The combined query.
     */
    GAMECORE_API FGameplayTagQuery CombineGameplayTagQueriesAnyExprMatch(
        const FGameplayTagQuery& queryA,
        const FGameplayTagQuery& queryB,
        FString newQueryUserDescription = FString());

    /**
     * @brief Combine two gameplay tag queries by an "ALL" expression.
     * @return The combined query.
     */
    GAMECORE_API FGameplayTagQuery CombineGameplayTagQueriesAllExprMatch(
        const FGameplayTagQuery& queryA,
        const FGameplayTagQuery& queryB,
        FString newQueryUserDescription = FString());

    /**
     * @brief Combine two gameplay tag queries by a "NONE" expression.
     * @return The combined query.
     */
    GAMECORE_API FGameplayTagQuery CombineGameplayTagQueriesNoExprMatch(
        const FGameplayTagQuery& queryA,
        const FGameplayTagQuery& queryB,
        FString newQueryUserDescription = FString());
}
