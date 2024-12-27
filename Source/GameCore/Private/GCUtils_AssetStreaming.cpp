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
