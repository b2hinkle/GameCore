// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"
#include "String/StringBuilderAppender.h"

/**
 * Makes a string literal out of the given text.
 */
#define GC_STRING_LITERALIZE(text) TEXT(PREPROCESSOR_TO_STRING(text))

/**
 * Literal alternative to GCUtils::String::StringNull.
 */
#define GC_STRING_LITERAL_NULL TEXT("NULL")

/**
 * Literal alternative to GCUtils::String::StringTrue.
 */
#define GC_STRING_LITERAL_TRUE TEXT("True")

/**
 * Literal alternative to GCUtils::String::StringFalse.
 */
#define GC_STRING_LITERAL_FALSE TEXT("False")

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
    template <class TCharType = TCHAR>
    using TStringBuilderCallback = TFunctionRef<void(TStringBuilderBase<TCharType>&)>;

    /**
     * @brief Makes a prvalue string builder initialized with the given callback.
     * @param inInitializationCallback Callback function for initializing the string builder as however needed.
     * @return String builder constructed in place (copy-elided) and initialized based on the callback.
     */
    template <int32 BufferSize, class TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> MakeStringBuilder(const TStringBuilderCallback<TCharType>& inInitializationCallback);

    template <int32 BufferSize = 256, class TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetUObjectNameSafe(const UObject* inUObject);

    template <int32 BufferSize = 256, class TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetUObjectName(const UObject& inUObject);

    /**
     * @note UObjectBaseUtility::GetPathName() only supports TCHAR at the moment. You will
     *       get an error if you try using this function with other char types.
     */
    template <int32 BufferSize = 512, class TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetUObjectPathNameSafe(
        const UObject* inUObject,
        const UObject* inStopOuter = nullptr);

    /**
     * @note UObjectBaseUtility::GetPathName() only supports TCHAR at the moment. You will
     *       get an error if you try using this function with other char types.
     */
    template <int32 BufferSize = 512, class TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetUObjectPathName(
        const UObject& inUObject,
        const UObject* inStopOuter = nullptr);

    /**
     * @note UObjectBaseUtility::GetFullName() only supports TCHAR at the moment. You will
     *       get an error if you try using this function with other char types.
     */
    template <int32 BufferSize = 512, class TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetUObjectFullNameSafe(
        const UObject* inUObject,
        const UObject* inStopOuter = nullptr,
        EObjectFullNameFlags inFlags = EObjectFullNameFlags::None);

    /**
     * @note UObjectBaseUtility::GetFullName() only supports TCHAR at the moment. You will
     *       get an error if you try using this function with other char types.
     */
    template <int32 BufferSize = 512, class TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetUObjectFullName(
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

    GAMECORE_API constexpr const FStringView BoolToString(const bool inBool);

    constexpr FStringView StringNull = PREPROCESSOR_JOIN(GC_STRING_LITERAL_NULL, _PrivateSV);

    constexpr FStringView StringTrue = PREPROCESSOR_JOIN(GC_STRING_LITERAL_TRUE, _PrivateSV);

    constexpr FStringView StringFalse = PREPROCESSOR_JOIN(GC_STRING_LITERAL_FALSE, _PrivateSV);
}

template <int32 BufferSize, class TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::MakeStringBuilder(const TStringBuilderCallback<TCharType>& inInitializationCallback)
{
    // Note: String builders are not copyable so we must return a prvalue to elide the copy. This is
    // a good idea anyway because we want to be wary of possibly large buffer sizes.
    return TStringBuilderWithBuffer<TCharType, BufferSize>(
        EInPlace::InPlace,
        TStringBuilderAppender(
            [&inInitializationCallback](TStringBuilderBase<TCharType>& inStringBuilder) -> TStringBuilderBase<TCharType>&
            {
                inInitializationCallback(inStringBuilder);
                return inStringBuilder;
            }
            )
        );
}

template <int32 BufferSize, class TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetUObjectNameSafe(const UObject* inUObject)
{
    if (!inUObject)
    {
        return TStringBuilderWithBuffer<TCharType, BufferSize>(EInPlace::InPlace, StringNull);
    }

    return GetUObjectName<BufferSize, TCharType>(*inUObject);
}

template <int32 BufferSize, class TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetUObjectName(const UObject& inUObject)
{
    // Avoid `UObjectBaseUtility::GetName()` which does an unnecessary free-store string
    // allocation. Use the FName instead.
    return TStringBuilderWithBuffer<TCharType, BufferSize>(EInPlace::InPlace, inUObject.GetFName());
}

template <int32 BufferSize, class TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetUObjectPathNameSafe(
    const UObject* inUObject,
    const UObject* inStopOuter)
{
    if (!inUObject)
    {
        return TStringBuilderWithBuffer<TCharType, BufferSize>(EInPlace::InPlace, StringNull);
    }

    return GetUObjectPathName<BufferSize, TCharType>(*inUObject, inStopOuter);
}

template <int32 BufferSize, class TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetUObjectPathName(
    const UObject& inUObject,
    const UObject* inStopOuter)
{
    return MakeStringBuilder<BufferSize, TCharType>(
        [&inUObject, inStopOuter](TStringBuilderBase<TCharType>& inStringBuilder) -> void
        {
            inUObject.GetPathName(inStopOuter, inStringBuilder);
        }
        );
}

template <int32 BufferSize, class TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetUObjectFullNameSafe(
    const UObject* inUObject,
    const UObject* inStopOuter,
    EObjectFullNameFlags inFlags)
{
    if (!inUObject)
    {
        return TStringBuilderWithBuffer<TCharType, BufferSize>(EInPlace::InPlace, StringNull);
    }

    return GetUObjectFullName<BufferSize, TCharType>(*inUObject, inStopOuter, inFlags);
}

template <int32 BufferSize, class TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetUObjectFullName(
    const UObject& inUObject,
    const UObject* inStopOuter,
    EObjectFullNameFlags inFlags)
{
    return MakeStringBuilder<BufferSize, TCharType>(
        [&inUObject, inStopOuter, inFlags](TStringBuilderBase<TCharType>& inStringBuilder) -> void
        {
            inUObject.GetFullName(inStopOuter, inStringBuilder, inFlags);
        }
        );
}
