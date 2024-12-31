// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GCConcepts.h"

/**
 * @brief For the given UObject-derived type or IInterface type, get the necessary
 *        type which has a `UClass`. This is helpful for generic code that doesn't
 *        know whether a type is an IInterface or not. Overload resolution of template
 *        constraints will pass it off to the necessary specialization.
 */
template <GCConcepts::UObjectDerivedOrIInterface T>
struct TGCGetUClassType;

/**
 * @brief Specialization of `TGCGetUClassType<>` for UObject-derived types.
 */
template <GCConcepts::UObjectDerivedOrIInterface T>
    requires GCConcepts::UObjectDerived<T>
struct TGCGetUClassType<T>
{
public:

    using Type = T;
};

/**
 * @brief Specialization of `TGCGetUClassType<>` for IInterface types.
 */
template <GCConcepts::UObjectDerivedOrIInterface T>
    requires GCConcepts::IInterface<T>
struct TGCGetUClassType<T>
{
public:

    using Type = T::UClassType;
};

/**
 * @brief Convenient type-alias version of `TGCGetUClassType<>`.
 */
template <GCConcepts::UObjectDerivedOrIInterface T>
using TGCGetUClassType_T = TGCGetUClassType<T>::Type;
