// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"
#include "Types/GCStringBuilderAppender.h"
#include <source_location>

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
    template <GCConcepts::CharType TCharType>
    using TStringBuilderCallback = TFunctionRef<void(TStringBuilderBase<TCharType>&)>;

    /**
     * @brief Writes and returns a created string builder of a template character type. This
     *        is a generalized version of engine's "WriteToString" functions that takes in the
     *        string builder's char type as a template parameter.
     * @tparam TCharType The character type for the string builder to be created.
     * @tparam BufferSize The buffer length for the string builder to be created.
     * @param inArgs The arguments to write into the string builder.
     */
    template <GCConcepts::CharType TCharType, int32 BufferSize, class... TArgs>
    TStringBuilderWithBuffer<TCharType, BufferSize> WriteToStringGeneric(TArgs&&... inArgs);

    /**
     * @brief Create a string builder and initialize it from a callback while still being constructed as a prvalue.
     * @param inInitializationCallback Callback function for initializing the string builder as however needed.
     * @return String builder constructed in place (copy/move-elided) and initialized based on the callback.
     */
    template <int32 BufferSize, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> ConstructStringBuilder(
        const TStringBuilderCallback<TCharType>& inInitializationCallback);

    // ~ UObject functions.

    template <int32 BufferSize = 256, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetUObjectNameSafe(const UObject* inUObject);

    template <int32 BufferSize = 256, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetUObjectName(const UObject& inUObject);

    /**
     * @note `UObjectBaseUtility::GetPathName()` only supports TCHAR at the moment. You will
     *       get an error if you try using this function with other char types.
     */
    template <int32 BufferSize = 512, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetUObjectPathNameSafe(
        const UObject* inUObject,
        const UObject* inStopOuter = nullptr);

    /**
     * @note `UObjectBaseUtility::GetPathName()` only supports TCHAR at the moment. You will
     *       get an error if you try using this function with other char types.
     */
    template <int32 BufferSize = 512, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetUObjectPathName(
        const UObject& inUObject,
        const UObject* inStopOuter = nullptr);

    /**
     * @note `UObjectBaseUtility::GetFullName()` only supports TCHAR at the moment. You will
     *       get an error if you try using this function with other char types.
     */
    template <int32 BufferSize = 512, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetUObjectFullNameSafe(
        const UObject* inUObject,
        const UObject* inStopOuter = nullptr,
        EObjectFullNameFlags inFlags = EObjectFullNameFlags::None);

    /**
     * @note `UObjectBaseUtility::GetFullName()` only supports TCHAR at the moment. You will
     *       get an error if you try using this function with other char types.
     */
    template <int32 BufferSize = 512, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetUObjectFullName(
        const UObject& inUObject,
        const UObject* inStopOuter = nullptr,
        EObjectFullNameFlags inFlags = EObjectFullNameFlags::None);

    // ~ UObject functions.

    // ~ IInterface functions.

    template <GCConcepts::PointerToIInterface TIInterfacePtr, int32 BufferSize = 256, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetIInterfaceNameSafe(const TIInterfacePtr inIInterface);

    template <GCConcepts::ReferenceToIInterface TIInterfaceRef, int32 BufferSize = 256, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetIInterfaceName(const TIInterfaceRef inIInterface);

    /**
     * @note `UObjectBaseUtility::GetPathName()` only supports TCHAR at the moment. You will
     *       get an error if you try using this function with other char types.
     */
    template <GCConcepts::PointerToIInterface TIInterfacePtr, int32 BufferSize = 512, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetIInterfacePathNameSafe(
        const TIInterfacePtr inIInterface,
        const UObject* inStopOuter = nullptr);

    /**
     * @note `UObjectBaseUtility::GetPathName()` only supports TCHAR at the moment. You will
     *       get an error if you try using this function with other char types.
     */
    template <GCConcepts::ReferenceToIInterface TIInterfaceRef, int32 BufferSize = 512, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetIInterfacePathName(
        const TIInterfaceRef inIInterface,
        const UObject* inStopOuter = nullptr);

    /**
     * @note `UObjectBaseUtility::GetFullName()` only supports TCHAR at the moment. You will
     *       get an error if you try using this function with other char types.
     */
    template <GCConcepts::PointerToIInterface TIInterfacePtr, int32 BufferSize = 512, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetIInterfaceFullNameSafe(
        const TIInterfacePtr inIInterface,
        const UObject* inStopOuter = nullptr,
        EObjectFullNameFlags inFlags = EObjectFullNameFlags::None);

    /**
     * @note `UObjectBaseUtility::GetFullName()` only supports TCHAR at the moment. You will
     *       get an error if you try using this function with other char types.
     */
    template <GCConcepts::ReferenceToIInterface TIInterfaceRef, int32 BufferSize = 512, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetIInterfaceFullName(
        const TIInterfaceRef inIInterface,
        const UObject* inStopOuter = nullptr,
        EObjectFullNameFlags inFlags = EObjectFullNameFlags::None);

    // ~ IInterface functions.

    /**
     * @brief Has the same behavior as `AActor::GetActorNameOrLabel()`.
     */
    template <int32 BufferSize = 512, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetActorNameOrLabelSafe(const AActor* inActor);

    /**
     * @brief Has the same behavior as `AActor::GetActorNameOrLabel()`.
     */
    template <int32 BufferSize = 512, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetActorNameOrLabel(const AActor& inActor);

    /**
     * @brief Gets the current locaiton in code as a string. Esentially a wrapper of `std::source_location` but geared towards UE's string building workflow.
     * @tparam BufferSize The amount of characters this string will hold.
     * @tparam TCharType The character type to use.
     * @param sourceLocation The location in code to write as a string. Default value will cover mostly every use case.
     * @return String of the current location in code.
     */
    template <int32 BufferSize = 512, GCConcepts::CharType TCharType = TCHAR>
    TStringBuilderWithBuffer<TCharType, BufferSize> GetFunctionSourceLocation(const std::source_location& sourceLocation = std::source_location::current());

    GAMECORE_API const FStringView GetWorldNetModeString(const UObject* inWorldContextObject);

    GAMECORE_API const FStringView GetObjectLocalRoleString(const UObject* inActorContextObject);

    /**
     * Whether the "owning" controller for an object is local.
     *
     * See GCUtils::GetController().
     */
    GAMECORE_API const FStringView GetIsControllerLocalString(const UObject* inControllerContextObject);

    GAMECORE_API constexpr const FStringView BoolToString(const bool inBool);

    GAMECORE_API constexpr const FStringView BoolToString(const bool inBool);
    
    /**
     * @brief Gets a net mode string describing the current PIE instance. Similar to how the prefix string works from `UKismetSystemLibrary::PrintString`.
     * @tparam BufferSize The amount of characters this string will hold.
     * @tparam TCharType The character type to use.
     * @param world The context world.
     * @return A string builder of the result PIE instance string.
     */
    template <int32 BufferSize = 64, GCConcepts::CharType TCharType = TCHAR>
    const TStringBuilderWithBuffer<TCharType, BufferSize> GetPlayInEditorInstanceMultiplayerName(const UWorld& world);

    constexpr FStringView StringNull = PREPROCESSOR_JOIN(GC_STRING_LITERAL_NULL, _PrivateSV);

    constexpr FStringView StringTrue = PREPROCESSOR_JOIN(GC_STRING_LITERAL_TRUE, _PrivateSV);

    constexpr FStringView StringFalse = PREPROCESSOR_JOIN(GC_STRING_LITERAL_FALSE, _PrivateSV);
}

template <GCConcepts::CharType TCharType, int32 BufferSize, class... TArgs>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::WriteToStringGeneric(TArgs&&... inArgs)
{
    return TStringBuilderWithBuffer<TCharType, BufferSize>(EInPlace::InPlace, Forward<TArgs>(inArgs)...);
}

template <int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::ConstructStringBuilder(
    const TStringBuilderCallback<TCharType>& inInitializationCallback)
{
    // Note: String builders are not copyable so we must return a prvalue to elide the copy. This is
    // a good idea anyway because we want to be wary of possibly large buffer sizes.
    return WriteToStringGeneric<TCharType, BufferSize>(
        ConstructStringBuilderAppender<TCharType>(
            [&inInitializationCallback](TStringBuilderBase<TCharType>& inStringBuilder) -> TStringBuilderBase<TCharType>&
            {
                inInitializationCallback(inStringBuilder);
                return inStringBuilder;
            }
            )
        );
}

template <int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetUObjectNameSafe(const UObject* inUObject)
{
    if (!inUObject)
    {
        return WriteToStringGeneric<TCharType, BufferSize>(StringNull);
    }

    return GetUObjectName<BufferSize, TCharType>(*inUObject);
}

template <int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetUObjectName(const UObject& inUObject)
{
    // Avoid `UObjectBaseUtility::GetName()` which does an unnecessary free-store string
    // allocation. Use the FName instead.
    return WriteToStringGeneric<TCharType, BufferSize>(inUObject.GetFName());
}

template <int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetUObjectPathNameSafe(
    const UObject* inUObject,
    const UObject* inStopOuter)
{
    if (!inUObject)
    {
        return WriteToStringGeneric<TCharType, BufferSize>(StringNull);
    }

    return GetUObjectPathName<BufferSize, TCharType>(*inUObject, inStopOuter);
}

template <int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetUObjectPathName(
    const UObject& inUObject,
    const UObject* inStopOuter)
{
    return ConstructStringBuilder<BufferSize, TCharType>(
        [&inUObject, inStopOuter](TStringBuilderBase<TCharType>& inStringBuilder) -> void
        {
            inUObject.GetPathName(inStopOuter, inStringBuilder);
        }
        );
}

template <int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetUObjectFullNameSafe(
    const UObject* inUObject,
    const UObject* inStopOuter,
    EObjectFullNameFlags inFlags)
{
    if (!inUObject)
    {
        return WriteToStringGeneric<TCharType, BufferSize>(StringNull);
    }

    return GetUObjectFullName<BufferSize, TCharType>(*inUObject, inStopOuter, inFlags);
}

template <int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetUObjectFullName(
    const UObject& inUObject,
    const UObject* inStopOuter,
    EObjectFullNameFlags inFlags)
{
    return ConstructStringBuilder<BufferSize, TCharType>(
        [&inUObject, inStopOuter, inFlags](TStringBuilderBase<TCharType>& inStringBuilder) -> void
        {
            inUObject.GetFullName(inStopOuter, inStringBuilder, inFlags);
        }
        );
}

template <GCConcepts::PointerToIInterface TIInterfacePtr, int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetIInterfaceNameSafe(const TIInterfacePtr inIInterface)
{
    if (!inIInterface)
    {
        return WriteToStringGeneric<TCharType, BufferSize>(StringNull);
    }

    return GetIInterfaceName(*inIInterface);
}

template <GCConcepts::ReferenceToIInterface TIInterfaceRef, int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetIInterfaceName(const TIInterfaceRef inIInterface)
{
    return GetUObjectNameSafe(inIInterface._getUObject());
}

template <GCConcepts::PointerToIInterface TIInterfacePtr, int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetIInterfacePathNameSafe(
    const TIInterfacePtr inIInterface,
    const UObject* inStopOuter)
{
    if (!inIInterface)
    {
        return WriteToStringGeneric<TCharType, BufferSize>(StringNull);
    }

    return GetIInterfacePathName(*inIInterface, inStopOuter);
}

template <GCConcepts::ReferenceToIInterface TIInterfaceRef, int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetIInterfacePathName(
    const TIInterfaceRef inIInterface,
    const UObject* inStopOuter)
{
    return GetUObjectPathNameSafe(inIInterface._getUObject(), inStopOuter);
}

template <GCConcepts::PointerToIInterface TIInterfacePtr, int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetIInterfaceFullNameSafe(
    const TIInterfacePtr inIInterface,
    const UObject* inStopOuter,
    EObjectFullNameFlags inFlags)
{
    if (!inIInterface)
    {
        return WriteToStringGeneric<TCharType, BufferSize>(StringNull);
    }

    return GetIInterfaceFullName(*inIInterface, inStopOuter, inFlags);
}

template <GCConcepts::ReferenceToIInterface TIInterfaceRef, int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetIInterfaceFullName(
    const TIInterfaceRef inIInterface,
    const UObject* inStopOuter,
    EObjectFullNameFlags inFlags)
{
    return GetUObjectFullNameSafe(inIInterface._getUObject(), inStopOuter, inFlags);
}

template <int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetActorNameOrLabelSafe(const AActor* inActor)
{
    if (!inActor)
    {
        return WriteToStringGeneric<TCharType, BufferSize>(StringNull);
    }

    return GetActorNameOrLabel<BufferSize, TCharType>(*inActor);
}

template <int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetActorNameOrLabel(const AActor& inActor)
{
    return ConstructStringBuilder<BufferSize, TCharType>(
        [&inActor](TStringBuilderBase<TCharType>& inStringBuilder) -> void
        {
#if WITH_EDITORONLY_DATA || ACTOR_HAS_LABELS
            const FStringView& actorLabelString = inActor.GetActorLabelView();
            if (actorLabelString.IsEmpty() == false)
            {
                // Use name if label is empty.
                inStringBuilder << inActor.GetFName();
                return;
            }

            inStringBuilder << actorLabelString;
#else // #if WITH_EDITORONLY_DATA || ACTOR_HAS_LABELS
            inStringBuilder << inActor.GetFName();
#endif // #else // #if WITH_EDITORONLY_DATA || ACTOR_HAS_LABELS
        }
        );
}

template <int32 BufferSize, GCConcepts::CharType TCharType>
TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetFunctionSourceLocation(const std::source_location& sourceLocation)
{
    return WriteToStringGeneric<TCharType, BufferSize>(sourceLocation.function_name());
}

template <int32 BufferSize, GCConcepts::CharType TCharType>
const TStringBuilderWithBuffer<TCharType, BufferSize> GCUtils::String::GetPlayInEditorInstanceMultiplayerName(const UWorld& world)
{
    checkf(world.WorldType == EWorldType::PIE, TEXT("Function `%s` only for PIE sessions."), GCUtils::String::GetFunctionSourceLocation().ToString());
    switch (world.GetNetMode())
    {
    case NM_Client:
        return WriteToStringGeneric<TCharType, BufferSize>(TEXT("Client "), UE::GetPlayInEditorID());
    case NM_DedicatedServer:
        return WriteToStringGeneric<TCharType, BufferSize>("Dedicated Server");
    case NM_ListenServer:
        return WriteToStringGeneric<TCharType, BufferSize>(TEXT("Listen Server"));
    case NM_Standalone:
        return WriteToStringGeneric<TCharType, BufferSize>(TEXT("Standalone"));
    }

    checkf(0, TEXT("This will never hit."));
    return WriteToStringGeneric<TCharType, BufferSize>(TEXT(""));
}
