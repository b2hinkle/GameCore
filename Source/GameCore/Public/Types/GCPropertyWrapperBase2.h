// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Kismet/KismetSystemLibrary.h"

GAMECORE_API DECLARE_LOG_CATEGORY_EXTERN(LogGCPropertyWrapperBase2, Log, All);

/**
 * Expected to implement specializations of this type for wrapper structs.
 */
template <class T>
struct TGCPropertyWrapperValueTypeTrait;

/**
 * Convenient typedef for getting the value type of a certain property wrapper.
 */
template <class T>
using TGCPropertyWrapperValueTypeTrait_T = typename TGCPropertyWrapperValueTypeTrait<T>::FValueType;

/**
 * Property wrapper implementation.
 */
template <class TWrapperType>
struct TGCPropertyWrapperBase2
{

private:

    using FValueType = TGCPropertyWrapperValueTypeTrait_T<TWrapperType>;

public:

    // Provide default ctr to support potential UStructs users. Automatically use type's default value.
    TGCPropertyWrapperBase2()
        : TGCPropertyWrapperBase2(FValueType{})
    {
    }

    // TODO: We should support other overloads as well.
    TGCPropertyWrapperBase2(FValueType defaultValue)
    {
        GetValue() = defaultValue;
    }

    // CRTP usage here.
    FORCEINLINE TWrapperType& GetWrapper()
    {
        return static_cast<TWrapperType&>(*this);
    }

    FORCEINLINE FValueType& GetValue()
    {
        return GetWrapper().Value;
    }

    // Implements implicit conversion from this struct to Value's type. Allows you to treat this struct as its Value's type in code.
    operator FValueType() const
    {
        return GetValue();
    }

    // Broadcasts ValueChangeDelegate TODO: We should support other overloads as well.
    // TODO: It would be nice to expose ability to broadcast even when value is same.
    FValueType& operator=(const FValueType& newValue)
    {
        const FValueType oldValue = GetValue();
        GetValue() = newValue;

        if (newValue != oldValue)
        {
            ValueChangeDelegate.Broadcast(oldValue, newValue);
        }

        return GetValue();
    }

    // Change event.
    TMulticastDelegate<void(const FValueType&, const FValueType&)> ValueChangeDelegate;


};
