// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GCUtils_AssetStreaming.h"

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GCUtils_Log.h"
#include "GCUtils_String.h"

namespace GCUtils::AssetStreaming::Private
{
    template
        <
        class TAllocator,
        bool shouldManageActiveHandle,
        bool shouldReportErrors,
        bool shouldAssumeSuccess
        >
    TArray<UObject*, TAllocator> LoadSyncGeneralized(
        FStreamableManager& inStreamableManager,
        TArray<FSoftObjectPath>&& inAssetPaths,
        TSharedPtr<FStreamableHandle>& outStreamableHandle);

    template
        <
        class TAllocator,
        bool shouldReportErrors,
        bool shouldAssumeSuccess
        >
    TArray<UObject*, TAllocator> GetLoadedAssetsGeneralized(
        const TSharedRef<FStreamableHandle>& streamableHandle);

    template
        <
        bool shouldManageActiveHandle,
        bool shouldReportErrors,
        bool shouldAssumeSuccess
        >
    TSharedPtr<FStreamableHandle> LoadSyncGeneralized(
        FStreamableManager& inStreamableManager,
        TArray<FSoftObjectPath>&& inAssetPaths);
}

template <bool shouldManageActiveHandle>
UObject& GCUtils::AssetStreaming::LoadSyncChecked(
    FStreamableManager& inStreamableManager,
    FSoftObjectPath&& inAssetPath,
    TSharedPtr<FStreamableHandle>& outStreamableHandle)
{
    using TAllocator = TFixedAllocator<1u>;

    TArray<FSoftObjectPath> assetPathArray;
    assetPathArray.Emplace(MoveTemp(inAssetPath));

    TArray<UObject*, TAllocator> loadedAssetArray =
        LoadSyncChecked<TAllocator, shouldManageActiveHandle>(inStreamableManager, MoveTemp(assetPathArray));

    return *loadedAssetArray[0];
}

template <class TAllocator, bool shouldManageActiveHandle>
TArray<UObject*, TAllocator> GCUtils::AssetStreaming::LoadSyncChecked(
    FStreamableManager& inStreamableManager,
    TArray<FSoftObjectPath>&& inAssetPaths,
    TSharedPtr<FStreamableHandle>& outStreamableHandle)
{
    constexpr bool shouldReportErrors = true;
    constexpr bool shouldAssumeSuccess = true;
    return Private::LoadSyncGeneralized<TAllocator, shouldManageActiveHandle, shouldReportErrors, shouldAssumeSuccess>(
        inStreamableManager,
        MoveTemp(inAssetPaths),
        outStreamableHandle
        );
}

template <bool shouldManageActiveHandle>
TSharedRef<FStreamableHandle> GCUtils::AssetStreaming::LoadSyncChecked(
    FStreamableManager& inStreamableManager,
    TArray<FSoftObjectPath>&& inAssetPaths)
{
    constexpr bool shouldReportErrors = true;
    constexpr bool shouldAssumeSuccess = true;
    return Private::LoadSyncGeneralized<shouldManageActiveHandle, shouldReportErrors, shouldAssumeSuccess>(
        inStreamableManager,
        MoveTemp(inAssetPaths)
        ).ToSharedRef();
}

template <bool shouldManageActiveHandle, bool shouldReportErrors>
UObject* GCUtils::AssetStreaming::LoadSync(
    FStreamableManager& inStreamableManager,
    FSoftObjectPath&& inAssetPath,
    TSharedPtr<FStreamableHandle>& outStreamableHandle)
{
    using TAllocator = TFixedAllocator<1u>;

    TArray<FSoftObjectPath> assetPathArray;
    assetPathArray.Emplace(MoveTemp(inAssetPath));

    TArray<UObject*, TAllocator> loadedAssetArray =
        LoadSync<TAllocator, shouldManageActiveHandle, shouldReportErrors>(inStreamableManager, MoveTemp(assetPathArray));

    return loadedAssetArray[0];
}

template <class TAllocator, bool shouldManageActiveHandle, bool shouldReportErrors>
TArray<UObject*, TAllocator> GCUtils::AssetStreaming::LoadSync(
    FStreamableManager& inStreamableManager,
    TArray<FSoftObjectPath>&& inAssetPaths,
    TSharedPtr<FStreamableHandle>& outStreamableHandle)
{
    constexpr bool shouldAssumeSuccess = false;
    return Private::LoadSyncGeneralized<TAllocator, shouldManageActiveHandle, shouldReportErrors, shouldAssumeSuccess>(
        inStreamableManager,
        MoveTemp(inAssetPaths),
        outStreamableHandle
        );
}

template <bool shouldManageActiveHandle, bool shouldReportErrors>
TSharedPtr<FStreamableHandle> GCUtils::AssetStreaming::LoadSync(
    FStreamableManager& inStreamableManager,
    TArray<FSoftObjectPath>&& inAssetPaths)
{
    constexpr bool shouldAssumeSuccess = false;
    return Private::LoadSyncGeneralized<shouldManageActiveHandle, shouldReportErrors, shouldAssumeSuccess>(
        inStreamableManager,
        MoveTemp(inAssetPaths)
        );
}

template <int32 bufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, bufferSize> GCUtils::AssetStreaming::MakeStringFromAssetPaths(
    const TArrayView<const FSoftObjectPath>& inAssetPaths)
{
    return
        GCUtils::String::ConstructStringBuilder<bufferSize, TCharType>(
            [&inAssetPaths](TStringBuilderBase<TCharType>& stringBuilder)
            {
                for (int32 i = 0; i < inAssetPaths.Num(); ++i)
                {
                    // Append this path.
                    stringBuilder << inAssetPaths[i];

                    // Conditionally append a delimiting comma.
                    if (i < inAssetPaths.Num() - 1)
                    {
                        stringBuilder << TEXT(',');
                    }
                }
            }
            );
}

template
    <
    class TAllocator,
    bool shouldManageActiveHandle,
    bool shouldReportErrors,
    bool shouldAssumeSuccess
    >
TArray<UObject*, TAllocator> GCUtils::AssetStreaming::Private::LoadSyncGeneralized(
    FStreamableManager& inStreamableManager,
    TArray<FSoftObjectPath>&& inAssetPaths,
    TSharedPtr<FStreamableHandle>& outStreamableHandle)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::AssetStreaming::Private::LoadSyncGeneralized(FStreamableManager&, TArray<FSoftObjectPath>&&, TSharedPtr<FStreamableHandle>&));

#if !NO_LOGGING || DO_ENSURE
    if (!ensure(!outStreamableHandle))
    {
        GC_LOG_STR_NO_CONTEXT(
            LogGCUtils_AssetStreaming,
            Warning,
            TEXT("The out parameter has been passed an existing streamable handle. Improper usage of function!")
            );
    }
#endif // #if !NO_LOGGING || DO_ENSURE

    outStreamableHandle =
        LoadSyncGeneralized<shouldManageActiveHandle, shouldReportErrors, shouldAssumeSuccess>(
            inStreamableManager,
            MoveTemp(inAssetPaths)
            );

    if constexpr (!shouldAssumeSuccess)
    {
        if (!outStreamableHandle)
        {
            return {};
        }
    }

    check(outStreamableHandle);
    return GetLoadedAssetsGeneralized<TAllocator, shouldReportErrors, shouldAssumeSuccess>(outStreamableHandle.ToSharedRef());
}

template
    <
    class TAllocator,
    bool shouldReportErrors,
    bool shouldAssumeSuccess
    >
TArray<UObject*, TAllocator> GCUtils::AssetStreaming::Private::GetLoadedAssetsGeneralized(
    const TSharedRef<FStreamableHandle>& streamableHandle)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::AssetStreaming::Private::GetLoadedAssetsGeneralized(const TSharedRef<FStreamableHandle>&));

    TArray<UObject*, TAllocator> loadedAssets;

    streamableHandle->ForEachLoadedAsset(
        [&loadedAssets, &streamableHandle, iteration = 0](UObject* loadedAsset) mutable
        {
            ON_SCOPE_EXIT
            {
                ++iteration;
            };

#if !NO_LOGGING || DO_ENSURE
            if (!loadedAsset)
            {
                const FStringBuilderBase& logString =
                    GCUtils::Materialize(TStringBuilder<512>())
                        << TEXT("Loaded asset is null.")
                        TEXT(" ")
                        TEXT("Request debug name: '") << streamableHandle->GetDebugName() << TEXT("'.")
                        TEXT(" ")
                        << TEXT("Index: `") << iteration << TEXT("`.");

                if constexpr (shouldReportErrors)
                {
                    GC_LOG_STR_NO_CONTEXT(
                        LogGCUtils_AssetStreaming,
                        Error,
                        logString
                        );
                    ensure(false);
                }
                else
                {
                    // Write an informative, non-error log.
                    GC_LOG_STR_NO_CONTEXT(
                        LogGCUtils_AssetStreaming,
                        Log,
                        logString
                        );
                }

                check(!shouldAssumeSuccess);
            }
#endif // #if !NO_LOGGING || DO_ENSURE

            loadedAssets.Emplace(loadedAsset);
        }
        );

    return loadedAssets;
}

template
    <
    bool shouldManageActiveHandle,
    bool shouldReportErrors,
    bool shouldAssumeSuccess
    >
TSharedPtr<FStreamableHandle> GCUtils::AssetStreaming::Private::LoadSyncGeneralized(
    FStreamableManager& inStreamableManager,
    TArray<FSoftObjectPath>&& inAssetPaths)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::AssetStreaming::Private::LoadSyncGeneralized(FStreamableManager&, TArray<FSoftObjectPath>&&));

#if !NO_LOGGING || DO_ENSURE
    FStringBuilderBase&& assetPathsString = MakeStringFromAssetPaths(inAssetPaths);
#endif // #if !NO_LOGGING || DO_ENSURE

    constexpr bool shouldStartStalled = false;

    FString debugName;

    // Note: We should ideally use the `FStreamableManager::ShouldStripDebugName()` function to
    // conditionally build a debug string, but the engine doesn't give us access to that so
    // we just do a simple build configuration check here and make a few assumptions.
#if !UE_BUILD_SHIPPING
    debugName = FString(MakeStringFromAssetPaths(inAssetPaths));
#endif // #if !UE_BUILD_SHIPPING

    // Start async load.
    TSharedPtr<FStreamableHandle> streamableHandle =
        inStreamableManager.RequestAsyncLoad(
            MoveTemp(inAssetPaths),
            FStreamableDelegate(),
            AsyncLoadPriorityBlocking,
            shouldManageActiveHandle,
            shouldStartStalled,
            MoveTemp(debugName)
            );

    if (!streamableHandle)
    {
#if !NO_LOGGING || DO_ENSURE
        {
            const FStringBuilderBase& logString =
                GCUtils::Materialize(TStringBuilder<512>())
                    << TEXT("Async load request returned a null streamable handle.")
                    TEXT(" ")
                    TEXT("Asset paths: '") << assetPathsString << TEXT("'.");

            if constexpr (shouldReportErrors)
            {
                GC_LOG_STR_NO_CONTEXT(
                    LogGCUtils_AssetStreaming,
                    Error,
                    logString
                    );
                ensure(false);
            }
            else
            {
                // Write an informative, non-error log.
                GC_LOG_STR_NO_CONTEXT(
                    LogGCUtils_AssetStreaming,
                    Log,
                    logString
                    );
            }
        }
#endif // #if !NO_LOGGING || DO_ENSURE

        if constexpr (!shouldAssumeSuccess)
        {
            return nullptr;
        }
    }

    // Wait synchronously for the load to complete.
    check(streamableHandle);
    const EAsyncPackageState::Type asyncPackageState = streamableHandle->WaitUntilComplete();

#if !NO_LOGGING || DO_ENSURE
    if (asyncPackageState != EAsyncPackageState::Complete)
    {
        const FStringBuilderBase& logString =
            GCUtils::Materialize(TStringBuilder<512>())
                << TEXT("Waiting until complete returned a non-complete async package state!")
                TEXT(" ")
                TEXT("Asset paths: '") << assetPathsString << TEXT("'.")
                TEXT(" ")
                << TEXT("Async package state: '") << LexToString(asyncPackageState) << TEXT("'.");

        if constexpr (shouldReportErrors)
        {
            GC_LOG_STR_NO_CONTEXT(
                LogGCUtils_AssetStreaming,
                Error,
                logString
                );
            ensure(false);
        }
        else
        {
            // Write an informative, non-error log.
            GC_LOG_STR_NO_CONTEXT(
                LogGCUtils_AssetStreaming,
                Log,
                logString
                );
        }
    }

    if (streamableHandle->HasLoadCompleted() == false)
    {
        const FStringBuilderBase& logString =
            GCUtils::Materialize(TStringBuilder<512>())
                << TEXT("Load request has not fully completed after waiting until complete!")
                TEXT(" ")
                TEXT("Asset paths: '") << assetPathsString << TEXT("'.")
                TEXT(" ")
                << TEXT("Async package state: '") << LexToString(asyncPackageState) << TEXT("'.");

        if constexpr (shouldReportErrors)
        {
            GC_LOG_STR_NO_CONTEXT(
                LogGCUtils_AssetStreaming,
                Error,
                logString
                );
            ensure(false);
        }
        else
        {
            // Write an informative, non-error log.
            GC_LOG_STR_NO_CONTEXT(
                LogGCUtils_AssetStreaming,
                Log,
                logString
                );
        }
    }
#endif // #if !NO_LOGGING || DO_ENSURE

    return streamableHandle;
}
