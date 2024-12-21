// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FGCPrintToScreenArgs;

// TODO: Document
/**
 * @brief 
 * @param args 
 */
GAMECORE_API void GCPrintToScreen(const FGCPrintToScreenArgs& args);
GAMECORE_API void GCPrintToScreen(FGCPrintToScreenArgs&& args);

/**
 * @brief Struct containing arguments for UEngine::AddOnScreenDebugMessage.
 */
struct GAMECORE_API FGCAddOnScreenDebugMessageArgs
{
public:

    static FString GetMessageDefault() { return TEXT("Hello"); }

public:

    FString Message = GetMessageDefault();

    FVector2D TextScale = FVector2D::UnitVector;

    FColor Color = FColor(0.f, 168.f, 255.f);

    int32 Key = -1;

    float Duration = 2.f;

    bool bNewerOnTop = true;
};

/**
 * @brief Struct containing arguments for our GCPrintToScreen function.
 */
struct GAMECORE_API FGCPrintToScreenArgs
{

public:

    FGCPrintToScreenArgs(FGCAddOnScreenDebugMessageArgs&& addOnScreenDebugMessageArgs)
        : AddOnScreenDebugMessageArgs(MoveTemp(addOnScreenDebugMessageArgs))
    {
    }

    FGCPrintToScreenArgs(FGCAddOnScreenDebugMessageArgs&& addOnScreenDebugMessageArgs, const UObject* worldContextObj)
        : AddOnScreenDebugMessageArgs(MoveTemp(addOnScreenDebugMessageArgs)),
        WorldContextObj(worldContextObj)
    {
    }
    
public:

    FGCAddOnScreenDebugMessageArgs AddOnScreenDebugMessageArgs;

    const UObject* WorldContextObj = nullptr;

};

/**
 * @brief Prints specified string to the screen. This struct uses the Named Parameter Idiom technique to achieve quick and simple calls.
 */
struct GAMECORE_API FGCPrintToScreen
{
public:

    /**
     * @brief Prints specified string to the screen. Further arguments may be provided through builder functions. E.g. FGCPrintToScreen(TEXT("Hi")).bNewerOnTop(false); 
     * @Note: Temporary lifetime is the intended lifetime for objects of this struct.
     * @param message The message to print.
     */
    FGCPrintToScreen(FString&& message = FGCAddOnScreenDebugMessageArgs::GetMessageDefault())
        : PrintToScreenArgs(
            FGCAddOnScreenDebugMessageArgs(MoveTemp(message))
        )
    {
    }

    /**
     * @brief Prints specified string to the screen. Further arguments may be provided through builder functions. E.g. FGCPrintToScreen(TEXT("Hi")).bNewerOnTop(false); 
     * @Note: Temporary lifetime is the intended lifetime for objects of this struct.
     * @param message The message to print.
     */
    FGCPrintToScreen(const UObject* worldContextObj, FString&& message = FGCAddOnScreenDebugMessageArgs::GetMessageDefault())
        : PrintToScreenArgs(
            FGCAddOnScreenDebugMessageArgs(MoveTemp(message)),
            worldContextObj
        )
    {
    }


    // No use for this ctr.
    FGCPrintToScreen(const FGCPrintToScreen& other) = delete;

    ~FGCPrintToScreen()
    {
        ::GCPrintToScreen(MoveTemp(PrintToScreenArgs));
    }


public: // Builders functions.

    FGCPrintToScreen&& Key(const int32 value) &&
    {
        PrintToScreenArgs.AddOnScreenDebugMessageArgs.Key = value;
        return MoveTemp(*this);
    }
    FGCPrintToScreen&& Duration(const float value) &&
    {
        PrintToScreenArgs.AddOnScreenDebugMessageArgs.Duration = value;
        return MoveTemp(*this);
    }
    FGCPrintToScreen&& Color(const FColor value) &&
    {
        PrintToScreenArgs.AddOnScreenDebugMessageArgs.Color = value;
        return MoveTemp(*this);
    }
    FGCPrintToScreen&& bNewerOnTop(const bool value) &&
    {
        PrintToScreenArgs.AddOnScreenDebugMessageArgs.bNewerOnTop = value;
        return MoveTemp(*this);
    }
    FGCPrintToScreen&& TextScale(const FVector2D& value) &&
    {
        PrintToScreenArgs.AddOnScreenDebugMessageArgs.TextScale = value;
        return MoveTemp(*this);
    }

private: // Arguments

    FGCPrintToScreenArgs PrintToScreenArgs;
};
