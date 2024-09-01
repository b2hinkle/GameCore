// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <type_traits>
#include "GCConcepts.h"

namespace GCUtils
{
    /**
     * Get the "owning" controller for an object.
     */
    GAMECORE_API const AController* GetController(const UObject* inObject);
    GAMECORE_API AController* GetController(UObject* inObject);

    /**
     * @brief Materialize a prvalue to a temporary and get an lvalue reference to it.
     * @tparam T Type of temporary to materialize. Automatically deduced but may be specified
     *         manually in order to implicitly convert the prvalue to another type.
     * @param inTemporary The caller's prvalue argument materialized to temporary.
     * @return Lvalue reference to the temporary.
     */
    template <GCConcepts::NonLvalueReference T>
    T& Materialize(T&& inTemporary);

    /**
     * If inObject is already guaranteed to be a TTo.
     *
     * E.g., you load an asset by class and you are certain the result is a TTo.
     *
     * E.g., you are up-casting.
     */
    template <GCConcepts::PointerToUObjectDerivedOrIInterface TToPtr, GCConcepts::PointerToUObjectDerivedOrIInterface TFromPtr>
    FORCEINLINE_DEBUGGABLE TToPtr StaticCastChecked(TFromPtr inObject);
    /**
     * If inObject is already guaranteed to be a TTo.
     *
     * E.g., you load an asset by class and you are certain the result is a TTo.
     *
     * E.g., you are up-casting.
     */
    template <GCConcepts::PointerToUObjectDerivedOrIInterface TToPtr, GCConcepts::ReferenceToUObjectDerivedOrIInterface TFromRef>
    FORCEINLINE_DEBUGGABLE TToPtr StaticCastChecked(TFromRef&& inObject);
    /**
     * If inObject is already guaranteed to be a TTo.
     *
     * E.g., you load an asset by class and you are certain the result is a TTo.
     *
     * E.g., you are up-casting.
     */
    template <GCConcepts::ReferenceToUObjectDerivedOrIInterface TToRef, GCConcepts::PointerToUObjectDerivedOrIInterface TFromPtr>
    FORCEINLINE_DEBUGGABLE TToRef StaticCastChecked(TFromPtr inObject);
    /**
     * If inObject is already guaranteed to be a TTo.
     *
     * E.g., you load an asset by class and you are certain the result is a TTo.
     *
     * E.g., you are up-casting.
     */
    template <GCConcepts::ReferenceToUObjectDerivedOrIInterface TToRef, GCConcepts::ReferenceToUObjectDerivedOrIInterface TFromRef>
    FORCEINLINE_DEBUGGABLE TToRef StaticCastChecked(TFromRef&& inObject);

    /**
     * If static casting isn't an option but inObject is still guaranteed to be a TTo.
     *
     * Necessary for casting to interface classes that aren't part of TFrom's inheritance chain.
     */
    template <GCConcepts::PointerToUObjectDerivedOrIInterface TToPtr, GCConcepts::PointerToUObjectDerivedOrIInterface TFromPtr>
    FORCEINLINE_DEBUGGABLE TToPtr ReinterpretCastChecked(TFromPtr inObject);
    /**
     * If static casting isn't an option but inObject is still guaranteed to be a TTo.
     *
     * Necessary for casting to interface classes that aren't part of TFrom's inheritance chain.
     */
    template <GCConcepts::PointerToUObjectDerivedOrIInterface TToPtr, GCConcepts::ReferenceToUObjectDerivedOrIInterface TFromRef>
    FORCEINLINE_DEBUGGABLE TToPtr ReinterpretCastChecked(TFromRef&& inObject);
    /**
     * If static casting isn't an option but inObject is still guaranteed to be a TTo.
     *
     * Necessary for casting to interface classes that aren't part of TFrom's inheritance chain.
     */
    template <GCConcepts::ReferenceToUObjectDerivedOrIInterface TToRef, GCConcepts::PointerToUObjectDerivedOrIInterface TFromPtr>
    FORCEINLINE_DEBUGGABLE TToRef ReinterpretCastChecked(TFromPtr inObject);
    /**
     * If static casting isn't an option but inObject is still guaranteed to be a TTo.
     *
     * Necessary for casting to interface classes that aren't part of TFrom's inheritance chain.
     */
    template <GCConcepts::ReferenceToUObjectDerivedOrIInterface TToRef, GCConcepts::ReferenceToUObjectDerivedOrIInterface TFromRef>
    FORCEINLINE_DEBUGGABLE TToRef ReinterpretCastChecked(TFromRef&& inObject);
}

template <GCConcepts::NonLvalueReference T>
T& GCUtils::Materialize(T&& inTemporary)
{
    // Note that the temporary gets promoted to an lvalue for the scope of this function. It
    // will revert to its original lifetime of temporary when this function is exited.
    return static_cast<T&>(inTemporary);
}

template <GCConcepts::PointerToUObjectDerivedOrIInterface TToPtr, GCConcepts::PointerToUObjectDerivedOrIInterface TFromPtr>
TToPtr GCUtils::StaticCastChecked(TFromPtr inObject)
{
#if DO_CHECK
    if (inObject)
    {
        using FToType = std::remove_pointer_t<TToPtr>;
        check(Cast<FToType>(inObject));
    }
#endif // DO_CHECK

    return static_cast<TToPtr>(inObject);
}
template <GCConcepts::PointerToUObjectDerivedOrIInterface TToPtr, GCConcepts::ReferenceToUObjectDerivedOrIInterface TFromRef>
TToPtr GCUtils::StaticCastChecked(TFromRef&& inObject)
{
#if DO_CHECK
    {
        using FToType = std::remove_pointer_t<TToPtr>;
        check(Cast<FToType>(&inObject));
    }
#endif // DO_CHECK

    return static_cast<TToPtr>(&inObject);
}
template <GCConcepts::ReferenceToUObjectDerivedOrIInterface TToRef, GCConcepts::PointerToUObjectDerivedOrIInterface TFromPtr>
TToRef GCUtils::StaticCastChecked(TFromPtr inObject)
{
    check(inObject);
    return StaticCastChecked<TToRef>(*inObject);
}
template <GCConcepts::ReferenceToUObjectDerivedOrIInterface TToRef, GCConcepts::ReferenceToUObjectDerivedOrIInterface TFromRef>
TToRef GCUtils::StaticCastChecked(TFromRef&& inObject)
{
#if DO_CHECK
    {
        using FToType = std::remove_reference_t<TToRef>;
        check(Cast<FToType>(&inObject));
    }
#endif // DO_CHECK

    return static_cast<TToRef>(Forward<TFromRef>(inObject));
}

template <GCConcepts::PointerToUObjectDerivedOrIInterface TToPtr, GCConcepts::PointerToUObjectDerivedOrIInterface TFromPtr>
TToPtr GCUtils::ReinterpretCastChecked(TFromPtr inObject)
{
#if DO_CHECK
    if (inObject)
    {
        using FToType = std::remove_pointer_t<TToPtr>;
        check(Cast<FToType>(inObject));
    }
#endif // DO_CHECK

    return reinterpret_cast<TToPtr>(inObject);
}
template <GCConcepts::PointerToUObjectDerivedOrIInterface TToPtr, GCConcepts::ReferenceToUObjectDerivedOrIInterface TFromRef>
TToPtr GCUtils::ReinterpretCastChecked(TFromRef&& inObject)
{
#if DO_CHECK
    {
        using FToType = std::remove_pointer_t<TToPtr>;
        check(Cast<FToType>(&inObject));
    }
#endif // DO_CHECK

    return reinterpret_cast<TToPtr>(&inObject);
}
template <GCConcepts::ReferenceToUObjectDerivedOrIInterface TToRef, GCConcepts::PointerToUObjectDerivedOrIInterface TFromPtr>
TToRef GCUtils::ReinterpretCastChecked(TFromPtr inObject)
{
    check(inObject);
    return ReinterpretCastChecked<TToRef>(*inObject);
}
template <GCConcepts::ReferenceToUObjectDerivedOrIInterface TToRef, GCConcepts::ReferenceToUObjectDerivedOrIInterface TFromRef>
TToRef GCUtils::ReinterpretCastChecked(TFromRef&& inObject)
{
#if DO_CHECK
    {
        using FToType = std::remove_reference_t<TToRef>;
        check(Cast<FToType>(&inObject));
    }
#endif // DO_CHECK

    return reinterpret_cast<TToRef>(Forward<TFromRef>(inObject));
}
