// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "type_traits"
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
    concept UObjectPointer = std::is_pointer_v<T> && std::is_base_of_v<std::remove_pointer_t<T>, UObject>;

    template <class T>
    concept UObjectReference = std::is_reference_v<T> && std::is_base_of_v<std::remove_reference_t<T>, UObject>;

    template <class T>
    concept IInterfacePointer = std::is_pointer_v<T> && TIsIInterface<std::remove_pointer_t<T>>::Value;

    template <class T>
    concept IInterfaceReference = std::is_reference_v<T> && TIsIInterface<std::remove_reference_t<T>>::Value;
}
