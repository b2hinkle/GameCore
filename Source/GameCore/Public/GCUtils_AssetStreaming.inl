// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GCUtils_AssetStreaming.h"

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GCUtils_Log.h"
#include "GCUtils_String.h"
#include "GCConcepts.h"
#include "GCUtils.h"
#include "Templates/GCGetUClassType.h"

namespace GCUtils::AssetStreaming::Private
{
#if !NO_LOGGING || DO_ENSURE || DO_CHECK
    template
        <
        bool shouldReportErrors,
        bool shouldAssumeSuccess
        >
    void HandleNullLoadedAsset(
        const FStreamableHandle& inStreamableHandle,
        const int32 inLoadedAssetIndex);
#endif // #if !NO_LOGGING || DO_ENSURE || DO_CHECK

#if !NO_LOGGING || DO_ENSURE || DO_CHECK
    template
        <
        bool shouldReportErrors,
        bool shouldAssumeSuccess,
        GCConcepts::UObjectDerivedOrIInterface TAsset
        >
    void HandleFailedCastOfLoadedAsset(
        const FStreamableHandle& inStreamableHandle,
        const int32 inLoadedAssetIndex,
        const UObject& loadedAsset);
#endif // #if !NO_LOGGING || DO_ENSURE || DO_CHECK

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
TSharedRef<FStreamableHandle> GCUtils::AssetStreaming::LoadSyncChecked(
    FStreamableManager& inStreamableManager,
    FSoftObjectPath&& inAssetPath)
{
    TArray<FSoftObjectPath> assetPathArray;
    assetPathArray.Emplace(MoveTemp(inAssetPath));

    return LoadSyncChecked<shouldManageActiveHandle>(inStreamableManager, MoveTemp(assetPathArray));
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
TSharedPtr<FStreamableHandle> GCUtils::AssetStreaming::LoadSync(
    FStreamableManager& inStreamableManager,
    FSoftObjectPath&& inAssetPath)
{
    TArray<FSoftObjectPath> assetPathArray;
    assetPathArray.Emplace(MoveTemp(inAssetPath));

    return LoadSync<shouldManageActiveHandle, shouldReportErrors>(inStreamableManager, MoveTemp(assetPathArray));
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

template
    <
    GCConcepts::UObjectDerivedOrIInterface TAsset
    >
TAsset& GCUtils::AssetStreaming::GetLoadedAssetChecked(
    const TSharedRef<FStreamableHandle>& inStreamableHandle)
{
    using TAllocator = TFixedAllocator<1u>;

    TArray<std::reference_wrapper<TAsset>, TAllocator> loadedAssetArray =
        GetLoadedAssetsChecked<TAllocator, TAsset>(
            inStreamableHandle
            );

    return loadedAssetArray[0];
}

template
    <
    class TAllocator,
    GCConcepts::UObjectDerivedOrIInterface TAsset
    >
TArray<std::reference_wrapper<TAsset>, TAllocator> GCUtils::AssetStreaming::GetLoadedAssetsChecked(
    const TSharedRef<FStreamableHandle>& inStreamableHandle)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::AssetStreaming::GetLoadedAssetsChecked);

    constexpr bool shouldReportErrors = true;
    constexpr bool shouldAssumeSuccess = true;

    TArray<std::reference_wrapper<TAsset>, TAllocator> loadedAssets;

    ForEachLoadedAssetBreakable(inStreamableHandle,
        [&loadedAssets](UObject* loadedAsset, const int32 index, FStreamableHandle& streamableHandle)
        {
#if !NO_LOGGING || DO_ENSURE || DO_CHECK
            if (!loadedAsset)
            {
                Private::HandleNullLoadedAsset<shouldReportErrors, shouldAssumeSuccess>(streamableHandle, index);
            }
            else
            {
                if (!Cast<TAsset>(loadedAsset))
                {
                    Private::HandleFailedCastOfLoadedAsset<shouldReportErrors, shouldAssumeSuccess, TAsset>(streamableHandle, index, *loadedAsset);
                }
            }
#endif // #if !NO_LOGGING || DO_ENSURE || DO_CHECK

            // TODO: Ideally, this would be a static cast or reinterpret cast depending on whether we're
            // casting to a UObject or IInterface class.
            TAsset& loadedAssetCasted = GCUtils::ReinterpretCastChecked<TAsset&>(loadedAsset);
            loadedAssets.Emplace(loadedAssetCasted);
            return true;
        }
        );

    return loadedAssets;
}

template
    <
    bool shouldReportErrors,
    GCConcepts::UObjectDerivedOrIInterface TAsset
    >
TAsset* GCUtils::AssetStreaming::GetLoadedAsset(
    const TSharedRef<FStreamableHandle>& inStreamableHandle)
{
    using TAllocator = TFixedAllocator<1u>;

#if !NO_LOGGING || DO_ENSURE
    {
        int32 numLoaded = 0;
        int32 numRequested = 0;
        inStreamableHandle->GetLoadedCount(numLoaded, numRequested);

        if (!ensure(numRequested <= 1))
        {
            GC_LOG_STR_NO_CONTEXT(
                LogGCUtils_AssetStreaming,
                Warning,
                GCUtils::Materialize(TStringBuilder<512>())
                    << TEXT("Singular asset function was used for a request of multiple assets. Improper use of function!")
                    TEXT(" ")
                    TEXT("Load request debug name: '") << inStreamableHandle->GetDebugName() << TEXT("'.")
                );
        }
    }
#endif // #if !NO_LOGGING || DO_ENSURE

    TArray<TAsset*, TAllocator> loadedAssetArray =
        GetLoadedAssets<TAllocator, shouldReportErrors, TAsset>(
            inStreamableHandle
            );

    if (loadedAssetArray.IsEmpty())
    {
#if !NO_LOGGING
        {
            const FStringBuilderBase& logString =
                GCUtils::Materialize(TStringBuilder<512>())
                    << TEXT("Array of loaded assets is empty. Null will be returned.")
                    TEXT(" ")
                    TEXT("Load request debug name: '") << inStreamableHandle->GetDebugName() << TEXT("'.");

            if constexpr (shouldReportErrors)
            {
                GC_LOG_STR_NO_CONTEXT(
                    LogGCUtils_AssetStreaming,
                    Error,
                    logString
                    );
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
#endif // #if !NO_LOGGING

        ensure(!shouldReportErrors);
        return nullptr;
    }

    return loadedAssetArray[0];
}

template
    <
    class TAllocator,
    bool shouldReportErrors,
    GCConcepts::UObjectDerivedOrIInterface TAsset
    >
TArray<TAsset*, TAllocator> GCUtils::AssetStreaming::GetLoadedAssets(
    const TSharedRef<FStreamableHandle>& inStreamableHandle)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::AssetStreaming::GetLoadedAssets);

    constexpr bool shouldAssumeSuccess = false;

    TArray<TAsset*, TAllocator> loadedAssets;

    ForEachLoadedAssetBreakable(inStreamableHandle,
        [&loadedAssets](UObject* loadedAsset, const int32 index, FStreamableHandle& streamableHandle)
        {
#if !NO_LOGGING || DO_ENSURE || DO_CHECK
            if (!loadedAsset)
            {
                Private::HandleNullLoadedAsset<shouldReportErrors, shouldAssumeSuccess>(streamableHandle, index);
            }
            else
            {
                if (!Cast<TAsset>(loadedAsset))
                {
                    Private::HandleFailedCastOfLoadedAsset<shouldReportErrors, shouldAssumeSuccess, TAsset>(streamableHandle, index, *loadedAsset);
                }
            }
#endif // #if !NO_LOGGING || DO_ENSURE || DO_CHECK

            TAsset* loadedAssetCasted = Cast<TAsset>(loadedAsset);
            loadedAssets.Emplace(loadedAssetCasted);
            return true;
        }
        );

    return loadedAssets;
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

#if !NO_LOGGING || DO_ENSURE || DO_CHECK
template
    <
    bool shouldReportErrors,
    bool shouldAssumeSuccess
    >
void GCUtils::AssetStreaming::Private::HandleNullLoadedAsset(
    const FStreamableHandle& inStreamableHandle, const int32 inLoadedAssetIndex)
{
#if !NO_LOGGING
    {
        const FStringBuilderBase& logString =
            GCUtils::Materialize(TStringBuilder<512>())
                << TEXT("Loaded asset is null.")
                TEXT(" ")
                TEXT("Load request debug name: '") << inStreamableHandle.GetDebugName() << TEXT("'.")
                TEXT(" ")
                << TEXT("Index: `") << inLoadedAssetIndex << TEXT("`.");

        if constexpr (shouldReportErrors)
        {
            GC_LOG_STR_NO_CONTEXT(
                LogGCUtils_AssetStreaming,
                Error,
                logString
                );
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
#endif // #if !NO_LOGGING

    ensure(!shouldReportErrors);
    check(!shouldAssumeSuccess);
}
#endif // #if !NO_LOGGING || DO_ENSURE || DO_CHECK

#if !NO_LOGGING || DO_ENSURE || DO_CHECK
template
    <
    bool shouldReportErrors,
    bool shouldAssumeSuccess,
    GCConcepts::UObjectDerivedOrIInterface TAsset
    >
void GCUtils::AssetStreaming::Private::HandleFailedCastOfLoadedAsset(
    const FStreamableHandle& inStreamableHandle, const int32 inLoadedAssetIndex, const UObject& loadedAsset)
{
#if !NO_LOGGING
    {
        const FStringBuilderBase& logString =
            GCUtils::Materialize(TStringBuilder<512>())
                << TEXT("Cast of loaded asset failed.")
                TEXT(" ")
                TEXT("Load request debug name: '") << inStreamableHandle.GetDebugName() << TEXT("'.")
                TEXT(" ")
                << TEXT("Index: `") << inLoadedAssetIndex << TEXT("`.")
                TEXT(" ")
                << TEXT("Target class: '") << GCUtils::String::GetUObjectPathNameSafe(TGCGetUClassType_T<TAsset>::StaticClass()) << TEXT("'.")
                TEXT(" ")
                << TEXT("Actual class of loaded asset: '") << GCUtils::String::GetUObjectPathNameSafe(loadedAsset.GetClass()) << TEXT("'.");

        if constexpr (shouldReportErrors)
        {
            GC_LOG_STR_NO_CONTEXT(
                LogGCUtils_AssetStreaming,
                Error,
                logString
                );
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
#endif // #if !NO_LOGGING

    ensure(!shouldReportErrors);
    check(!shouldAssumeSuccess);
}
#endif // #if !NO_LOGGING || DO_ENSURE || DO_CHECK

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
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::AssetStreaming::Private::LoadSyncGeneralized);

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

            ensure(!shouldReportErrors);
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

        ensure(!shouldReportErrors);
    }
#endif // #if !NO_LOGGING || DO_ENSURE

#if !NO_LOGGING || DO_ENSURE
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

        ensure(!shouldReportErrors);
    }
#endif // #if !NO_LOGGING || DO_ENSURE

    return streamableHandle;
}
