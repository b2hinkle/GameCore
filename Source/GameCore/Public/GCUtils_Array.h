// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"
#include "GCUtils_String.h"

/**
 * @brief Array utils.
 */
namespace GCUtils::Array
{
    /**
     * @brief Removes all items at and after the given index.
     */
    template <class TElement, class TAllocator>
    void RemoveTheRestAtInclusive(TArray<TElement, TAllocator>& array, const int32 index);

    /**
     * @brief Removes all items from one index to another, inclusively.
     */
    template <class TElement, class TAllocator>
    void RemoveInRangeInclusive(TArray<TElement, TAllocator>& array, const int32 fromIndex, const int32 toIndex);
}

template <class TElement, class TAllocator>
void GCUtils::Array::RemoveTheRestAtInclusive(TArray<TElement, TAllocator>& array, const int32 index)
{
    checkf(array.IsValidIndex(index),
        TEXT("The ") GC_STRING_LITERALIZE(index) TEXT(" %d is not valid for the ") GC_STRING_LITERALIZE(array) TEXT(" of size %d."),
        index,
        array.Num());

    const int32 lastIndex = array.Num() - 1;
    RemoveInRangeInclusive(array, index, lastIndex);
}

template <class TElement, class TAllocator>
void GCUtils::Array::RemoveInRangeInclusive(TArray<TElement, TAllocator>& array, const int32 fromIndex, const int32 toIndex)
{
    checkf(array.IsValidIndex(fromIndex),
        TEXT("The ") GC_STRING_LITERALIZE(fromIndex) TEXT(" %d is not valid for the ") GC_STRING_LITERALIZE(array) TEXT(" of size %d."),
        fromIndex,
        array.Num());
    checkf(array.IsValidIndex(toIndex),
        TEXT("The ") GC_STRING_LITERALIZE(toIndex) TEXT(" %d is not valid for the ") GC_STRING_LITERALIZE(array) TEXT(" of size %d."),
        toIndex,
        array.Num());

    checkf(fromIndex <= toIndex,
        TEXT("The ") GC_STRING_LITERALIZE(fromIndex) TEXT(" %d is less than the ") GC_STRING_LITERALIZE(toIndex) TEXT(" %d."),
        fromIndex,
        toIndex);

    const int32 count = (toIndex - fromIndex) + 1;
    check(count >= 0);
    check(fromIndex + count <= array.Num());

    array.RemoveAt(fromIndex, count);
}
