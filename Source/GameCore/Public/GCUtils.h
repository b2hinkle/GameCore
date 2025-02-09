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

    // Overloads for casting from pointer to reference. Null may be returned if null was passed in.

    template <GCConcepts::PointerToIInterface TToPtr, GCConcepts::PointerToIInterface TFromPtr>
    FORCEINLINE_DEBUGGABLE TToPtr CastUObjectChecked(TFromPtr inObject);
    template <GCConcepts::PointerToUObjectDerived TToPtr, GCConcepts::PointerToIInterface TFromPtr>
    FORCEINLINE_DEBUGGABLE TToPtr CastUObjectChecked(TFromPtr inObject);
    template <GCConcepts::PointerToIInterface TToPtr, GCConcepts::PointerToUObjectDerived TFromPtr>
    FORCEINLINE_DEBUGGABLE TToPtr CastUObjectChecked(TFromPtr inObject);
    template <GCConcepts::PointerToUObjectDerived TToPtr, GCConcepts::PointerToUObjectDerived TFromPtr>
    FORCEINLINE_DEBUGGABLE TToPtr CastUObjectChecked(TFromPtr inObject);

    // Overloads for casting from pointer to reference. Null must not be passed in.

    template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::PointerToIInterface TFromPtr>
    FORCEINLINE_DEBUGGABLE TToRef CastUObjectChecked(TFromPtr inObject);
    template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::PointerToIInterface TFromPtr>
    FORCEINLINE_DEBUGGABLE TToRef CastUObjectChecked(TFromPtr inObject);
    template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::PointerToUObjectDerived TFromPtr>
    FORCEINLINE_DEBUGGABLE TToRef CastUObjectChecked(TFromPtr inObject);
    template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::PointerToUObjectDerived TFromPtr>
    FORCEINLINE_DEBUGGABLE TToRef CastUObjectChecked(TFromPtr inObject);

    // Overloads for casting from reference to reference.

    template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::ReferenceToIInterface TFromRef>
    FORCEINLINE_DEBUGGABLE TToRef CastUObjectChecked(TFromRef& inObject);
    template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::ReferenceToIInterface TFromRef>
    FORCEINLINE_DEBUGGABLE TToRef CastUObjectChecked(TFromRef& inObject);
    template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::ReferenceToUObjectDerived TFromRef>
    FORCEINLINE_DEBUGGABLE TToRef CastUObjectChecked(TFromRef& inObject);
    template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::ReferenceToUObjectDerived TFromRef>
    FORCEINLINE_DEBUGGABLE TToRef CastUObjectChecked(TFromRef& inObject);
}

template <GCConcepts::NonLvalueReference T>
T& GCUtils::Materialize(T&& inTemporary)
{
    // Note that the temporary gets promoted to an lvalue for the scope of this function. It
    // will revert to its original lifetime of temporary when this function is exited.
    return static_cast<T&>(inTemporary);
}

template <GCConcepts::PointerToIInterface TToPtr, GCConcepts::PointerToIInterface TFromPtr>
TToPtr GCUtils::CastUObjectChecked(TFromPtr inObject)
{
    if (!inObject)
    {
        return nullptr;
    }

    using FFromType = std::remove_pointer_t<TFromPtr>;
    using FToType = std::remove_pointer_t<TToPtr>;

    check(inObject);
    return &CastUObjectChecked<FToType&, FFromType&>(*inObject);
}
template <GCConcepts::PointerToUObjectDerived TToPtr, GCConcepts::PointerToIInterface TFromPtr>
TToPtr GCUtils::CastUObjectChecked(TFromPtr inObject)
{
    if (!inObject)
    {
        return nullptr;
    }

    using FFromType = std::remove_pointer_t<TFromPtr>;
    using FToType = std::remove_pointer_t<TToPtr>;

    check(inObject);
    return &CastUObjectChecked<FToType&, FFromType&>(*inObject);
}
template <GCConcepts::PointerToIInterface TToPtr, GCConcepts::PointerToUObjectDerived TFromPtr>
TToPtr GCUtils::CastUObjectChecked(TFromPtr inObject)
{
    if (!inObject)
    {
        return nullptr;
    }

    using FFromType = std::remove_pointer_t<TFromPtr>;
    using FToType = std::remove_pointer_t<TToPtr>;

    check(inObject);
    return &CastUObjectChecked<FToType&, FFromType&>(*inObject);
}
template <GCConcepts::PointerToUObjectDerived TToPtr, GCConcepts::PointerToUObjectDerived TFromPtr>
TToPtr GCUtils::CastUObjectChecked(TFromPtr inObject)
{
    if (!inObject)
    {
        return nullptr;
    }

    using FFromType = std::remove_pointer_t<TFromPtr>;
    using FToType = std::remove_pointer_t<TToPtr>;

    check(inObject);
    return &CastUObjectChecked<FToType&, FFromType&>(*inObject);
}

template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::PointerToIInterface TFromPtr>
TToRef GCUtils::CastUObjectChecked(TFromPtr inObject)
{
    using FFromType = std::remove_pointer_t<TFromPtr>;

    check(inObject);
    return CastUObjectChecked<TToRef, FFromType&>(*inObject);
}
template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::PointerToIInterface TFromPtr>
TToRef GCUtils::CastUObjectChecked(TFromPtr inObject)
{
    using FFromType = std::remove_pointer_t<TFromPtr>;

    check(inObject);
    return CastUObjectChecked<TToRef, FFromType&>(*inObject);
}
template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::PointerToUObjectDerived TFromPtr>
TToRef GCUtils::CastUObjectChecked(TFromPtr inObject)
{
    using FFromType = std::remove_pointer_t<TFromPtr>;

    check(inObject);
    return CastUObjectChecked<TToRef, FFromType&>(*inObject);
}
template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::PointerToUObjectDerived TFromPtr>
TToRef GCUtils::CastUObjectChecked(TFromPtr inObject)
{
    using FFromType = std::remove_pointer_t<TFromPtr>;

    check(inObject);
    return CastUObjectChecked<TToRef, FFromType&>(*inObject);
}

template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::ReferenceToIInterface TFromRef>
TToRef GCUtils::CastUObjectChecked(TFromRef& inObject)
{
#if DO_CHECK
    {
        using FToType = std::remove_reference_t<TToRef>;
        check(Cast<FToType>(&inObject));
    }
#endif // #if DO_CHECK

    UObject* implementerObject = inObject._getUObject();
    check(implementerObject);

    return CastUObjectChecked<TToRef, UObject&>(*implementerObject);
}
template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::ReferenceToIInterface TFromRef>
TToRef GCUtils::CastUObjectChecked(TFromRef& inObject)
{
#if DO_CHECK
    {
        using FToType = std::remove_reference_t<TToRef>;
        check(Cast<FToType>(&inObject));
    }
#endif // #if DO_CHECK

    UObject* implementerObject = inObject._getUObject();
    check(implementerObject);

    return CastUObjectChecked<TToRef, UObject&>(*implementerObject);
}
template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::ReferenceToUObjectDerived TFromRef>
TToRef GCUtils::CastUObjectChecked(TFromRef& inObject)
{
    using FToType = std::remove_reference_t<TToRef>;
    using FFromType = std::remove_reference_t<TFromRef>;

#if DO_CHECK
    {
        check(Cast<FToType>(&inObject));
    }
#endif // #if DO_CHECK

    // A conditionally const void pointer type based on the qualifiers of `FFromType`.
    using FVoidPointerToInterface = std::conditional_t<std::is_const_v<FFromType>, const void*, void*>;

    // Get the address of the interface on the object.
    FVoidPointerToInterface toAddress = Forward<TFromRef>(inObject).GetNativeInterfaceAddress(FToType::UClassType::StaticClass());
    check(toAddress);

    FToType& toInterface = *reinterpret_cast<FToType*>(toAddress);
    return Forward<TToRef>(toInterface);
}
template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::ReferenceToUObjectDerived TFromRef>
TToRef GCUtils::CastUObjectChecked(TFromRef& inObject)
{
    using FToType = std::remove_reference_t<TToRef>;

#if DO_CHECK
    {
        check(Cast<FToType>(&inObject));
    }
#endif // #if DO_CHECK

    FToType& toObject = static_cast<FToType&>(Forward<TFromRef>(inObject));
    return Forward<TToRef>(toObject);
}
