// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Map.h"

/**
 * @brief Map utils.
 */
namespace GCUtils::Map
{
    /**
     * @brief
     */
    template <class TKey, class TValue, class TSetAllocator, class TKeyFuncs>
    const TPair<TKey, TValue>* FindByPredicate(
        const TMapBase<TKey, TValue, TSetAllocator, TKeyFuncs>& inMap,
        const TFunctionRef<bool(const TPair<TKey, TValue>&)>& inPredicate);
}

template <class TKey, class TValue, class TSetAllocator, class TKeyFuncs>
const TPair<TKey, TValue>* GCUtils::Map::FindByPredicate(
    const TMapBase<TKey, TValue, TSetAllocator, TKeyFuncs>& inMap,
    const TFunctionRef<bool(const TPair<TKey, TValue>&)>& inPredicate)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Map::FindByPredicate);

    for (const TPair<TKey, TValue>& pair : inMap)
    {
        if (inPredicate(pair))
        {
            return &pair;
        }
    }

    return nullptr;
}
