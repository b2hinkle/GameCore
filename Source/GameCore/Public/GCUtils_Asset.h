// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
namespace GCUtils::Asset
{
    template <class TTo, class TFrom>
    TSoftObjectPtr<TTo> CastSoftObjectPtr(const TSoftObjectPtr<TFrom>& objectSoft);

    template <class TTo, class TFrom>
    TSoftClassPtr<TTo> CastSoftClassPtr(const TSoftClassPtr<TFrom>& classSoft);

    template <class TTo, class TFrom>
    TSoftObjectPtr<TTo> ReinterpretSoftObjectPtr(const TSoftObjectPtr<TFrom>& objectSoft);

    template <class TTo, class TFrom>
    TSoftClassPtr<TTo> ReinterpretSoftClassPtr(const TSoftClassPtr<TFrom>& classSoft);

    template <class TTargetClass>
    bool SoftIsA(FSoftObjectPath objectPath);

    GAMECORE_API bool SoftIsA(FSoftObjectPath objectPath, const UClass* targetClass);

    /**
     * @brief Determines whether an object is of the target class without loading the object nor the class.
     */
    GAMECORE_API bool SoftIsA(FSoftObjectPath objectPath, const FSoftClassPath& targetClassPath);

    template <class TTargetClass>
    bool SoftIsChildOf(FSoftClassPath classPath);

    GAMECORE_API bool SoftIsChildOf(FSoftClassPath classPath, const UClass* targetClass);

    /**
     * @brief Determines whether a class is of the target class without loading either of the classes.
     */
    GAMECORE_API bool SoftIsChildOf(FSoftClassPath classPath, const FSoftClassPath& targetClassPath);

    GAMECORE_API FSoftClassPath SoftGetSuperClass(const FSoftClassPath& classPath);

    GAMECORE_API FSoftClassPath SoftGetClass(const FSoftObjectPath& objectPath);

    GAMECORE_API FSoftObjectPath MakeSoftObjectPathForSoftClassPath(FSoftClassPath classPath);

    GAMECORE_API FSoftClassPath MakeSoftClassPathForSoftObjectPath(const FSoftObjectPath& objectPath);
}

template <class TTo, class TFrom>
TSoftObjectPtr<TTo> GCUtils::Asset::CastSoftObjectPtr(const TSoftObjectPtr<TFrom>& objectSoft)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::CastSoftObjectPtr);

    if (SoftIsA<TTo>(objectSoft.ToSoftObjectPath()))
    {
        return ReinterpretSoftObjectPtr<TTo>(objectSoft);
    }

    return nullptr;
}

template <class TTo, class TFrom>
TSoftClassPtr<TTo> GCUtils::Asset::CastSoftClassPtr(const TSoftClassPtr<TFrom>& classSoft)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::CastSoftClassPtr);

    if (SoftIsChildOf<TTo>(MakeSoftClassPathForSoftObjectPath(classSoft.ToSoftObjectPath())))
    {
        return ReinterpretSoftClassPtr<TTo>(classSoft);
    }

    return nullptr;
}

template <class TTo, class TFrom>
TSoftObjectPtr<TTo> GCUtils::Asset::ReinterpretSoftObjectPtr(const TSoftObjectPtr<TFrom>& objectSoft)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::ReinterpretSoftObjectPtr);

    // Copy the pointer if valid.
    if (const UObject* objectPtr = objectSoft.Get())
    {
        return objectPtr;
    }

    return TSoftObjectPtr<TTo>(objectSoft.ToSoftObjectPath());
}

template <class TTo, class TFrom>
TSoftClassPtr<TTo> GCUtils::Asset::ReinterpretSoftClassPtr(const TSoftClassPtr<TFrom>& classSoft)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::ReinterpretSoftClassPtr);

    // Copy the pointer if valid.
    if (const UClass* classPtr = classSoft.Get())
    {
        return classPtr;
    }

    return TSoftClassPtr<TTo>(classSoft.ToSoftObjectPath());
}

template <class TTargetClass>
bool GCUtils::Asset::SoftIsA(FSoftObjectPath objectPath)
{
    return SoftIsA(MoveTemp(objectPath), TTargetClass::StaticClass());
}

template <class TTargetClass>
bool GCUtils::Asset::SoftIsChildOf(FSoftClassPath classPath)
{
    return SoftIsChildOf(MoveTemp(classPath), TTargetClass::StaticClass());
}
