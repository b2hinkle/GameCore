// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 *
 */
namespace GCUtils::GameplayTag
{

    typedef TFunctionRef<void(const FGameplayTag&)> FTagTreeTraversalCallbackRef;

    /**
     * @brief Combine two gameplay tag queries by an "ANY" expression.
     * @return The combined query.
     */
    GAMECORE_API FGameplayTagQuery CombineGameplayTagQueriesAnyExprMatch(
        const FGameplayTagQuery& queryA,
        const FGameplayTagQuery& queryB,
        FString&& newQueryUserDescription = FString());

    /**
     * @brief Combine two gameplay tag queries by an "ALL" expression.
     * @return The combined query.
     * 
     */
    GAMECORE_API FGameplayTagQuery CombineGameplayTagQueriesAllExprMatch(
        const FGameplayTagQuery& queryA,
        const FGameplayTagQuery& queryB,
        FString&& newQueryUserDescription = FString());

    /**
     * @brief Combine two gameplay tag queries by a "NONE" expression.
     * @return The combined query.
     */
    GAMECORE_API FGameplayTagQuery CombineGameplayTagQueriesNoExprMatch(
        const FGameplayTagQuery& queryA,
        const FGameplayTagQuery& queryB,
        FString&& newQueryUserDescription = FString());

    /**
     * @brief A gameplay tag paired to a tag container is essentially a node in a tree. A group of
     *        these pairs makes up the tree. This function will fire the provided visit callback
              in a post-order traversal.
     * @param currentNode The current node being processed during the traversal. Non-recursive callsites
     *                    will want to pass in the starting point for the traversal.
     * @param tagTree The tree to be traversed. Be mindful of your tree to ensure there isn't a circular
     *                connection, which would cause a stack overflow.
     * @param visitCallback The logic to execute on a node visit.
     */
    GAMECORE_API void PerformTagTreeTraversalPostorder(
        const TMap<FGameplayTag, FGameplayTagContainer>& tagTree,
        const FGameplayTag& currentNode,
        const FTagTreeTraversalCallbackRef& visitCallback);
}
