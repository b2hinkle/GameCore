// Fill out your copyright notice in the Description page of Project Settings.

#include "GCPrintToScreen.h"

void GCPrintToScreen(const FGCPrintToScreenArgs& args)
{
    if (!GAreScreenMessagesEnabled)
    {
        return;
    }

    GEngine->AddOnScreenDebugMessage(
        args.AddOnScreenDebugMessageArgs.Key,
        args.AddOnScreenDebugMessageArgs.Duration,
        args.AddOnScreenDebugMessageArgs.Color,
        args.AddOnScreenDebugMessageArgs.Message,
        args.AddOnScreenDebugMessageArgs.bNewerOnTop,
        args.AddOnScreenDebugMessageArgs.TextScale
    );
}
void GCPrintToScreen(FGCPrintToScreenArgs&& args)
{
    if (!GAreScreenMessagesEnabled)
    {
        return;
    }

    GEngine->AddOnScreenDebugMessage(
        args.AddOnScreenDebugMessageArgs.Key,
        args.AddOnScreenDebugMessageArgs.Duration,
        MoveTemp(args.AddOnScreenDebugMessageArgs.Color),
        MoveTemp(args.AddOnScreenDebugMessageArgs.Message),
        args.AddOnScreenDebugMessageArgs.bNewerOnTop,
        MoveTemp(args.AddOnScreenDebugMessageArgs.TextScale)
    );
}
