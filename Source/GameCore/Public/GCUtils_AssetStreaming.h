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
    template
        <
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    using TForEachLoadedAssetPointerBreakableCallbackFunctionRef =
        TFunctionRef<bool(TAsset* /* loadedAsset */, const int32 /* index */, FStreamableHandle& /* streamableHandle */)>;

    template
        <
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    using TForEachLoadedAssetPointerBreakableConstCallbackFunctionRef =
        TFunctionRef<bool(TAsset* /* loadedAsset */, const int32 /* index */, const FStreamableHandle& /* streamableHandle */)>;

    template
        <
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    using TForEachLoadedAssetReferenceBreakableCallbackFunctionRef =
        TFunctionRef<bool(TAsset& /* loadedAsset */, const int32 /* index */, FStreamableHandle& /* streamableHandle */)>;

    template
        <
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    using TForEachLoadedAssetReferenceBreakableConstCallbackFunctionRef =
        TFunctionRef<bool(TAsset& /* loadedAsset */, const int32 /* index */, const FStreamableHandle& /* streamableHandle */)>;

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
     * @brief Get all successfully-loaded assets from a streamable handle.
     */
    template
        <
        bool shouldReportErrors = true,
        class TAllocator,
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    TArray<std::reference_wrapper<TAsset>, TAllocator> GetSuccessfullyLoadedAssets(
        const TSharedRef<FStreamableHandle>& inStreamableHandle);

    /**
     * @brief Get the loaded asset from a streamable handle. The
     *        asset must be successfully loaded and of the correct type.
     */
    template
        <
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    TAsset& GetLoadedAssetChecked(
        const TSharedRef<FStreamableHandle>& inStreamableHandle);

    /**
     * @brief Get all loaded assets from a streamable handle. All
     *        assets must be successfully loaded and of the correct type.
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
        bool shouldReportErrors = true,
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
     * @brief Iterate on all loaded assets with a callback function and break if returned false. Only
     *        calls the callback for successfully loaded and casted assets.
     * @note This function provides more information to the callback compared to the
     *       engine's `FStreamableHandle::ForEachLoadedAsset<>()` function.
     */
    template
        <
        bool shouldReportErrors = true,
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    void ForEachSuccessfullyLoadedAsset(
        const TSharedRef<FStreamableHandle>& inStreamableHandle,
        const TForEachLoadedAssetReferenceBreakableCallbackFunctionRef<TAsset>& inCallback);
    template
        <
        bool shouldReportErrors = true,
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    void ForEachSuccessfullyLoadedAsset(
        const TSharedRef<const FStreamableHandle>& inStreamableHandle,
        const TForEachLoadedAssetReferenceBreakableConstCallbackFunctionRef<TAsset>& inCallback);

    /**
     * @brief Iterate on all loaded assets with a callback function and break if returned false. All
     *        assets must be successfully loaded and of the correct type.
     * @note This function provides more information to the callback compared to the
     *       engine's `FStreamableHandle::ForEachLoadedAsset<>()` function.
     */
    template
        <
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    void ForEachLoadedAssetChecked(
        const TSharedRef<FStreamableHandle>& inStreamableHandle,
        const TForEachLoadedAssetReferenceBreakableCallbackFunctionRef<TAsset>& inCallback);
    template
        <
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    void ForEachLoadedAssetChecked(
        const TSharedRef<const FStreamableHandle>& inStreamableHandle,
        const TForEachLoadedAssetReferenceBreakableConstCallbackFunctionRef<TAsset>& inCallback);

    /**
     * @brief Iterate on all loaded assets with a callback function and break if returned false.
     * @note This function provides more information to the callback compared to the
     *       engine's `FStreamableHandle::ForEachLoadedAsset<>()` function.
     */
    template
        <
        bool shouldReportErrors = true,
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    void ForEachLoadedAsset(
        const TSharedRef<FStreamableHandle>& inStreamableHandle,
        const TForEachLoadedAssetPointerBreakableCallbackFunctionRef<TAsset>& inCallback);
    template
        <
        bool shouldReportErrors = true,
        GCConcepts::UObjectDerivedOrIInterface TAsset = UObject
        >
    void ForEachLoadedAsset(
        const TSharedRef<const FStreamableHandle>& inStreamableHandle,
        const TForEachLoadedAssetPointerBreakableConstCallbackFunctionRef<TAsset>& inCallback);

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
