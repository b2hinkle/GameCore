// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Check if T is a pointer to TPointedType.
 * 
 * NOTE: Only for raw pointer types.
 */
template <class T, class TPointedType>
struct TGCIsPointerTo
{
    enum
    {
        Value = TAnd<TIsPointer<T>, TIsSame<TRemovePointer<T>::Type, TPointedType>>::Value
    };
};

/**
 * Check if T is a pointer to TPointedType.
 * 
 * Accounts for UObject pointer wrappers.
 */
template <class T>
struct TGCIsObjectPointer
{
    enum
    {
        Value = 0
    };
};

// NOTE: There is probably a better way to implement these specializations without worrying
// about the const qualifier - maybe by using something like TRemoveCV<>.

template <class T> struct TGCIsObjectPointer<T*>                      { enum { Value = 1 }; };
template <class T> struct TGCIsObjectPointer<T* const>                { enum { Value = 1 }; };

template <class T> struct TGCIsObjectPointer<TObjectPtr<T>>           { enum { Value = 1 }; };
template <class T> struct TGCIsObjectPointer<const TObjectPtr<T>>     { enum { Value = 1 }; };

template <class T> struct TGCIsObjectPointer<TWeakObjectPtr<T>>       { enum { Value = 1 }; };
template <class T> struct TGCIsObjectPointer<const TWeakObjectPtr<T>> { enum { Value = 1 }; };

/**
 * Remove pointer from UObject T.
 * 
 * Accounts for UObject pointer wrappers.
 */
template <class T>
struct TGCRemoveObjectPointer
{
    using Type = T;
};

// NOTE: There is probably a better way to implement these specializations without worrying
// about the const qualifier - maybe by using something like TRemoveCV<>.

template <class T> struct TGCRemoveObjectPointer<T*>                      { using Type = T; };
template <class T> struct TGCRemoveObjectPointer<T* const>                { using Type = T; };

template <class T> struct TGCRemoveObjectPointer<TObjectPtr<T>>           { using Type = T; };
template <class T> struct TGCRemoveObjectPointer<const TObjectPtr<T>>     { using Type = T; };

template <class T> struct TGCRemoveObjectPointer<TWeakObjectPtr<T>>       { using Type = T; };
template <class T> struct TGCRemoveObjectPointer<const TWeakObjectPtr<T>> { using Type = T; };

/**
 * Check if T is a pointer to TPointedType.
 * 
 * Accounts for UObject pointer wrappers.
 */
template <class T, class TPointedType>
struct TGCIsObjectPointerTo
{
    enum
    {
        Value = TAnd<TGCIsObjectPointer<T>, TIsSame<TGCRemoveObjectPointer<T>::Type, TPointedType>>::Value
    };
};
