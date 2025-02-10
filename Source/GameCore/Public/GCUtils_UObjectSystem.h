// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <type_traits>
#include "GCConcepts.h"

namespace GCUtils::UObjectSystem
{
    // Overloads for casting from pointer to reference. Null may be returned if null was passed in.

    template <GCConcepts::PointerToIInterface TToPtr, GCConcepts::PointerToIInterface TFromPtr>
    FORCEINLINE_DEBUGGABLE TToPtr CastChecked(TFromPtr inObject);
    template <GCConcepts::PointerToUObjectDerived TToPtr, GCConcepts::PointerToIInterface TFromPtr>
    FORCEINLINE_DEBUGGABLE TToPtr CastChecked(TFromPtr inObject);
    template <GCConcepts::PointerToIInterface TToPtr, GCConcepts::PointerToUObjectDerived TFromPtr>
    FORCEINLINE_DEBUGGABLE TToPtr CastChecked(TFromPtr inObject);
    template <GCConcepts::PointerToUObjectDerived TToPtr, GCConcepts::PointerToUObjectDerived TFromPtr>
    FORCEINLINE_DEBUGGABLE TToPtr CastChecked(TFromPtr inObject);

    // Overloads for casting from pointer to reference. Null must not be passed in.

    template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::PointerToIInterface TFromPtr>
    FORCEINLINE_DEBUGGABLE TToRef CastChecked(TFromPtr inObject);
    template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::PointerToIInterface TFromPtr>
    FORCEINLINE_DEBUGGABLE TToRef CastChecked(TFromPtr inObject);
    template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::PointerToUObjectDerived TFromPtr>
    FORCEINLINE_DEBUGGABLE TToRef CastChecked(TFromPtr inObject);
    template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::PointerToUObjectDerived TFromPtr>
    FORCEINLINE_DEBUGGABLE TToRef CastChecked(TFromPtr inObject);

    // Overloads for casting from reference to reference.

    template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::ReferenceToIInterface TFromRef>
    FORCEINLINE_DEBUGGABLE TToRef CastChecked(TFromRef& inObject);
    template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::ReferenceToIInterface TFromRef>
    FORCEINLINE_DEBUGGABLE TToRef CastChecked(TFromRef& inObject);
    template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::ReferenceToUObjectDerived TFromRef>
    FORCEINLINE_DEBUGGABLE TToRef CastChecked(TFromRef& inObject);
    template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::ReferenceToUObjectDerived TFromRef>
    FORCEINLINE_DEBUGGABLE TToRef CastChecked(TFromRef& inObject);
}

template <GCConcepts::PointerToIInterface TToPtr, GCConcepts::PointerToIInterface TFromPtr>
TToPtr GCUtils::UObjectSystem::CastChecked(TFromPtr inObject)
{
    if (!inObject)
    {
        return nullptr;
    }

    using FFromType = std::remove_pointer_t<TFromPtr>;
    using FToType = std::remove_pointer_t<TToPtr>;

    check(inObject);
    return &CastChecked<FToType&, FFromType&>(*inObject);
}
template <GCConcepts::PointerToUObjectDerived TToPtr, GCConcepts::PointerToIInterface TFromPtr>
TToPtr GCUtils::UObjectSystem::CastChecked(TFromPtr inObject)
{
    if (!inObject)
    {
        return nullptr;
    }

    using FFromType = std::remove_pointer_t<TFromPtr>;
    using FToType = std::remove_pointer_t<TToPtr>;

    check(inObject);
    return &CastChecked<FToType&, FFromType&>(*inObject);
}
template <GCConcepts::PointerToIInterface TToPtr, GCConcepts::PointerToUObjectDerived TFromPtr>
TToPtr GCUtils::UObjectSystem::CastChecked(TFromPtr inObject)
{
    if (!inObject)
    {
        return nullptr;
    }

    using FFromType = std::remove_pointer_t<TFromPtr>;
    using FToType = std::remove_pointer_t<TToPtr>;

    check(inObject);
    return &CastChecked<FToType&, FFromType&>(*inObject);
}
template <GCConcepts::PointerToUObjectDerived TToPtr, GCConcepts::PointerToUObjectDerived TFromPtr>
TToPtr GCUtils::UObjectSystem::CastChecked(TFromPtr inObject)
{
    if (!inObject)
    {
        return nullptr;
    }

    using FFromType = std::remove_pointer_t<TFromPtr>;
    using FToType = std::remove_pointer_t<TToPtr>;

    check(inObject);
    return &CastChecked<FToType&, FFromType&>(*inObject);
}

template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::PointerToIInterface TFromPtr>
TToRef GCUtils::UObjectSystem::CastChecked(TFromPtr inObject)
{
    using FFromType = std::remove_pointer_t<TFromPtr>;

    check(inObject);
    return CastChecked<TToRef, FFromType&>(*inObject);
}
template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::PointerToIInterface TFromPtr>
TToRef GCUtils::UObjectSystem::CastChecked(TFromPtr inObject)
{
    using FFromType = std::remove_pointer_t<TFromPtr>;

    check(inObject);
    return CastChecked<TToRef, FFromType&>(*inObject);
}
template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::PointerToUObjectDerived TFromPtr>
TToRef GCUtils::UObjectSystem::CastChecked(TFromPtr inObject)
{
    using FFromType = std::remove_pointer_t<TFromPtr>;

    check(inObject);
    return CastChecked<TToRef, FFromType&>(*inObject);
}
template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::PointerToUObjectDerived TFromPtr>
TToRef GCUtils::UObjectSystem::CastChecked(TFromPtr inObject)
{
    using FFromType = std::remove_pointer_t<TFromPtr>;

    check(inObject);
    return CastChecked<TToRef, FFromType&>(*inObject);
}

template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::ReferenceToIInterface TFromRef>
TToRef GCUtils::UObjectSystem::CastChecked(TFromRef& inObject)
{
#if DO_CHECK
    {
        using FToType = std::remove_reference_t<TToRef>;
        check(Cast<FToType>(&inObject));
    }
#endif // #if DO_CHECK

    UObject* implementerObject = inObject._getUObject();
    check(implementerObject);

    return CastChecked<TToRef, UObject&>(*implementerObject);
}
template <GCConcepts::ReferenceToUObjectDerived TToRef, GCConcepts::ReferenceToIInterface TFromRef>
TToRef GCUtils::UObjectSystem::CastChecked(TFromRef& inObject)
{
#if DO_CHECK
    {
        using FToType = std::remove_reference_t<TToRef>;
        check(Cast<FToType>(&inObject));
    }
#endif // #if DO_CHECK

    UObject* implementerObject = inObject._getUObject();
    check(implementerObject);

    return CastChecked<TToRef, UObject&>(*implementerObject);
}
template <GCConcepts::ReferenceToIInterface TToRef, GCConcepts::ReferenceToUObjectDerived TFromRef>
TToRef GCUtils::UObjectSystem::CastChecked(TFromRef& inObject)
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
TToRef GCUtils::UObjectSystem::CastChecked(TFromRef& inObject)
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
