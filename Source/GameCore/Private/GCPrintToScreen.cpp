// Fill out your copyright notice in the Description page of Project Settings.

#include "GCPrintToScreen.h"

#include "GCUtils_String.h"

TStringBuilder<128> GCGetPrintToScreenPrefix(const UObject* worldContextObj)
{
    // TODO: This is more or less a temporary decision for the multiplayer prefix. Ideally we make it the same across all build targets.
    //       We should also account for split screen.

    if (!worldContextObj)
    {
        return WriteToString<128>(TEXT("No world context object: "));
    }

    const UWorld* world = worldContextObj->GetWorld();
    if (!world)
    {
        return WriteToString<128>(TEXT("No world: "));
    }

    if (world->WorldType == EWorldType::PIE)
    {
        return WriteToString<128>(GCUtils::String::GetPlayInEditorInstanceMultiplayerName(*world), TEXT(": "));
    }

    return WriteToString<128>(GCUtils::String::GetWorldNetModeString(world), TEXT(": "));
}

void GCPrintToScreen(const FGCPrintToScreenArgs& args)
{
    if (!GAreScreenMessagesEnabled)
    {
        return;
    }

    TStringBuilder<64> message;

    // Prepend the prefix string.
    if (args.OptionalWorldContextObjWeak.IsSet())
    {
        const TWeakObjectPtr<const UObject>& worldContextObjWeak = args.OptionalWorldContextObjWeak.GetValue();
        message << GCGetPrintToScreenPrefix(worldContextObjWeak.Get());
    }

    message << args.AddOnScreenDebugMessageArgs.Message;

    GEngine->AddOnScreenDebugMessage(
        args.AddOnScreenDebugMessageArgs.Key,
        args.AddOnScreenDebugMessageArgs.Duration,
        args.AddOnScreenDebugMessageArgs.Color,
        message.ToString(),
        args.AddOnScreenDebugMessageArgs.bNewerOnTop,
        args.AddOnScreenDebugMessageArgs.TextScale
    );
}
