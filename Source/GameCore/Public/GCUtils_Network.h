// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/EngineBaseTypes.h"
#include "HAL/PreprocessorHelpers.h"
#include "Misc/CoreMisc.h"

class UWorld;
struct FURL;

DECLARE_LOG_CATEGORY_EXTERN(LogGCUtils_Network, Log, All);

/**
 * @brief String-literal alternative to `GCUtils::Network::URLOptionListen`.
 */
#define GC_URL_OPTION_LISTEN_STRING_LITERAL TEXT("listen")

/**
 * @brief Utilities for networking.
 */
namespace GCUtils::Network
{
    /**
     * @brief Mimics `UWorld::IsNetMode()` but is unconcerned of any pending world travels.
     * @remark #duplicate-code-engine: Review implementation when upgrading.
     */
    GAMECORE_API FORCEINLINE_DEBUGGABLE bool IsNetModeRaw(const UWorld& inWorld, const ENetMode inNetMode);

    /**
     * @brief Mimics `UWorld::GetNetMode()` but is unconcerned of any pending world travels.
     * @remark #duplicate-code-engine: Review implementation when upgrading.
     */
    GAMECORE_API ENetMode GetNetModeRaw(const UWorld& inWorld);

    /**
     * @brief Mimics `UWorld::AttemptDeriveFromURL()` but for a particular URL.
     * @remark #duplicate-code-engine: Review implementation when upgrading.
     */
    GAMECORE_API ENetMode GetNetModeAttemptDeriveFromURL(const FURL& inURL);

    /**
     * @brief URL option for listen server.
     */
    constexpr FStringView URLOptionListen = UE_JOIN(GC_URL_OPTION_LISTEN_STRING_LITERAL, _PrivateSV);
}

bool GCUtils::Network::IsNetModeRaw(const UWorld& inWorld, const ENetMode inNetMode)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Network::IsNetModeRaw);

    // Comment from `UWorld::IsNetMode()`:
    // Editor builds are special because of PIE, which can run a dedicated server without the app running with -server.

#if !UE_EDITOR
    // Comment from `UWorld::IsNetMode()`:
    // IsRunningDedicatedServer() is a compile-time check in optimized non-editor builds.
    const bool isRunningDedicatedServer = IsRunningDedicatedServer();

    if (inNetMode == ENetMode::NM_DedicatedServer)
    {
        return isRunningDedicatedServer;
    }

    if (isRunningDedicatedServer)
    {
        return false;
    }
#endif // #if !UE_EDITOR

    return GetNetModeRaw(inWorld) == inNetMode;
}
