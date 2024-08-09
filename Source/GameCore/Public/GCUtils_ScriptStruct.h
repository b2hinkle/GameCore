// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "type_traits"
#include "GCConcepts.h"
#include "Templates/GCHasStaticScriptStruct.h"
#include "Concepts/BaseStructureProvider.h"
#include "Templates/GCHasGetScriptStruct.h"

/**
 *
 */
namespace GCUtils::ScriptStruct
{
    /**
     * @brief Dynamic cast function for USTRUCTs.
     * @param inValue Pointer to the struct object.
     * @return Casted pointer to the struct object. Null if script structs are incompatible.
     */
    template <GCConcepts::Pointer TTo, GCConcepts::Pointer TFrom>
        requires (GCHasStaticScriptStruct<std::remove_pointer_t<TTo>>() && GCHasGetScriptStruct<std::remove_pointer_t<TFrom>>())
    FORCEINLINE_DEBUGGABLE TTo CastScriptStruct(TFrom inValue);

    /**
     * @brief Dynamic cast function for USTRUCTs.
     * @param inValue Reference to the struct object.
     * @return Casted pointer to the struct object. Null if script structs are incompatible.
     */
    template <GCConcepts::Pointer TTo, GCConcepts::Reference TFrom>
        requires (GCHasStaticScriptStruct<std::remove_pointer_t<TTo>>() && GCHasGetScriptStruct<std::remove_reference_t<TFrom>>())
    FORCEINLINE_DEBUGGABLE TTo CastScriptStruct(TFrom&& inValue);

    /**
     * @brief Dynamic cast function for USTRUCTs. Check-asserts that script structs are compatible.
     * @param inValue Pointer to the struct object.
     * @return Casted reference to the struct object.
     */
    template <GCConcepts::Reference TTo, GCConcepts::Pointer TFrom>
        requires (GCHasStaticScriptStruct<std::remove_reference_t<TTo>>() && GCHasGetScriptStruct<std::remove_pointer_t<TFrom>>())
    FORCEINLINE_DEBUGGABLE TTo CastScriptStruct(TFrom inValue);

    /**
     * @brief Dynamic cast function for USTRUCTs. Check-asserts that script structs are compatible.
     * @param inValue Reference to the struct object.
     * @return Casted reference to the struct object.
     */
    template <GCConcepts::Reference TTo, GCConcepts::Reference TFrom>
        requires (GCHasStaticScriptStruct<std::remove_reference_t<TTo>>() && GCHasGetScriptStruct<std::remove_reference_t<TFrom>>())
    FORCEINLINE_DEBUGGABLE TTo CastScriptStruct(TFrom&& inValue);

    /**
     * @brief Get the `UScriptStruct` for a struct type.
     * @note Works for both USTRUCTs and instrinsic base structures, e.g., `FSoftObjectPath`.
     */
    template <class T>
    UScriptStruct* GCGetStaticScriptStruct();

    /**
     * @brief Get the `UScriptStruct` for a struct type.
     * @note Works for both USTRUCTs and instrinsic base structures, e.g., `FSoftObjectPath`.
     */
    template <class T, class = std::enable_if_t<GCHasGetScriptStruct<T>()>>
    UScriptStruct& GCGetStaticScriptStructChecked();
}

template <GCConcepts::Pointer TTo, GCConcepts::Pointer TFrom>
    requires (GCHasStaticScriptStruct<std::remove_pointer_t<TTo>>() && GCHasGetScriptStruct<std::remove_pointer_t<TFrom>>())
TTo GCUtils::ScriptStruct::CastScriptStruct(TFrom inValue)
{
    using FToStruct = std::remove_pointer_t<TTo>;
    using FFromStruct = std::remove_pointer_t<TFrom>;

    constexpr bool isDowncasting = !std::is_same_v<FFromStruct, FToStruct> && !std::is_base_of_v<FFromStruct, FToStruct>;
    if constexpr (!isDowncasting)
    {
        // Not a downcast. No need for runtime type testing.
        return static_cast<FToStruct*>(inValue);
    }

    if (!inValue)
    {
        return nullptr;
    }

    return CastScriptStruct<TTo>(*inValue);
}

template <GCConcepts::Pointer TTo, GCConcepts::Reference TFrom>
    requires (GCHasStaticScriptStruct<std::remove_pointer_t<TTo>>() && GCHasGetScriptStruct<std::remove_reference_t<TFrom>>())
TTo GCUtils::ScriptStruct::CastScriptStruct(TFrom&& inValue)
{
    using FToStruct = std::remove_pointer_t<TTo>;
    using FFromStruct = std::remove_reference_t<TFrom>;

    constexpr bool isDowncasting = !std::is_same_v<FFromStruct, FToStruct> && !std::is_base_of_v<FFromStruct, FToStruct>;
    if constexpr (!isDowncasting)
    {
        // Not a downcast. No need for runtime type testing.
        return &static_cast<FToStruct&>(Forward<TFrom>(inValue));
    }

    UScriptStruct* fromScriptStruct = inValue.GetScriptStruct();
    if (!fromScriptStruct)
    {
        return nullptr;
    }

    const UScriptStruct& toScriptStruct = GCGetStaticScriptStructChecked<FToStruct>();
    if (toScriptStruct.IsChildOf(fromScriptStruct) == false)
    {
        return nullptr;
    }

    return &static_cast<FToStruct&>(Forward<TFrom>(inValue));
}

template <GCConcepts::Reference TTo, GCConcepts::Pointer TFrom>
    requires (GCHasStaticScriptStruct<std::remove_reference_t<TTo>>() && GCHasGetScriptStruct<std::remove_pointer_t<TFrom>>())
TTo GCUtils::ScriptStruct::CastScriptStruct(TFrom inValue)
{
    using FToStruct = std::remove_reference_t<TTo>;
    using FFromStruct = std::remove_pointer_t<TFrom>;

    check(inValue);

    constexpr bool isDowncasting = !std::is_same_v<FFromStruct, FToStruct> && !std::is_base_of_v<FFromStruct, FToStruct>;
    if constexpr (!isDowncasting)
    {
        // Not a downcast. No need for runtime type testing.
        return static_cast<TTo>(*inValue);
    }

    return CastScriptStruct<TTo>(*inValue);
}

template <GCConcepts::Reference TTo, GCConcepts::Reference TFrom>
    requires (GCHasStaticScriptStruct<std::remove_reference_t<TTo>>() && GCHasGetScriptStruct<std::remove_reference_t<TFrom>>())
TTo GCUtils::ScriptStruct::CastScriptStruct(TFrom&& inValue)
{
    using FToStruct = std::remove_reference_t<TTo>;
    using FFromStruct = std::remove_reference_t<TFrom>;

    constexpr bool isDowncasting = !std::is_same_v<FFromStruct, FToStruct> && !std::is_base_of_v<FFromStruct, FToStruct>;
    if constexpr (!isDowncasting)
    {
        // Not a downcast. No need for runtime type testing.
        return static_cast<TTo>(Forward<TFrom>(inValue));
    }

    UScriptStruct* fromScriptStruct = inValue.GetScriptStruct();
    check(fromScriptStruct);

    const UScriptStruct& toScriptStruct = GCGetStaticScriptStructChecked<std::remove_reference_t<TTo>>();
    check(toScriptStruct.IsChildOf(fromScriptStruct));

    return static_cast<TTo>(Forward<TFrom>(inValue));
}

template <class T>
UScriptStruct* GCUtils::ScriptStruct::GCGetStaticScriptStruct()
{
    if constexpr (TModels<CStaticStructProvider, T>::Value)
    {
        return T::StaticStruct();
    }

    if constexpr (TModels<CBaseStructureProvider, T>::Value)
    {
        return TBaseStructure<T>::Get();
    }

    return nullptr;
}

template <class T, class>
UScriptStruct& GCUtils::ScriptStruct::GCGetStaticScriptStructChecked()
{
    check(GCGetStaticScriptStruct<T>());
    return *GCGetStaticScriptStruct<T>();
}
