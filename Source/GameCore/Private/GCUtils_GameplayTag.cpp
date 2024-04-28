// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_GameplayTag.h"

namespace GCUtils::GameplayTag
{
    template
        <
        EGameplayTagQueryExprType QueryExpressionType,
        class = typename TEnableIf
            <
            QueryExpressionType == EGameplayTagQueryExprType::AnyExprMatch
            || QueryExpressionType == EGameplayTagQueryExprType::AllExprMatch
            || QueryExpressionType == EGameplayTagQueryExprType::NoExprMatch
            >::Type
        >
    static FGameplayTagQuery CombineGameplayTagQueries(
        const FGameplayTagQuery& queryA,
        const FGameplayTagQuery& queryB,
        FString newQueryUserDescription);
}

FGameplayTagQuery GCUtils::GameplayTag::CombineGameplayTagQueriesAnyExprMatch(
    const FGameplayTagQuery& queryA,
    const FGameplayTagQuery& queryB,
    FString newQueryUserDescription)
{
    return CombineGameplayTagQueries<EGameplayTagQueryExprType::AnyExprMatch>(
        queryA,
        queryB,
        MoveTemp(newQueryUserDescription));
}

FGameplayTagQuery GCUtils::GameplayTag::CombineGameplayTagQueriesAllExprMatch(
    const FGameplayTagQuery& queryA,
    const FGameplayTagQuery& queryB,
    FString newQueryUserDescription)
{
    return CombineGameplayTagQueries<EGameplayTagQueryExprType::AllExprMatch>(
        queryA,
        queryB,
        MoveTemp(newQueryUserDescription));
}

FGameplayTagQuery GCUtils::GameplayTag::CombineGameplayTagQueriesNoExprMatch(
    const FGameplayTagQuery& queryA,
    const FGameplayTagQuery& queryB,
    FString newQueryUserDescription)
{
    return CombineGameplayTagQueries<EGameplayTagQueryExprType::NoExprMatch>(
        queryA,
        queryB,
        MoveTemp(newQueryUserDescription));
}

template <EGameplayTagQueryExprType QueryExpressionType, class>
FGameplayTagQuery GCUtils::GameplayTag::CombineGameplayTagQueries(
    const FGameplayTagQuery& queryA,
    const FGameplayTagQuery& queryB,
    FString newQueryUserDescription)
{
    // If one query is empty, return the other. It's important to cover these cases
    // because doing GetQueryExpr with an empty gameplay tag query would give invalid results.

    if (queryA.IsEmpty())
    {
        return queryB;
    }

    if (queryB.IsEmpty())
    {
        return queryA;
    }

    // Combine the queries into a query expression.

    FGameplayTagQueryExpression queryExpressionA;
    queryA.GetQueryExpr(queryExpressionA);

    FGameplayTagQueryExpression queryExpressionB;
    queryB.GetQueryExpr(queryExpressionB);

    FGameplayTagQueryExpression queryExpressionCombined;
    queryExpressionCombined.ExprType = QueryExpressionType;
    queryExpressionCombined.AddExpr(queryExpressionA);
    queryExpressionCombined.AddExpr(queryExpressionB);

    // Build query from expression.

    return FGameplayTagQuery::BuildQuery(queryExpressionCombined,
        MoveTemp(newQueryUserDescription));
}
