// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"

/**
 * Makes a cstring literal out of the given text.
 */
#define GC_CSTRINGIZE(text) TEXT(PREPROCESSOR_TO_STRING(text))

/**
 * Macro alternative to GCUtils::String::CStringNull. Nice if you need a string literal instead of
 * a variable. E.g., for combining string literals together.
 */
#define GC_CSTRING_NULL TEXT("NULL")

/**
 * Macro alternative to GCUtils::String::CStringTrue.
 */
#define GC_CSTRING_TRUE TEXT("True")

/**
 * Macro alternative to GCUtils::String::CStringFalse.
 */
#define GC_CSTRING_FALSE TEXT("False")

/**
 * Returns the string representation of the specified ENetMode value.
 *
 * @param inNetMode The string representation of ENetMode.
 * @return A CString representation of the net mode.
 */
GAMECORE_API const TCHAR* LexToString(ENetMode inNetMode);

/**
 * Returns the string representation of the specified ENetRole value.
 *
 * @param inNetRole The string representation of ENetRole.
 * @return A CString representation of the net role.
 */
GAMECORE_API const TCHAR* LexToString(ENetRole inNetRole);

namespace GCUtils::String
{
    constexpr const TCHAR* CStringNull = GC_CSTRING_NULL;

    constexpr const TCHAR* CStringTrue = GC_CSTRING_TRUE;

    constexpr const TCHAR* CStringFalse = GC_CSTRING_FALSE;

    GAMECORE_API constexpr const TCHAR* BoolToCString(const bool inBool);

    GAMECORE_API FString GetUObjectNameSafe(const UObject* inUObject);

    GAMECORE_API FString GetUObjectFullNameSafe(const UObject* inUObject, const UObject* inStopOuter = nullptr, EObjectFullNameFlags inFlags = EObjectFullNameFlags::None);

    GAMECORE_API const TCHAR* GetWorldNetModeCString(const UObject* inWorldContextObject);

    GAMECORE_API const TCHAR* GetObjectLocalRoleCString(const UObject* inContextObject);

    /**
     * Whether the "owning" controller for an object is local.
     *
     * See GCUtils::GetController().
     */
    GAMECORE_API const TCHAR* GetIsControllerLocalCString(const UObject* inObject);
}
