// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GCUtils_Array.generated.h"



/**
 * A collection of helpful functions related to TArrays
 */
UCLASS()
class GAMECORE_API UGCBlueprintFunctionLibrary_ArrayHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Removes all items at and after the given index */
    template <class T>
    static void RemoveTheRestAt(TArray<T>& InOutArray, const int32& Index)
    {
        if (InOutArray.IsValidIndex(Index) == false)
        {
            UE_LOG(LogGCArrayHelpers, Error, TEXT("%s() was given an invalid index [%d]"), ANSI_TO_TCHAR(__FUNCTION__), Index);
            check(0);
        }

        const int32 LastIndex = InOutArray.Num() - 1;
        RemoveInRange(InOutArray, Index, LastIndex);
    }

    /** Removes all items from one index to another */
    template <class T>
    static void RemoveInRange(TArray<T>& InOutArray, const int32& FromIndex, const int32& ToIndex)
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

};
