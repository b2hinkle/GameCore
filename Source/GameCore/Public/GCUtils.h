// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/GCIsUObjectOrIInterface.h"
#include "Templates/GCRemovePtrOrRef.h"

/**
 * Makes a cstring literal out of the given text.
 */
#define GC_CSTRINGIZE(text) TEXT(PREPROCESSOR_TO_STRING(text))

/**
 * Macro version of GCUtils::CStringNull. Nice if you need a string literal instead of
 * a variable. E.g., for combining string literals together.
 */
#define GC_CSTRING_NULL TEXT("NULL")

/**
 * Returns the string representation of the specified ENetMode value.
 *
 * @param inNetMode The string representation of ENetMode.
 * @return A CString representation of the net mode.
 */
GAMECORE_API const TCHAR* LexToString(ENetMode inNetMode);

/**
 * Returns the string representation of the specified ENetRole value.
 *
 * @param inNetRole The string representation of ENetRole.
 * @return A CString representation of the net role.
 */
GAMECORE_API const TCHAR* LexToString(ENetRole inNetRole);

namespace GCUtils
{
    constexpr const TCHAR* CStringNull = GC_CSTRING_NULL;

    GAMECORE_API FString GetUObjectNameSafe(const UObject* inUObject);

    GAMECORE_API FString GetUObjectFullNameSafe(const UObject* inUObject, const UObject* inStopOuter = nullptr, EObjectFullNameFlags inFlags = EObjectFullNameFlags::None);

    GAMECORE_API const TCHAR* BoolToCString(const bool inBool);

    GAMECORE_API const TCHAR* GetWorldNetModeCString(const UObject* inWorldContextObject);

    GAMECORE_API const TCHAR* GetObjectLocalRoleCString(const UObject* inContextObject);

    GAMECORE_API UWorld* GetWorldSafe(const UObject* inContextObject);

    /**
     * Get the "owning" controller for an object.
     */
    GAMECORE_API const AController* GetController(const UObject* inObject);
    GAMECORE_API AController* GetController(UObject* inObject);

    /**
     * If inObject is already guaranteed to be a TTo.
     *
     * E.g., you load an asset by class and you are certain the result is a TTo.
     *
     * E.g., you are up-casting.
     */
    template
        <
        class TTo, class TFrom,
        class = typename TEnableIf
            <
            TGCIsUObjectOrIInterface<typename TGCRemovePtrOrRef<TTo>::Type>::Value
            >::Type,
        class = typename TEnableIf
            <
            //
            // Note: We remove reference from TFrom first things first to handle case where forwarding reference parameter makes TFrom an lvalue reference.
            //
            // E.g., lvalue UObject* argument -> UObject*& parameter.
            //
            TGCIsUObjectOrIInterface<typename TRemovePointer<typename TRemoveReference<TFrom>::Type>::Type>::Value
            >::Type
        >
    TTo StaticCastChecked(TFrom&& inObject);

    /**
     * If static casting isn't an option but inObject is still guaranteed to be a TTo.
     *
     * Necessary for casting to interface classes that aren't part of TFrom's inheritance chain.
     */
    template
        <
        class TTo, class TFrom,
        class = typename TEnableIf
            <
            TGCIsUObjectOrIInterface<typename TGCRemovePtrOrRef<TTo>::Type>::Value
            >::Type,
        class = typename TEnableIf
            <
            // See StaticCastChecked() comment.
            TGCIsUObjectOrIInterface<typename TRemovePointer<typename TRemoveReference<TFrom>::Type>::Type>::Value
            >::Type
        >
    TTo ReinterpretCastChecked(TFrom&& inObject);
}

template
    <
    class TTo, class TFrom,
    class,
    class
    >
TTo GCUtils::StaticCastChecked(TFrom&& inObject)
{
#if DO_CHECK
    if constexpr (TIsPointer<TTo>::Value)
    {
        if (inObject)
        {
            check(Cast<typename TRemovePointer<TTo>::Type>(inObject));
        }
    }
    else if constexpr (TIsReferenceType<TTo>::Value)
    {
        check(Cast<typename TRemoveReference<TTo>::Type>(&inObject));
    }
#endif // DO_CHECK

    return static_cast<TTo>(Forward<TFrom>(inObject));
}

template
    <
    class TTo, class TFrom,
    class,
    class
    >
TTo GCUtils::ReinterpretCastChecked(TFrom&& inObject)
{
#if DO_CHECK
    if constexpr (TIsPointer<TTo>::Value)
    {
        if (inObject)
        {
            check(Cast<typename TRemovePointer<TTo>::Type>(inObject));
        }
    }
    else if constexpr (TIsReferenceType<TTo>::Value)
    {
        check(Cast<typename TRemoveReference<TTo>::Type>(&inObject));
    }
#endif // DO_CHECK

    return reinterpret_cast<TTo>(Forward<TFrom>(inObject));
}
