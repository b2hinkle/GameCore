// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"

/**
 * Makes a cstring literal out of the given text.
 */
#define GC_CSTRINGIZE(text) TEXT(PREPROCESSOR_TO_STRING(text))

/**
 * Literal macro alternative to GCUtils::String::StringNull. Nice if you need a string literal instead of
 * a variable. E.g., for combining string literals together.
 */
#define GC_CSTRING_NULL TEXT("NULL")

/**
 * Literal macro alternative to GCUtils::String::StringTrue.
 */
#define GC_CSTRING_TRUE TEXT("True")

/**
 * Literal macro alternative to GCUtils::String::StringFalse.
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
    /**
     * @brief Utility struct for using `UObjectBaseUtility::GetPathName()` in the string
     *        builder's EInPlace constructor.
     */
    GAMECORE_API struct FStringAppender_UObjectBaseUtility_GetPathName
    {
    public:

        // Note: Friend only so that we can define the free function right here in the class definition.
        friend FStringBuilderBase& operator<<(FStringBuilderBase& inStringBuilder,
            const FStringAppender_UObjectBaseUtility_GetPathName& inStringAppender)
        {
            inStringAppender.Object.GetPathName(
                inStringAppender.StopOuter,
                inStringBuilder);
            return inStringBuilder;
        }

    public:

        const UObject& Object;
        const UObject* StopOuter = nullptr;
    };

    /**
     * @brief Utility struct for using `UObjectBaseUtility::GetFullName()` in the string
     *        builder's EInPlace constructor.
     */
    GAMECORE_API struct FStringAppender_UObjectBaseUtility_GetFullName
    {
    public:

        // Note: Friend only so that we can define the free function right here in the class definition.
        friend FStringBuilderBase& operator<<(FStringBuilderBase& inStringBuilder,
            const FStringAppender_UObjectBaseUtility_GetFullName& inStringAppender)
        {
            inStringAppender.Object.GetFullName(
                inStringBuilder,
                inStringAppender.StopOuter,
                inStringAppender.Flags);
            return inStringBuilder;
        }

    public:

        const UObject& Object;
        const UObject* StopOuter = nullptr;
        EObjectFullNameFlags Flags = EObjectFullNameFlags::None;
    };

    GAMECORE_API constexpr const FStringView BoolToString(const bool inBool);

    template <int32 BufferSize = 256>
    TStringBuilder<BufferSize> GetUObjectNameSafe(const UObject* inUObject);

    template <int32 BufferSize = 256>
    TStringBuilder<BufferSize> GetUObjectName(const UObject& inUObject);

    template <int32 BufferSize = 512>
    TStringBuilder<BufferSize> GetUObjectPathNameSafe(
        const UObject* inUObject,
        const UObject* inStopOuter = nullptr);

    template <int32 BufferSize = 512>
    TStringBuilder<BufferSize> GetUObjectPathName(
        const UObject& inUObject,
        const UObject* inStopOuter = nullptr);

    template <int32 BufferSize = 512>
    TStringBuilder<BufferSize> GetUObjectFullNameSafe(
        const UObject* inUObject,
        const UObject* inStopOuter = nullptr,
        EObjectFullNameFlags inFlags = EObjectFullNameFlags::None);

    template <int32 BufferSize = 512>
    TStringBuilder<BufferSize> GetUObjectFullName(
        const UObject& inUObject,
        const UObject* inStopOuter = nullptr,
        EObjectFullNameFlags inFlags = EObjectFullNameFlags::None);

    GAMECORE_API const FStringView GetWorldNetModeString(const UObject* inWorldContextObject);

    GAMECORE_API const FStringView GetObjectLocalRoleString(const UObject* inActorContextObject);

    /**
     * Whether the "owning" controller for an object is local.
     *
     * See GCUtils::GetController().
     */
    GAMECORE_API const FStringView GetIsControllerLocalString(const UObject* inControllerContextObject);

    constexpr FStringView StringNull = PREPROCESSOR_JOIN(GC_CSTRING_NULL, _PrivateSV);

    constexpr FStringView StringTrue = PREPROCESSOR_JOIN(GC_CSTRING_TRUE, _PrivateSV);

    constexpr FStringView StringFalse = PREPROCESSOR_JOIN(GC_CSTRING_FALSE, _PrivateSV);
}

template <int32 BufferSize>
TStringBuilder<BufferSize> GCUtils::String::GetUObjectNameSafe(const UObject* inUObject)
{
    if (!inUObject)
    {
        return TStringBuilder<BufferSize>(EInPlace::InPlace, StringNull);
    }

    return GetUObjectName(*inUObject);
}

template <int32 BufferSize>
TStringBuilder<BufferSize> GCUtils::String::GetUObjectName(const UObject& inUObject)
{
    // Avoid `UObjectBaseUtility::GetName()` which does an unnecessary free-store string
    // allocation. Use the FName instead.
    return TStringBuilder<BufferSize>(EInPlace::InPlace, inUObject.GetFName());
}

template <int32 BufferSize>
TStringBuilder<BufferSize> GCUtils::String::GetUObjectPathNameSafe(
    const UObject* inUObject,
    const UObject* inStopOuter)
{
    if (!inUObject)
    {
        return TStringBuilder<BufferSize>(EInPlace::InPlace, StringNull);
    }

    return GetUObjectPathName(*inUObject, inStopOuter);
}

template <int32 BufferSize>
TStringBuilder<BufferSize> GCUtils::String::GetUObjectPathName(
    const UObject& inUObject,
    const UObject* inStopOuter)
{
    // Note: String builders are not copyable so we must return a prvalue to elide the copy. This is
    // a good idea anyway because we want to be wary of possibly large buffer sizes.
    return TStringBuilder<BufferSize>(
        EInPlace::InPlace,
        FStringAppender_UObjectBaseUtility_GetPathName
        {
            .Object = inUObject,
            .StopOuter = inStopOuter
        }
        );
}

template <int32 BufferSize>
TStringBuilder<BufferSize> GCUtils::String::GetUObjectFullNameSafe(
    const UObject* inUObject,
    const UObject* inStopOuter,
    EObjectFullNameFlags inFlags)
{
    if (!inUObject)
    {
        return TStringBuilder<BufferSize>(EInPlace::InPlace, StringNull);
    }

    return GetUObjectFullName(*inUObject, inStopOuter, inFlags);
}

template <int32 BufferSize>
TStringBuilder<BufferSize> GCUtils::String::GetUObjectFullName(
    const UObject& inUObject,
    const UObject* inStopOuter,
    EObjectFullNameFlags inFlags)
{
    // Note: String builders are not copyable so we must return a prvalue to elide the copy. This is
    // a good idea anyway because we want to be wary of possibly large buffer sizes.
    return TStringBuilder<BufferSize>(
        EInPlace::InPlace,
        FStringAppender_UObjectBaseUtility_GetFullName
        {
            .Object = inUObject,
            .StopOuter = inStopOuter,
            .Flags = inFlags
        }
        );
}
