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

    /**
     * @brief Load an asset synchronously and assume the load is successful.
     * @todo Use shared ref for the `outStreamableHandle` somehow.
     * @todo Return casted object and use concepts.
     */
    template <bool shouldManageActiveHandle = true>
    UObject& LoadSyncChecked(
        FStreamableManager& inStreamableManager,
        FSoftObjectPath&& inAssetPath,
        TSharedPtr<FStreamableHandle>& outStreamableHandle = GCUtils::Materialize(TSharedPtr<FStreamableHandle>()));

    /**
     * @brief Load assets synchronously and assume the load is successful.
     * @todo Use shared ref for the `outStreamableHandle` somehow.
     * @todo Use `std::reference_wrapper<>` for the array of object pointers somehow.
     * @todo Return casted objects and use concepts.
     */
    template <class TAllocator, bool shouldManageActiveHandle = true>
    TArray<UObject*, TAllocator> LoadSyncChecked(
        FStreamableManager& inStreamableManager,
        TArray<FSoftObjectPath>&& inAssetPaths,
        TSharedPtr<FStreamableHandle>& outStreamableHandle = GCUtils::Materialize(TSharedPtr<FStreamableHandle>()));

    /**
     * @brief Load assets synchronously and assume the load is successful.
     * @todo Use shared ref for the `outStreamableHandle` somehow.
     */
    template <bool shouldManageActiveHandle = true>
    TSharedRef<FStreamableHandle> LoadSyncChecked(
        FStreamableManager& inStreamableManager,
        TArray<FSoftObjectPath>&& inAssetPaths);

    /**
     * @brief Load an asset synchronously.
     * @todo Return casted object and use concepts.
     */
    template <bool shouldManageActiveHandle = true, bool shouldReportErrors = true>
    UObject* LoadSync(
        FStreamableManager& inStreamableManager,
        FSoftObjectPath&& inAssetPath,
        TSharedPtr<FStreamableHandle>& outStreamableHandle = GCUtils::Materialize(TSharedPtr<FStreamableHandle>()));

    /**
     * @brief Load assets synchronously.
     * @todo Return casted objects and use concepts.
     */
    template <class TAllocator, bool shouldManageActiveHandle = true, bool shouldReportErrors = true>
    TArray<UObject*, TAllocator> LoadSync(
        FStreamableManager& inStreamableManager,
        TArray<FSoftObjectPath>&& inAssetPaths,
        TSharedPtr<FStreamableHandle>& outStreamableHandle = GCUtils::Materialize(TSharedPtr<FStreamableHandle>()));

    /**
     * @brief Load assets synchronously.
     */
    template <bool shouldManageActiveHandle = true, bool shouldReportErrors = true>
    TSharedPtr<FStreamableHandle> LoadSync(
        FStreamableManager& inStreamableManager,
        TArray<FSoftObjectPath>&& inAssetPaths);

    /**
     * @brief Iterate on all loaded assets with a callback function and break if returned false.
     * @note This function provides more information to the callback compared to the
     *       engine's `FStreamableHandle::ForEachLoadedAsset<>()` function.
     */
    GAMECORE_API void ForEachLoadedAssetBreakable(
        const TSharedRef<FStreamableHandle>& inStreamableHandle,
        const FForEachLoadedAssetBreakableCallbackFunctionRef& inCallback);

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
