// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
template <class T>
struct TGCIsUObjectOrIInterface;

template <class T>
struct TGCIsUObjectOrIInterface
{
    enum
    {
        Value = TIsDerivedFrom<T, UObject>::Value || TIsIInterface<T>::Value
    };
};
