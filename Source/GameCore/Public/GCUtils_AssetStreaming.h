// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StreamableManager.h"
#include "Containers/Array.h"
#include "UObject/SoftObjectPath.h"
#include "Templates/SharedPointer.h"
#include "GCConcepts.h"
#include "GCUtils.h"
#include <functional>

struct FStreamableManager;

GAMECORE_API DECLARE_LOG_CATEGORY_EXTERN(LogGCUtils_AssetStreaming, Log, All);

GAMECORE_API const TCHAR* LexToString(const EAsyncPackageState::Type value);

/**
 * @brief Utilities for loading assets.
 */
namespace GCUtils::AssetStreaming
{
    using FForEachLoadedAssetBreakableCallbackFunctionRef =
        TFunctionRef<bool(UObject* /* loadedAsset */, const int32 /* index */, FStreamableHandle& /* streamableHandle */)>;
    using FForEachLoadedAssetBreakableConstCallbackFunctionRef =
        TFunctionRef<bool(UObject* /* loadedAsset */, const int32 /* index */, const FStreamableHandle& /* streamableHandle */)>;

    /**
     * @brief Load an asset synchronously and assume the load is successful.
     */
    template <bool shouldManageActiveHandle = true>
    TSharedRef<FStreamableHandle> LoadSyncChecked(
        FStreamableManager& inStreamableManager,
        FSoftObjectPath&& inAssetPath);

    /**
     * @brief Load assets synchronously and assume the load is successful.
     */
    template <bool shouldManageActiveHandle = true>
    TSharedRef<FStreamableHandle> LoadSyncChecked(
        FStreamableManager& inStreamableManager,
        TArray<FSoftObjectPath>&& inAssetPaths);

    /**
     * @brief Load an asset synchronously.
     */
    template <bool shouldManageActiveHandle = true, bool shouldReportErrors = true>
    TSharedPtr<FStreamableHandle> LoadSync(
        FStreamableManager& inStreamableManager,
        FSoftObjectPath&& inAssetPath);

    /**
     * @brief Load assets synchronously.
     */
    template <bool shouldManageActiveHandle = true, bool shouldReportErrors = true>
    TSharedPtr<FStreamableHandle> LoadSync(
        FStreamableManager& inStreamableManager,
        TArray<FSoftObjectPath>&& inAssetPaths);

    /**
     * @brief Get the loaded asset from a streamable handle.
     */
    template
        <
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    TAsset& GetLoadedAssetChecked(
        const TSharedRef<FStreamableHandle>& inStreamableHandle);

    /**
     * @brief Get all loaded assets from a streamable handle.
     */
    template
        <
        class TAllocator,
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    TArray<std::reference_wrapper<TAsset>, TAllocator> GetLoadedAssetsChecked(
        const TSharedRef<FStreamableHandle>& inStreamableHandle);

    /**
     * @brief Get the loaded asset from a streamable handle.
     */
    template
        <
        bool shouldReportErrors,
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    TAsset* GetLoadedAsset(
        const TSharedRef<FStreamableHandle>& inStreamableHandle);

    /**
     * @brief Get all loaded assets from a streamable handle.
     */
    template
        <
        class TAllocator,
        bool shouldReportErrors = true,
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    TArray<TAsset*, TAllocator> GetLoadedAssets(
        const TSharedRef<FStreamableHandle>& inStreamableHandle);

    /**
     * @brief Iterate on all loaded assets with a callback function and break if returned false.
     * @note This function provides more information to the callback compared to the
     *       engine's `FStreamableHandle::ForEachLoadedAsset<>()` function.
     */
    GAMECORE_API void ForEachLoadedAssetBreakable(
        const TSharedRef<FStreamableHandle>& inStreamableHandle,
        const FForEachLoadedAssetBreakableCallbackFunctionRef& inCallback);
    GAMECORE_API void ForEachLoadedAssetBreakable(
        const TSharedRef<const FStreamableHandle>& inStreamableHandle,
        const FForEachLoadedAssetBreakableConstCallbackFunctionRef& inCallback);

    /**
     * @brief Build a string of comma-separated asset paths.
     */
    template <int32 bufferSize = 1024, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, bufferSize> MakeStringFromAssetPaths(
        const TArrayView<const FSoftObjectPath>& inAssetPaths);

    /**
     * @brief Async load priority value for requests that are intended to block execution until complete.
     */
    constexpr TAsyncLoadPriority AsyncLoadPriorityBlocking = 1000;
}
