// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <type_traits>
#include "GCConcepts.h"
#include "Templates/Models.h"
#include "Concepts/StaticStructProvider.h"
#include "Concepts/BaseStructureProvider.h"

/**
 * @brief Utilities for working with USTRUCTs and intrinsic base structures (like `FSoftObjectPath`).
 */
namespace GCUtils::ScriptStruct
{
    /**
     * @brief Test if a given type a static getter function for the `UScriptStruct`.
     * @note This is generic to all USTRUCTs as well as intrinsic base structures (like `FSoftObjectPath`).
     */
    template <class T>
    consteval bool HasStaticScriptStructGetter();

    /**
     * @brief Test if a given type has the `UScriptStruct* GetScriptStruct() const` member function.
     */
    template <class T>
    consteval bool HasRuntimeScriptStructGetter();

    /**
     * @brief Concept satisfied by types with static getter functions to their script structs.
     * @note This is generic to all USTRUCTs as well as intrinsic base structures (like `FSoftObjectPath`).
     */
    template <class T>
    concept GettableStaticScriptStruct = HasStaticScriptStructGetter<T>();

    /**
     * @brief Concept satisfied by types with non-static getter functions to their script structs.
     */
    template <class T>
    concept GettableRuntimeScriptStruct = HasRuntimeScriptStructGetter<T>();

    template <class T>
    concept PointerToGettableStaticScriptStruct = GCConcepts::Pointer<T> && GettableStaticScriptStruct<std::remove_pointer_t<T>>;

    template <class T>
    concept PointerToGettableRuntimeScriptStruct = GCConcepts::Pointer<T> && GettableRuntimeScriptStruct<std::remove_pointer_t<T>>;

    template <class T>
    concept ReferenceToGettableStaticScriptStruct = GCConcepts::Reference<T> && GettableStaticScriptStruct<std::remove_reference_t<T>>;

    template <class T>
    concept ReferenceToGettableRuntimeScriptStruct = GCConcepts::Reference<T> && GettableRuntimeScriptStruct<std::remove_reference_t<T>>;

    /**
     * @brief Dynamic cast function for USTRUCTs - from pointer to pointer. Returns null if cast fails based on type information.
     */
    template <PointerToGettableStaticScriptStruct TTo, PointerToGettableRuntimeScriptStruct TFrom>
    FORCEINLINE_DEBUGGABLE TTo Cast(TFrom inValue);
    /**
     * @brief Dynamic cast function for USTRUCTs - from reference to pointer. Returns null if cast fails based on type information.
     * @note The from value must be an lvalue.
     */
    template <PointerToGettableStaticScriptStruct TTo, ReferenceToGettableRuntimeScriptStruct TFrom>
        requires (std::is_reference_v<TFrom> == false)
    FORCEINLINE_DEBUGGABLE TTo Cast(TFrom& inValue);

    /**
     * @brief Check-asserted dynamic cast function for USTRUCTs - from pointer to pointer. Null may be returned if null was passed in.
     */
    template <PointerToGettableStaticScriptStruct TTo, PointerToGettableRuntimeScriptStruct TFrom>
    FORCEINLINE_DEBUGGABLE TTo CastChecked(TFrom inValue);
    /**
     * @brief Check-asserted dynamic cast function for USTRUCTs - from pointer to reference. Null must not be passed in.
     */
    template <ReferenceToGettableStaticScriptStruct TTo, PointerToGettableRuntimeScriptStruct TFrom>
    FORCEINLINE_DEBUGGABLE TTo CastChecked(TFrom inValue);
    /**
     * @brief Check-asserted dynamic cast function for USTRUCTs - from reference to reference.
     */
    template <ReferenceToGettableStaticScriptStruct TTo, ReferenceToGettableRuntimeScriptStruct TFrom>
    FORCEINLINE_DEBUGGABLE TTo CastChecked(TFrom&& inValue);

    /**
     * @brief Get the `UScriptStruct` for a struct type.
     * @note This is generic to all USTRUCTs as well as intrinsic base structures (like `FSoftObjectPath`).
     */
    template <GettableStaticScriptStruct T>
    UScriptStruct* GetScriptStructStatic();

    /**
     * @brief Get the `UScriptStruct` for a struct type.
     * @note This is generic to all USTRUCTs as well as intrinsic base structures (like `FSoftObjectPath`).
     */
    template <GettableStaticScriptStruct T>
    UScriptStruct& GetScriptStructStaticChecked();
}

template <class T>
consteval bool GCUtils::ScriptStruct::HasStaticScriptStructGetter()
{
    if constexpr (TModels<CStaticStructProvider, T>::Value)
    {
        return true;
    }

    if constexpr (TModels<CBaseStructureProvider, T>::Value)
    {
        return true;
    }

    return false;
}

template <class T>
consteval bool GCUtils::ScriptStruct::HasRuntimeScriptStructGetter()
{
    using FRequiredMemberFunctionPtrType = UScriptStruct* (T::*)() const;

    constexpr bool hasMember =
        requires { static_cast<FRequiredMemberFunctionPtrType>(&T::GetScriptStruct); };

    if constexpr (!hasMember)
    {
        // Neither `T` nor any of its parent classes have the GetScriptStruct function.
        return false;
    }

    using FExistingMemberFunctionPtrType = decltype(&T::GetScriptStruct);
    constexpr bool hasMemberExact = std::same_as<FExistingMemberFunctionPtrType, FRequiredMemberFunctionPtrType>;

    if constexpr (!hasMemberExact)
    {
        // `T` itself doesn't have the GetScriptStruct function.
        return false;
    }

    return true;
}

template <GCUtils::ScriptStruct::PointerToGettableStaticScriptStruct TTo, GCUtils::ScriptStruct::PointerToGettableRuntimeScriptStruct TFrom>
TTo GCUtils::ScriptStruct::Cast(TFrom inValue)
{
    using FToStruct = std::remove_pointer_t<TTo>;
    using FFromStruct = std::remove_pointer_t<TFrom>;

    constexpr bool isDowncasting = !std::is_same_v<FFromStruct, FToStruct> && std::is_base_of_v<FFromStruct, FToStruct>;
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
template <GCUtils::ScriptStruct::PointerToGettableStaticScriptStruct TTo, GCUtils::ScriptStruct::ReferenceToGettableRuntimeScriptStruct TFrom>
    requires (std::is_reference_v<TFrom> == false)
TTo GCUtils::ScriptStruct::Cast(TFrom& inValue)
{
    using FToStruct = std::remove_pointer_t<TTo>;
    using FFromStruct = std::remove_reference_t<TFrom>;

    constexpr bool isDowncasting = !std::is_same_v<FFromStruct, FToStruct> && std::is_base_of_v<FFromStruct, FToStruct>;
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

    const UScriptStruct& toScriptStruct = GetScriptStructStaticChecked<FToStruct>();
    if (toScriptStruct.IsChildOf(fromScriptStruct) == false)
    {
        return nullptr;
    }

    return &static_cast<FToStruct&>(Forward<TFrom>(inValue));
}

template <GCUtils::ScriptStruct::PointerToGettableStaticScriptStruct TTo, GCUtils::ScriptStruct::PointerToGettableRuntimeScriptStruct TFrom>
TTo GCUtils::ScriptStruct::CastChecked(TFrom inValue)
{
    using FToStruct = std::remove_reference_t<TTo>;
    using FFromStruct = std::remove_pointer_t<TFrom>;

    check(inValue);

    constexpr bool isDowncasting = !std::is_same_v<FFromStruct, FToStruct> && std::is_base_of_v<FFromStruct, FToStruct>;
    if constexpr (!isDowncasting)
    {
        // Not a downcast. No need for runtime type testing.
        return static_cast<TTo>(*inValue);
    }

    return &CastScriptStruct<FToStruct&>(*inValue);
}
template <GCUtils::ScriptStruct::ReferenceToGettableStaticScriptStruct TTo, GCUtils::ScriptStruct::PointerToGettableRuntimeScriptStruct TFrom>
TTo GCUtils::ScriptStruct::CastChecked(TFrom inValue)
{
    using FToStruct = std::remove_reference_t<TTo>;
    using FFromStruct = std::remove_pointer_t<TFrom>;

    check(inValue);

    constexpr bool isDowncasting = !std::is_same_v<FFromStruct, FToStruct> && std::is_base_of_v<FFromStruct, FToStruct>;
    if constexpr (!isDowncasting)
    {
        // Not a downcast. No need for runtime type testing.
        return static_cast<TTo>(*inValue);
    }

    return CastScriptStruct<TTo>(*inValue);
}
template <GCUtils::ScriptStruct::ReferenceToGettableStaticScriptStruct TTo, GCUtils::ScriptStruct::ReferenceToGettableRuntimeScriptStruct TFrom>
TTo GCUtils::ScriptStruct::CastChecked(TFrom&& inValue)
{
    using FToStruct = std::remove_reference_t<TTo>;
    using FFromStruct = std::remove_reference_t<TFrom>;

    constexpr bool isDowncasting = !std::is_same_v<FFromStruct, FToStruct> && std::is_base_of_v<FFromStruct, FToStruct>;
    if constexpr (!isDowncasting)
    {
        // Not a downcast. No need for runtime type testing.
        return static_cast<TTo>(Forward<TFrom>(inValue));
    }

    UScriptStruct* fromScriptStruct = inValue.GetScriptStruct();
    check(fromScriptStruct);

    const UScriptStruct& toScriptStruct = GetScriptStructStaticChecked<FToStruct>();
    check(toScriptStruct.IsChildOf(fromScriptStruct));

    return static_cast<TTo>(Forward<TFrom>(inValue));
}

template <GCUtils::ScriptStruct::GettableStaticScriptStruct T>
UScriptStruct* GCUtils::ScriptStruct::GetScriptStructStatic()
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

template <GCUtils::ScriptStruct::GettableStaticScriptStruct T>
UScriptStruct& GCUtils::ScriptStruct::GetScriptStructStaticChecked()
{
    UScriptStruct* scriptStruct = GetScriptStructStatic<T>();
    check(scriptStruct);
    return *scriptStruct;
}
