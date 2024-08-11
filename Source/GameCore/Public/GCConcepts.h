// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "type_traits"

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
}
