// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_Network.h"

#include "CoreMinimal.h"
#include "GCUtils_Log.h"
#include "Engine/World.h"
#include "Engine/NetDriver.h"
#include "Engine/DemoNetDriver.h"

DEFINE_LOG_CATEGORY_STATIC(LogGCUtils_Network, Log, All);

namespace
{
#if WITH_EDITOR
    /**
     * @brief [inaccessible-access-engine] Get `UWorld::PlayInEditorNetMode`.
     */
    template <ENetMode UWorld::* pointerToMember = &UWorld::PlayInEditorNetMode>
    ENetMode GetPlayInEditorNetModeInternal(const UWorld& inWorld)
    {
        return inWorld.*pointerToMember;
    }
#endif // #if WITH_EDITOR
}

ENetMode GCUtils::Network::GetNetModeDirect(const UWorld& inWorld)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Network::GetNetModeDirect);

    // Comment from `UWorld::GetNetMode()`:
    // IsRunningDedicatedServer() is a compile-time check in optimized non-editor builds.
    if (IsRunningDedicatedServer())
    {
        return ENetMode::NM_DedicatedServer;
    }

    if (const UNetDriver* netDriver = inWorld.GetNetDriver())
    {
        if (IsRunningClientOnly())
        {
            return ENetMode::NM_Client;
        }

        return netDriver->GetNetMode();
    }

    if (const UDemoNetDriver* demoNetDriver = inWorld.GetDemoNetDriver())
    {
        // Comment from `UWorld::GetNetMode()`:
        // Use replay driver's net mode if we're in playback or ticking recording
        if (demoNetDriver->IsPlaying() || (demoNetDriver->IsRecording() && demoNetDriver->IsInTick()))
        {
            return demoNetDriver->GetNetMode();
        }
    }

    // Note: This is where behavior differs from the engine's `UWorld::GetNetMode()` function. We
    // derive the current net mode from the URL that was used initially to travel to this world.
    const FURL& urlToAttemptDeriveFrom = inWorld.URL;
    const ENetMode urlNetMode = GetNetModeAttemptDeriveFromURL(urlToAttemptDeriveFrom);

#if WITH_EDITOR
    if (inWorld.IsPlayInEditor())
    {
        const ENetMode playInEditorNetMode = GetPlayInEditorNetModeInternal(inWorld);

        if (urlNetMode == ENetMode::NM_Standalone || playInEditorNetMode == ENetMode::NM_DedicatedServer)
        {
            // Comment from `UWorld::GetNetMode()`:
            // If we're early in startup before the net driver exists and there is no URL override
            // or this is a dedicated server, use the mode we were first created with
            // This is required for dedicated server/listen worlds so it is correct for InitWorld
            return playInEditorNetMode;
        }
    }
#endif // #if WITH_EDITOR

    return urlNetMode;
}

ENetMode GCUtils::Network::GetNetModeAttemptDeriveFromURL(const FURL& inURL)
{
    if (inURL.Valid)
    {
        if (inURL.HasOption(URLOptionListen.GetData()))
        {
            return ENetMode::NM_ListenServer;
        }

        if (inURL.Host.IsEmpty() == false)
        {
            return ENetMode::NM_Client;
        }
    }

    return ENetMode::NM_Standalone;
}
