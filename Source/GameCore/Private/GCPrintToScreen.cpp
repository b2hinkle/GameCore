// Fill out your copyright notice in the Description page of Project Settings.

#include "GCPrintToScreen.h"

void GCPrintToScreen(const FGCAddOnScreenDebugMessageArgs& args)
{
    GEngine->AddOnScreenDebugMessage(
        args.Key,
        args.Duration,
        args.Color,
        args.Message,
        args.bNewerOnTop,
        args.TextScale);
}
void GCPrintToScreen(FGCAddOnScreenDebugMessageArgs&& args)
{
    GEngine->AddOnScreenDebugMessage(
        args.Key,
        args.Duration,
        MoveTemp(args.Color),
        MoveTemp(args.Message),
        args.bNewerOnTop,
        MoveTemp(args.TextScale));
}
