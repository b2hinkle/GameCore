// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_AssetStreaming.h"
#include "GCUtils_AssetStreaming.inl"

DEFINE_LOG_CATEGORY(LogGCUtils_AssetStreaming);

const TCHAR* LexToString(const EAsyncPackageState::Type value)
{
    switch (value)
    {
    case EAsyncPackageState::TimeOut:
        return TEXT("TimeOut");
    case EAsyncPackageState::PendingImports:
        return TEXT("PendingImports");
    case EAsyncPackageState::Complete:
        return TEXT("Complete");
    }

    return TEXT("");
}

void GCUtils::AssetStreaming::ForEachLoadedAssetBreakable(
    const TSharedRef<FStreamableHandle>& inStreamableHandle,
    const FForEachLoadedAssetBreakableCallbackFunctionRef& inCallback)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::AssetStreaming::ForEachLoadedAssetBreakable);

    inStreamableHandle->ForEachLoadedAsset(
        [&inCallback, &inStreamableHandle, iteration = 0, shouldContinue = true](UObject* loadedAsset) mutable
        {
            if (!shouldContinue)
            {
                return;
            }

            const int32 index = iteration;
            shouldContinue = inCallback(loadedAsset, index, inStreamableHandle.Get());

            ++iteration;
        }
        );
}
