// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <type_traits>
#include "Templates/Casts.h"

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
    concept UObjectDerived = std::is_base_of_v<UObject, T>;

    template <class T>
    concept IInterface = static_cast<bool>(TIsIInterface<T>::Value);

    template <class T>
    concept UObjectOrIInterface = UObjectDerived<T> || IInterface<T>;

    template <class T>
    concept UObjectPointer = std::is_pointer_v<T> && UObjectDerived<std::remove_pointer_t<T>>;

    template <class T>
    concept UObjectReference = std::is_reference_v<T> && UObjectDerived<std::remove_reference_t<T>>;

    template <class T>
    concept IInterfacePointer = std::is_pointer_v<T> && IInterface<std::remove_pointer_t<T>>;

    template <class T>
    concept IInterfaceReference = std::is_reference_v<T> && IInterface<std::remove_reference_t<T>>;

    template <class T>
    concept UObjectOrIInterfacePointer = std::is_pointer_v<T> && UObjectOrIInterface<std::remove_pointer_t<T>>;

    template <class T>
    concept UObjectOrIInterfaceReference = std::is_reference_v<T> && UObjectOrIInterface<std::remove_reference_t<T>>;
}
