// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FGCPrintToScreenArgs;

GAMECORE_API TStringBuilder<128> GCGetPrintToScreenPrefix(const UObject* worldContextObj);

/**
 * @brief Our method for printing to the screen. While this function is useful for manual situations,
 *        using `FGCPrintToScreen` is the more streamlined choice.
 * @param args Settings we should use when printing.
 */
GAMECORE_API void GCPrintToScreen(const FGCPrintToScreenArgs& args);

/**
 * @brief Struct containing arguments for `UEngine::AddOnScreenDebugMessage`.
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
 * @brief Struct containing arguments for our `GCPrintToScreen` function.
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
        OptionalWorldContextObjWeak(worldContextObj)
    {
    }
    
public:

    FGCAddOnScreenDebugMessageArgs AddOnScreenDebugMessageArgs;
    
    /**
     * @brief Optional context object for a more detailed log.
     */
    TOptional<TWeakObjectPtr<const UObject>> OptionalWorldContextObjWeak;

};

/**
 * @brief This struct makes use of temporary lifetime for code execution. Combined with the Named Parameter Idiom technique (via function chaining), we can achieve
 *        quick and simple print to screen calls.
 */
struct GAMECORE_API FGCPrintToScreen
{
public:

    /**
     * @brief Prints specified string to the screen. Further arguments may be provided through builder functions. E.g. `FGCPrintToScreen(TEXT("Hi")).bNewerOnTop(false).Duration(1.f);` 
     * @Note: Temporary lifetime is the intended lifetime for objects of this type.
     * @param message The message to print.
     */
    FGCPrintToScreen(FString&& message = FGCAddOnScreenDebugMessageArgs::GetMessageDefault())
        : PrintToScreenArgs(
            FGCAddOnScreenDebugMessageArgs(MoveTemp(message))
        )
    {
    }
    
    /**
     * @brief Prints specified string to the screen. Further arguments may be provided through builder functions. E.g. `FGCPrintToScreen(TEXT("Hi")).bNewerOnTop(false).Duration(1.f);` 
     * @Note: Temporary lifetime is the intended lifetime for objects of this type.
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
