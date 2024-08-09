// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <type_traits>

class UScriptStruct;

/**
 *
 */
template <class T>
consteval bool GCHasGetScriptStruct()
{
    using FRequiredMemberFunctionPtrType = UScriptStruct* (T::*)() const;

    constexpr bool hasMember =
        requires { static_cast<FRequiredMemberFunctionPtrType>(&T::GetScriptStruct); };

    if constexpr (!hasMember)
    {
        // Neither `T` or any of its parent classes have this function.
        return false;
    }

    using FExistingMemberFunctionPtrType = decltype(&T::GetScriptStruct);
    constexpr bool hasMemberExact = std::same_as<FExistingMemberFunctionPtrType, FRequiredMemberFunctionPtrType>;

    if constexpr (!hasMemberExact)
    {
        // `T` itself doesn't have this function.
        return false;
    }

    return true;
}
