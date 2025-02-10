// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCConcepts.h"

namespace GCUtils
{
    /**
     * Get the "owning" controller for an object.
     */
    GAMECORE_API const AController* GetController(const UObject* inObject);
    GAMECORE_API AController* GetController(UObject* inObject);

    /**
     * @brief Materialize a prvalue to a temporary and get an lvalue reference to it.
     * @tparam T Type of temporary to materialize. Automatically deduced but may be specified
     *         manually in order to implicitly convert the prvalue to another type.
     * @param inTemporary The caller's prvalue argument materialized to temporary.
     * @return Lvalue reference to the temporary.
     */
    template <GCConcepts::NonLvalueReference T>
    T& Materialize(T&& inTemporary);
}

template <GCConcepts::NonLvalueReference T>
T& GCUtils::Materialize(T&& inTemporary)
{
    // Note that the temporary gets promoted to an lvalue for the scope of this function. It
    // will revert to its original lifetime of temporary when this function is exited.
    return static_cast<T&>(inTemporary);
}
