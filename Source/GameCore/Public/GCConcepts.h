// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <type_traits>
#include "Templates/Casts.h"
#include "Traits/IsCharType.h"

class UObject;

namespace GCConcepts
{
    template <class T>
    concept Pointer = std::is_pointer_v<T>;

    template <class T>
    concept Reference = std::is_reference_v<T>;

    template <class T>
    concept LvalueReference = std::is_lvalue_reference_v<T>;

    template <class T>
    concept RvalueReference = std::is_rvalue_reference_v<T>;

    /**
     * @brief Concept satisfied by types that are not lvalue references. This is a useful constraint for
     *        being able to have an rvalue-reference function parameter to a template-argument type
     *        without the possibility of an lvalue reference being used in that forwarding reference.
     */
    template <class T>
    concept NonLvalueReference = !std::is_lvalue_reference_v<T>;

    template <class T>
    concept CharType = static_cast<bool>(TIsCharType<T>::Value);

    template <class T, class TBase>
    concept PointerToDerivedFrom = std::is_pointer_v<T> && std::derived_from<std::remove_pointer_t<T>, TBase>;

    template <class T, class TBase>
    concept ReferenceToDerivedFrom = std::is_reference_v<T> && std::derived_from<std::remove_reference_t<T>, TBase>;

    template <class T>
    concept UObjectDerived = std::derived_from<T, UObject>;

    template <class T>
    concept PointerToUObjectDerived = PointerToDerivedFrom<T, UObject>;

    template <class T>
    concept ReferenceToUObjectDerived = ReferenceToDerivedFrom<T, UObject>;

    template <class T>
    concept IInterface = static_cast<bool>(TIsIInterface<T>::Value);

    template <class T>
    concept PointerToIInterface = std::is_pointer_v<T> && IInterface<std::remove_pointer_t<T>>;

    template <class T>
    concept ReferenceToIInterface = std::is_reference_v<T> && IInterface<std::remove_reference_t<T>>;

    template <class T>
    concept UObjectDerivedOrIInterface = UObjectDerived<T> || IInterface<T>;

    template <class T>
    concept PointerToUObjectDerivedOrIInterface = std::is_pointer_v<T> && UObjectDerivedOrIInterface<std::remove_pointer_t<T>>;

    template <class T>
    concept ReferenceToUObjectDerivedOrIInterface = std::is_reference_v<T> && UObjectDerivedOrIInterface<std::remove_reference_t<T>>;
}
