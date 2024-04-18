// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * @brief Utilities for sofly casting paths and more.
 *
 * @remark We pass around class paths as FSoftObjectPath because it's the most
 * supported and simplified form of an asset path. I would love to use FSoftClassPath but
 * it's just not as good. We runtime assert paths that are intended to be class paths
 * for safety.
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
    GAMECORE_API bool SoftIsA(FSoftObjectPath objectPath, const FSoftObjectPath& targetClassPath);

    template <class TTargetClass>
    bool SoftIsChildOf(FSoftObjectPath classPath);

    GAMECORE_API bool SoftIsChildOf(FSoftObjectPath classPath, const UClass* targetClass);

    /**
     * @brief Determines whether a class is of the target class without loading either of the classes.
     */
    GAMECORE_API bool SoftIsChildOf(FSoftObjectPath classPath, const FSoftObjectPath& targetClassPath);

    GAMECORE_API FSoftObjectPath SoftGetSuperClass(const FSoftObjectPath& classPath);

    GAMECORE_API FSoftObjectPath SoftGetClass(const FSoftObjectPath& objectPath);

    /**
     * @brief Determines whether the path is a path to a UClass (including bp-generated).
     */
    GAMECORE_API bool IsClassPath(const FSoftObjectPath& path);

    /**
     * @brief Determines whether the path is a path to a native UClass (excluding bp-generated).
     */
    GAMECORE_API bool IsNativeClassPath(const FSoftObjectPath& path);

    /**
     * @brief Determines whether the path is a path to a UBlueprintGeneratedClass.
     */
    GAMECORE_API bool IsBlueprintGeneratedClassPath(const FSoftObjectPath& path);

    /** @brief Determines whether an object name may be a UBlueprintGeneratedClass. */
    GAMECORE_API bool IsBlueprintGeneratedClassName(const FStringView& nameString);

    /** @brief Determines whether an object name may be a class default object. */
    GAMECORE_API bool IsClassDefaultObjectName(const FStringView& nameString);

    /**
     * @brief Postfix found at the end of blueprint generated class asset names. The engine
     * provides no constant for this so we have our own here.
     */
    constexpr TCHAR BlueprintGeneratedClassPostfix[] = TEXT("_C");
    constexpr int32 BlueprintGeneratedClassPostfixLen = (sizeof(BlueprintGeneratedClassPostfix) / sizeof(TCHAR)) - 1;
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

    if (SoftIsChildOf<TTo>(classSoft.ToSoftObjectPath()))
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
bool GCUtils::Asset::SoftIsChildOf(FSoftObjectPath classPath)
{
    return SoftIsChildOf(MoveTemp(classPath), TTargetClass::StaticClass());
}
