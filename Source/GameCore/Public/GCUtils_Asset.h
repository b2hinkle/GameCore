// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string>

DECLARE_LOG_CATEGORY_EXTERN(LogGCUtils_Asset, Log, All);

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
    /**
     * @brief Convert the soft object ptr to another type and return null if not of that type.
     */
    template <class TTo, class TFrom>
    TSoftObjectPtr<TTo> CastSoftObjectPtr(const TSoftObjectPtr<TFrom>& objectSoft);

    /**
     * @brief Convert the soft class ptr to another type and return null if not child of that type.
     */
    template <class TTo, class TFrom>
    TSoftClassPtr<TTo> CastSoftClassPtr(const TSoftClassPtr<TFrom>& classSoft);

    template <class TTo, class TFrom>
    TSoftObjectPtr<TTo> ReinterpretSoftObjectPtr(const TSoftObjectPtr<TFrom>& objectSoft);

    template <class TTo, class TFrom>
    TSoftClassPtr<TTo> ReinterpretSoftClassPtr(const TSoftClassPtr<TFrom>& classSoft);

    template <class TTargetClass>
    bool SoftIsA(const FSoftObjectPath& objectPath);

    /**
     * @brief Determines whether an object is of the already-loaded target class without loading the object.
     */
    GAMECORE_API bool SoftIsA(const FSoftObjectPath& objectPath, const UClass* targetClass);

    /**
     * @brief Determines whether an already-loaded object is of the target class without loading the target class.
     */
    GAMECORE_API bool SoftIsA(const UObject* objectPtr, const FSoftObjectPath& targetClassPath);

    /**
     * @brief Determines whether an object is of the target class without loading them.
     */
    GAMECORE_API bool SoftIsA(const FSoftObjectPath& objectPath, const FSoftObjectPath& targetClassPath);

    template <class TTargetClass>
    bool SoftIsChildOf(FSoftObjectPath classPath);

    /**
     * @brief Determines whether a class is of the already-loaded target class without loading the class.
     */
    GAMECORE_API bool SoftIsChildOf(FSoftObjectPath classPath, const UClass* targetClass);

    /**
     * @brief Determines whether an already-loaded class is of the target class without loading the target class.
     */
    GAMECORE_API bool SoftIsChildOf(const UClass* classPtr, const FSoftObjectPath& targetClassPath);

    /**
     * @brief Determines whether a class is of the target class without loading them.
     */
    GAMECORE_API bool SoftIsChildOf(FSoftObjectPath classPath, const FSoftObjectPath& targetClassPath);

    /**
     * @brief Get on-disk asset data for an asset that we know isn't loaded. Warning logs if asset is in memory.
     *
     * @note Only use this if you've already tried finding the object in memory, as getting
     * asset data is generally more expensive.
     */
    GAMECORE_API FAssetData GetAssetDataForUnloadedAsset(const FSoftObjectPath& path);

    /**
     * @brief Resolve a class path. Error logs if the resolved object isn't a class.
     */
    GAMECORE_API UClass* ResolveClass(const FSoftObjectPath& classPath);

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
    constexpr FStringView BlueprintGeneratedClassPostfixString =
        FStringView(TEXT("_C"), std::char_traits<TCHAR>::length(TEXT("_C")));

    /**
     * @brief Convenient string view of the engine's DEFAULT_OBJECT_PREFIX cstring.
     */
    constexpr FStringView ClassDefaultObjectPrefixString =
        FStringView(DEFAULT_OBJECT_PREFIX, std::char_traits<TCHAR>::length(DEFAULT_OBJECT_PREFIX));
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

    // Copy the stored pointer if valid.
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

    // Copy the stored pointer if valid.
    if (const UClass* classPtr = classSoft.Get())
    {
        return classPtr;
    }

    return TSoftClassPtr<TTo>(classSoft.ToSoftObjectPath());
}

template <class TTargetClass>
bool GCUtils::Asset::SoftIsA(const FSoftObjectPath& objectPath)
{
    return SoftIsA(objectPath, TTargetClass::StaticClass());
}

template <class TTargetClass>
bool GCUtils::Asset::SoftIsChildOf(FSoftObjectPath classPath)
{
    return SoftIsChildOf(MoveTemp(classPath), TTargetClass::StaticClass());
}
