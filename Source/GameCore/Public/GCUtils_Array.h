// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"

/**
 * A collection of helpful functions related to TArrays
 */
namespace GCUtils::Array
{
    /** Removes all items at and after the given index */
    template <class T>
    void RemoveTheRestAt(TArray<T>& InOutArray, const int32& Index);

    /** Removes all items from one index to another */
    template <class T>
    void RemoveInRange(TArray<T>& InOutArray, const int32& FromIndex, const int32& ToIndex);
}

template <class T>
void GCUtils::Array::RemoveTheRestAt(TArray<T>& InOutArray, const int32& Index)
{
    if (InOutArray.IsValidIndex(Index) == false)
    {
        UE_LOG(LogGCArrayHelpers, Error, TEXT("%s() was given an invalid index [%d]"), ANSI_TO_TCHAR(__FUNCTION__), Index);
        check(0);
    }

    const int32 LastIndex = InOutArray.Num() - 1;
    RemoveInRange(InOutArray, Index, LastIndex);
}

template <class T>
void GCUtils::Array::RemoveInRange(TArray<T>& InOutArray, const int32& FromIndex, const int32& ToIndex)
{
    if (InOutArray.IsValidIndex(FromIndex) == false)
    {
        UE_LOG(LogGCArrayHelpers, Error, TEXT("%s() was given an invalid FromIndex [%d]"), ANSI_TO_TCHAR(__FUNCTION__), FromIndex);
        check(0);
    }
    if (InOutArray.IsValidIndex(ToIndex) == false)
    {
        UE_LOG(LogGCArrayHelpers, Error, TEXT("%s() was given an invalid ToIndex [%d]"), ANSI_TO_TCHAR(__FUNCTION__), ToIndex);
        check(0);
    }

    const int AmountToRemove = ToIndex - (FromIndex - 1);
    InOutArray.RemoveAt(FromIndex, AmountToRemove);
}
