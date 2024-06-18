// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/GCIsUObjectOrIInterface.h"
#include "Templates/GCRemovePtrOrRef.h"

namespace GCUtils
{
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
    FORCEINLINE_DEBUGGABLE TTo StaticCastChecked(TFrom&& inObject);

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
    FORCEINLINE_DEBUGGABLE TTo ReinterpretCastChecked(TFrom&& inObject);
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
