// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

GAMECORE_API DECLARE_LOG_CATEGORY_EXTERN(LogGCPropertyWrapperNative, Log, All);

/**
 *
 */
template <class TPropertyType>
struct GAMECORE_API FGCPropertyWrapperNative
{
    FGCPropertyWrapperNative(TPropertyType defaultValue)
        : Value(defaultValue)
    {
    }

    /** Implements implicit conversion from this struct to Value's type. Allows you to treat this struct as its Value's type in code */
    operator TPropertyType() const
    {
        return Value;
    }

    // Broadcasts ValueChangeDelegate TODO: We should support other overloads as well.
    TPropertyType& operator=(const TPropertyType& newValue)
    {
        const TPropertyType oldValue = Value;
        Value = newValue;

        if (newValue != oldValue)
        {
            ValueChangeDelegate.Broadcast(oldValue, newValue);
        }

        return Value;
    }

    // Change event.
    TMulticastDelegate<void(const TPropertyType&, const TPropertyType&)> ValueChangeDelegate;

    TPropertyType Value;
};
