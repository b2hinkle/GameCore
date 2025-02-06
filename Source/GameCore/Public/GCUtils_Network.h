// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/EngineBaseTypes.h"
#include "HAL/PreprocessorHelpers.h"
#include "Misc/CoreMisc.h"

class UWorld;
struct FURL;

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
     * @brief Determine whether the given world is a server world using its direct net mode.
     * @see `GCUtils::Network::IsNetModeDirect()`.
     */
    GAMECORE_API FORCEINLINE_DEBUGGABLE bool IsServerDirect(const UWorld& inWorld);

    /**
     * @brief Determine whether the given world is a client world using its direct net mode.
     * @see `GCUtils::Network::IsNetModeDirect()`.
     */
    GAMECORE_API FORCEINLINE_DEBUGGABLE bool IsClientDirect(const UWorld& inWorld);

    /**
     * @brief Version of `UWorld::IsNetMode()` but is unconcerned of any pending world travels.
     * @see `GCUtils::Network::GetNetModeDirect()`.
     * @remark [duplicate-code-engine]: Review implementation when upgrading.
     */
    GAMECORE_API FORCEINLINE_DEBUGGABLE bool IsNetModeDirect(const UWorld& inWorld, const ENetMode inNetMode);

    /**
     * @brief Get a net mode that directly represents the given world. This behaves identically
     *        to `UWorld::GetNetMode()` but is unconcerned of any pending world travels.
     * @remark [duplicate-code-engine]: Review implementation when upgrading.
     */
    GAMECORE_API ENetMode GetNetModeDirect(const UWorld& inWorld);

    /**
     * @brief Get a net mode that represents the network setup of the given URL. This
     *        mimics `UWorld::AttemptDeriveFromURL()` but for a particular URL.
     * @remark [duplicate-code-engine]: Review implementation when upgrading.
     */
    GAMECORE_API ENetMode GetNetModeAttemptDeriveFromURL(const FURL& inURL);

    /**
     * @brief The URL option for listen server.
     */
    constexpr FStringView URLOptionListen = UE_JOIN(GC_URL_OPTION_LISTEN_STRING_LITERAL, _PrivateSV);
}

bool GCUtils::Network::IsServerDirect(const UWorld& inWorld)
{
    return !IsNetModeDirect(inWorld, ENetMode::NM_Client);
}

bool GCUtils::Network::IsClientDirect(const UWorld& inWorld)
{
    return IsNetModeDirect(inWorld, ENetMode::NM_Client);
}

bool GCUtils::Network::IsNetModeDirect(const UWorld& inWorld, const ENetMode inNetMode)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Network::IsNetModeDirect);

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

    return GetNetModeDirect(inWorld) == inNetMode;
}
