// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * If given pointer, removes pointer. If given reference, removes reference. Does not remove both.
 */
template <class T>
struct TGCRemovePtrOrRef;

template <class T>
struct TGCRemovePtrOrRef
{
    using Type = T;
};

template <class T>
struct TGCRemovePtrOrRef<T&>
{
    using Type = T;
};

template <class T>
struct TGCRemovePtrOrRef<T&&>
{
    using Type = T;
};

template <class T>
struct TGCRemovePtrOrRef<T*>
{
    using Type = T;
};
