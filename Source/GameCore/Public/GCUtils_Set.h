// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Set.h"

/**
 * @brief Set utils.
 */
namespace GCUtils::Set
{
    /**
     * @brief
     */
    template <class TElement, class TKeyFuncs, class TAllocator>
    const TElement* FindByPredicate(
        const TSet<TElement, TKeyFuncs, TAllocator>& inSet,
        const TFunctionRef<bool(const TElement&)>& inPredicate);
}

template <class TElement, class TKeyFuncs, class TAllocator>
const TElement* GCUtils::Set::FindByPredicate(
    const TSet<TElement, TKeyFuncs, TAllocator>& inSet,
    const TFunctionRef<bool(const TElement&)>& inPredicate)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Set::FindByPredicate);

    for (const TElement& element : inSet)
    {
        if (inPredicate(element))
        {
            return &element;
        }
    }

    return nullptr;
}
