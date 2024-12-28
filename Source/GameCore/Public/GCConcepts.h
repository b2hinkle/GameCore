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
    
    /**
     * @brief Concept is satisfied if T is a pointer type to a TBase-derrived type.
     * @tparam T The type to test.
     * @tparam TBase The type which T must derive from.
     */
    template <class T, class TBase>
    concept PointerToDerived = std::is_pointer_v<T> && std::derived_from<std::remove_pointer_t<T>, TBase>;

    /**
     * @brief Concept is satisfied if T is a reference type to a TBase-derrived type.
     * @tparam T The type to test.
     * @tparam TBase The type which T must derive from.
     */
    template <class T, class TBase>
    concept ReferenceToDerived = std::is_reference_v<T> && std::derived_from<std::remove_reference_t<T>, TBase>;
    
    /**
     * @brief Concept is satisfied if T is a UObject-derrived type.
     * @tparam T The type to test.
     */
    template <class T>
    concept UObjectDerived = std::derived_from<T, UObject>;
    
    /**
     * @brief Concept is satisfied if T is a pointer type to a UObject-derived type.
     * @tparam T The type to test.
     */
    template <class T>
    concept PointerToUObjectDerived = PointerToDerived<T, UObject>;
    
    /**
     * @brief Concept is satisfied if T is a reference type to a UObject-derived type.
     * @tparam T The type to test.
     */
    template <class T>
    concept ReferenceToUObjectDerived = ReferenceToDerived<T, UObject>;
    
    /**
     * @brief Concept is satisfied if T is an IInterface type.
     * @tparam T The type to test.
     */
    template <class T>
    concept IInterface = static_cast<bool>(TIsIInterface<T>::Value);
    
    /**
     * @brief Concept is satisfied if T is a pointer type to an IInterface type.
     * @tparam T The type to test.
     */
    template <class T>
    concept PointerToIInterface = std::is_pointer_v<T> && IInterface<std::remove_pointer_t<T>>;
    
    /**
     * @brief Concept is satisfied if T is a reference type to an IInterface type.
     * @tparam T The type to test.
     */
    template <class T>
    concept ReferenceToIInterface = std::is_reference_v<T> && IInterface<std::remove_reference_t<T>>;
    
    /**
     * @brief Concept is satisfied if T is a UObject-derrived type or an IInterface type.
     * @tparam T The type to test.
     */
    template <class T>
    concept UObjectDerivedOrIInterface = UObjectDerived<T> || IInterface<T>;
    
    /**
     * @brief Concept is satisfied if T is a pointer type to a UObject-derrived type or an IInterface type.
     * @tparam T The type to test.
     */
    template <class T>
    concept PointerToUObjectDerivedOrIInterface = std::is_pointer_v<T> && UObjectDerivedOrIInterface<std::remove_pointer_t<T>>;
    
    /**
     * @brief Concept is satisfied if T is a reference type to a UObject-derrived type or an IInterface type.
     * @tparam T The type to test.
     */
    template <class T>
    concept ReferenceToUObjectDerivedOrIInterface = std::is_reference_v<T> && UObjectDerivedOrIInterface<std::remove_reference_t<T>>;
}
