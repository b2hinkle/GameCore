// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGCUtils_Asset, Log, All);

/**
 * @brief Utilities for sofly casting paths and more.
 * @remark We pass around class paths as FSoftObjectPath because it's the most
 * supported and simplified form of an asset path. I would love to use FSoftClassPath but
 * it's just not as good. We runtime assert paths that are intended to be class paths and
 * error log if they're not.
 */
namespace GCUtils::Asset
{
    /**
     * @brief Convert the soft object ptr to another type and return null if not of that type.
     * @note This is still expensive. The function may resolve object paths, get on-disk asset data,
     *     and get weak object ptrs. It may be preferable for you, the caller, to perform these
     *     operations yourself so you can reuse that data, or just completely load the asset if you
     *     were planning to do that anyway.
     */
    template <class TTo, class TFrom>
    TSoftObjectPtr<TTo> CastSoftObjectPtr(const TSoftObjectPtr<TFrom>& objectSoft);

    /**
     * @brief Convert the soft class ptr to another type and return null if not child of that type.
     * @note This is still expensive. The function may resolve object paths, get on-disk asset data,
     *     and get weak object ptrs. It may be preferable for you, the caller, to perform these
     *     operations yourself so you can reuse that data, or just completely load the asset if you
     *     were planning to do that anyway.
     */
    template <class TTo, class TFrom>
    TSoftClassPtr<TTo> CastSoftClassPtr(const TSoftClassPtr<TFrom>& classSoft);

    /**
     * @brief Copy the data from one soft object ptr to another of a different type.
     * @note Consider using CastSoftObjectPtr for type safety.
     */
    template <class TTo, class TFrom>
    TSoftObjectPtr<TTo> ReinterpretSoftObjectPtr(const TSoftObjectPtr<TFrom>& objectSoft);

    /**
     * @brief Copy the data from one soft class ptr to another of a different type.
     * @note Consider using CastSoftClassPtr for type safety.
     */
    template <class TTo, class TFrom>
    TSoftClassPtr<TTo> ReinterpretSoftClassPtr(const TSoftClassPtr<TFrom>& classSoft);

    /**
     * @brief Determines whether an object is of the target class without loading.
     * @note This is still expensive. The function may resolve object paths and get on-disk asset
     *     data. It may be preferable for you, the caller, to perform these operations yourself so you
     *     can reuse that data, or just completely load the asset if you were planning to do that anyway.
     * @tparam TTargetClass Native target class type.
     * @param objectPath Unresolved or unloaded object path.
     */
    template <class TTargetClass>
    bool SoftIsA(const FSoftObjectPath& objectPath);

    /**
     * @brief Determines whether an object is of the target class without loading.
     * @note This is still expensive. The function may resolve object paths and get on-disk asset
     *     data. It may be preferable for you, the caller, to perform these operations yourself so you
     *     can reuse that data, or just completely load the asset if you were planning to do that anyway.
     * @param objectPath Unresolved or unloaded object path.
     * @param targetClass Loaded target class.
     */
    GAMECORE_API bool SoftIsA(const FSoftObjectPath& objectPath, const UClass* targetClass);

    /**
     * @brief Determines whether an object is of the target class without loading.
     * @note This is still expensive. The function may resolve object paths and get on-disk asset
     *     data. It may be preferable for you, the caller, to perform these operations yourself so you
     *     can reuse that data, or just completely load the asset if you were planning to do that anyway.
     * @param objectPtr Loaded object.
     * @param targetClassPath Unresolved or unloaded target class.
     */
    GAMECORE_API bool SoftIsA(const UObject* objectPtr, const FSoftObjectPath& targetClassPath);

    /**
     * @brief Determines whether an object is of the target class without loading.
     * @note This is still expensive. The function may resolve object paths and get on-disk asset
     *     data. It may be preferable for you, the caller, to perform these operations yourself so you
     *     can reuse that data, or just completely load the asset if you were planning to do that anyway.
     * @param objectPath Unresolved or unloaded object.
     * @param targetClassPath Unresolved or unloaded target class.
     */
    GAMECORE_API bool SoftIsA(const FSoftObjectPath& objectPath, const FSoftObjectPath& targetClassPath);

    /**
     * @brief Determines whether a class is a child of the target class without loading.
     * @note This is still expensive. The function may resolve object paths and get on-disk asset
     *     data. It may be preferable for you, the caller, to perform these operations yourself so you
     *     can reuse that data, or just completely load the asset if you were planning to do that anyway.
     * @tparam TTargetClass Native target class type.
     * @param classPath Unresolved or unloaded class path.
     */
    template <class TTargetClass>
    bool SoftIsChildOf(FSoftObjectPath classPath);

    /**
     * @brief Determines whether a class is a child of the target class without loading.
     * @note This is still expensive. The function may resolve object paths and get on-disk asset
     *     data. It may be preferable for you, the caller, to perform these operations yourself so you
     *     can reuse that data, or just completely load the asset if you were planning to do that anyway.
     * @param classPath Unresolved or unloaded class path.
     * @param targetClass Loaded target class.
     */
    GAMECORE_API bool SoftIsChildOf(FSoftObjectPath classPath, const UClass* targetClass);

    /**
     * @brief Determines whether a class is a child of the target class without loading.
     * @note This is still expensive. The function may resolve object paths and get on-disk asset
     *     data. It may be preferable for you, the caller, to perform these operations yourself so you
     *     can reuse that data, or just completely load the asset if you were planning to do that anyway.
     * @param classPtr Loaded class path.
     * @param targetClassPath Unresolved or unloaded target class path.
     */
    GAMECORE_API bool SoftIsChildOf(const UClass* classPtr, const FSoftObjectPath& targetClassPath);

    /**
     * @brief Determines whether a class is a child of the target class without loading.
     * @note This is still expensive. The function may resolve object paths and get on-disk asset
     *     data. It may be preferable for you, the caller, to perform these operations yourself so you
     *     can reuse that data, or just completely load the asset if you were planning to do that anyway.
     * @param classPath Unresolved or unloaded class path.
     * @param targetClassPath Unresolved or unloaded target class path.
     */
    GAMECORE_API bool SoftIsChildOf(FSoftObjectPath classPath, const FSoftObjectPath& targetClassPath);

    /**
     * @brief Get on-disk asset data for an asset that we know isn't loaded and warn if it is in memory.
     */
    GAMECORE_API FAssetData GetAssetByObjectPathUnloaded(const FSoftObjectPath& path);

    /**
     * @brief Resolve a class path. Error logs if the path and object aren't a class.
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
    GAMECORE_API bool IsBlueprintGeneratedClassName(const FName& name);

    /** @brief Determines whether an object name may be a class default object. */
    GAMECORE_API bool IsClassDefaultObjectName(const FName& name);

    /**
     * @brief Postfix found at the end of blueprint generated class asset names. The engine
     * provides no constant for this so we have our own here.
     */
    constexpr FStringView BlueprintGeneratedClassPostfixString = TEXTVIEW("_C");

    /**
     * @brief Convenient string view of the engine's DEFAULT_OBJECT_PREFIX string literal.
     */
    constexpr FStringView ClassDefaultObjectPrefixString = PREPROCESSOR_JOIN(DEFAULT_OBJECT_PREFIX, _PrivateSV);
}

template <class TTo, class TFrom>
TSoftObjectPtr<TTo> GCUtils::Asset::CastSoftObjectPtr(const TSoftObjectPtr<TFrom>& objectSoft)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::CastSoftObjectPtr);

    if (SoftIsA<TTo>(objectSoft.ToSoftObjectPath()))
    {
        return ReinterpretSoftObjectPtr<TTo>(objectSoft);
    }

    return TSoftObjectPtr<TTo>(nullptr);
}

template <class TTo, class TFrom>
TSoftClassPtr<TTo> GCUtils::Asset::CastSoftClassPtr(const TSoftClassPtr<TFrom>& classSoft)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::CastSoftClassPtr);

    if (SoftIsChildOf<TTo>(classSoft.ToSoftObjectPath()))
    {
        return ReinterpretSoftClassPtr<TTo>(classSoft);
    }

    return TSoftClassPtr<TTo>(nullptr);
}

template <class TTo, class TFrom>
TSoftObjectPtr<TTo> GCUtils::Asset::ReinterpretSoftObjectPtr(const TSoftObjectPtr<TFrom>& objectSoft)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::ReinterpretSoftObjectPtr);

    // Copy the stored pointer if valid.
    if (const UObject* objectPtr = objectSoft.Get())
    {
        return TSoftObjectPtr<TTo>(objectPtr);
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
        return TSoftClassPtr<TTo>(classPtr);
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
