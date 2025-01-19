// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GCUtils.h"
#include "GCUtils_String.h"
#include <source_location>
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerState.h"

/**
 * @brief Plain log. Printf style.
 * @note Prefer the contextual log macros over this one.
 * @remark This is just a wrapper of the engine's log macro but it exists to show a writer's
 *         intentions and to demonstrate consistency with the naming of our log macros.
 */
#define GC_PLAIN_LOG_FMT(inCategoryName, inVerbosity, inFormat, ...) UE_LOG(inCategoryName, inVerbosity, inFormat __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Plain conditional log. Printf style.
 * @note Prefer the contextual log macros over this one.
 * @remark This is just a wrapper of the engine's log macro but it exists to show a writer's
 *         intentions and to demonstrate consistency with the naming of our log macros.
 */
#define GC_PLAIN_CLOG_FMT(inCondition, inCategoryName, inVerbosity, inFormat, ...) UE_CLOG(inCondition, inCategoryName, inVerbosity, inFormat __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Extra format string literal used in no-context logs. For outputting info about the current context.
 * @note Internal macro! Not intended for use.
 * @note Corresponds with `GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_ARGS`.
 */
#define GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_STRING_LITERAL TEXT("[%s]")

/**
 * @brief Extra variadic format arguments used in no-context logs. For outputting info about the current context.
 * @note Internal macro! Not intended for use.
 * @note Arguments correspond with `GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_STRING_LITERAL`.
 */
#define GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_ARGS GCUtils::String::GetFunctionSourceLocation().ToString()

/**
 * @brief Log with no extra context to output info for. Printf style.
 */
#define GC_LOG_FMT_NO_CONTEXT(inCategoryName, inVerbosity, inFormat, ...) GC_PLAIN_LOG_FMT(inCategoryName, inVerbosity, GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_STRING_LITERAL TEXT(" ") inFormat, GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_ARGS __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Conditional log with no extra context to output info for. Printf style.
 */
#define GC_CLOG_FMT_NO_CONTEXT(inCondition, inCategoryName, inVerbosity, inFormat, ...) GC_PLAIN_CLOG_FMT(inCondition, inCategoryName, inVerbosity, GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_STRING_LITERAL TEXT(" ") inFormat, GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_ARGS __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Extra format string literal used in UObject-context logs. For outputting info about the current context.
 * @note Internal macro! Not intended for use.
 */
#define GC_PRIVATE_INFO_UOBJECT_FORMAT_STRING_LITERAL TEXT("[Context UObject Arg: `%s`]")

/**
 * @brief Log with a `UObject` for context to output extra info for. Printf style.
 * @param inContextUObject The `UObject*` used to output extra info.
 */
#define GC_LOG_FMT_UOBJECT(inContextUObject, inCategoryName, inVerbosity, inFormat, ...) GC_LOG_FMT_NO_CONTEXT(inCategoryName, inVerbosity, inFormat TEXT(" ") GC_PRIVATE_INFO_UOBJECT_FORMAT_STRING_LITERAL TEXT(" %s"), __VA_ARGS__ __VA_OPT__(,) TEXT(PREPROCESSOR_TO_STRING(inContextUObject)), GCUtils::Log::GetUObjectLogInfoString(inContextUObject).ToString())

/**
 * @brief Conditional log with a `UObject` for context to output extra info for. Printf style.
 * @param inContextUObject The `UObject*` used to output extra info.
 */
#define GC_CLOG_FMT_UOBJECT(inContextUObject, inCondition, inCategoryName, inVerbosity, inFormat, ...) GC_CLOG_FMT_NO_CONTEXT(inCondition, inCategoryName, inVerbosity, inFormat TEXT(" ") GC_PRIVATE_INFO_UOBJECT_FORMAT_STRING_LITERAL TEXT(" %s"), __VA_ARGS__ __VA_OPT__(,) TEXT(PREPROCESSOR_TO_STRING(inContextUObject)), GCUtils::Log::GetUObjectLogInfoString(inContextUObject).ToString())

/**
 * @brief Extra format string literal used in IInterface-context logs. For outputting info about the current context.
 * @note Internal macro! Not intended for use.
 */
#define GC_PRIVATE_INFO_IINTERFACE_FORMAT_STRING_LITERAL TEXT("[Context IInterface Arg: `%s`]")

/**
 * @brief Version of `GC_LOG_FMT_UOBJECT()` for `IInterface`s.
 */
#define GC_LOG_FMT_IINTERFACE(inContextIInterface, inCategoryName, inVerbosity, inFormat, ...) GC_LOG_FMT_NO_CONTEXT(inCategoryName, inVerbosity, inFormat TEXT(" ") GC_PRIVATE_INFO_IINTERFACE_FORMAT_STRING_LITERAL TEXT(" %s"), __VA_ARGS__ __VA_OPT__(,) TEXT(PREPROCESSOR_TO_STRING(inContextIInterface)), GCUtils::Log::GetIInterfaceLogInfoString(inContextIInterface).ToString())

/**
 * @brief Version of `GC_CLOG_FMT_UOBJECT()` for `IInterface`s.
 */
#define GC_CLOG_FMT_IINTERFACE(inContextIInterface, inCondition, inCategoryName, inVerbosity, inFormat, ...) GC_CLOG_FMT_NO_CONTEXT(inCondition, inCategoryName, inVerbosity, inFormat TEXT(" ") GC_PRIVATE_INFO_IINTERFACE_FORMAT_STRING_LITERAL TEXT(" %s"), __VA_ARGS__ __VA_OPT__(,) TEXT(PREPROCESSOR_TO_STRING(inContextIInterface)), GCUtils::Log::GetIInterfaceLogInfoString(inContextIInterface).ToString())

/**
 * @brief Plain log. From string.
 * @note Prefer the contextual log macros over this one.
 * @param inString String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_PLAIN_LOG_STR(inCategoryName, inVerbosity, inString) { const auto& refBoundString = inString; GC_PLAIN_LOG_FMT(inCategoryName, inVerbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

/**
 * @brief Plain conditional log. From string.
 * @note Prefer the contextual log macros over this one.
 * @param inString String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_PLAIN_CLOG_STR(inCondition, inCategoryName, inVerbosity, inString) { const auto& refBoundString = inString; GC_PLAIN_CLOG_FMT(inCondition, inCategoryName, inVerbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

/**
 * @brief Log with no extra context to output info for. From string.
 * @param inString String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_LOG_STR_NO_CONTEXT(inCategoryName, inVerbosity, inString) { const auto& refBoundString = inString; GC_LOG_FMT_NO_CONTEXT(inCategoryName, inVerbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

/**
 * @brief Conditional log with no extra context to output info for. From string.
 * @param inString String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_CLOG_STR_NO_CONTEXT(inCondition, inCategoryName, inVerbosity, inString) { const auto& refBoundString = inString; GC_CLOG_FMT_NO_CONTEXT(inCondition, inCategoryName, inVerbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

/**
 * @brief Log with a `UObject` for context to output extra info for. From string.
 * @param inContextUObject The `UObject*` used to output extra info.
 * @param inString String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_LOG_STR_UOBJECT(inContextUObject, inCategoryName, inVerbosity, inString) { const auto& refBoundString = inString; GC_LOG_FMT_UOBJECT(inContextUObject, inCategoryName, inVerbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

/**
 * @brief Conditional log with a `UObject` for context to output extra info for. From string.
 * @param inContextUObject The `UObject*` used to output extra info.
 * @param inString String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_CLOG_STR_UOBJECT(inContextUObject, inCondition, inCategoryName, inVerbosity, inString) { const auto& refBoundString = inString; GC_CLOG_FMT_UOBJECT(inContextUObject, inCondition, inCategoryName, inVerbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

/**
 * @brief Version of `GC_LOG_STR_UOBJECT()` for `IInterface`s.
 */
#define GC_LOG_STR_IINTERFACE(inContextIInterface, inCategoryName, inVerbosity, inString) { const auto& refBoundString = inString; GC_LOG_FMT_IINTERFACE(inContextIInterface, inCategoryName, inVerbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

/**
 * @brief Version of `GC_CLOG_STR_UOBJECT()` for `IInterface`s.
 */
#define GC_CLOG_STR_IINTERFACE(inContextIInterface, inCondition, inCategoryName, inVerbosity, inString) { const auto& refBoundString = inString; GC_CLOG_FMT_IINTERFACE(inContextIInterface, inCondition, inCategoryName, inVerbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

namespace GCUtils::Log
{
    /**
     * @brief Build a debug string given a `UObject`.
     */
    template <int32 bufferLength = 1024>
    TStringBuilder<bufferLength> GetUObjectLogInfoString(const UObject* inUObject);

    /**
     * @brief Build a debug string given an `IInterface`.
     */
    template <GCConcepts::PointerToIInterface TIInterfacePtr, int32 bufferLength = 1024>
    TStringBuilder<bufferLength> GetIInterfaceLogInfoString(const TIInterfacePtr inIInterface);
}

template <int32 bufferLength>
TStringBuilder<bufferLength> GCUtils::Log::GetUObjectLogInfoString(const UObject* inUObject)
{
    FString(* const getPlayerNameSafe)(const AController*) =
        [](const AController* controller) -> FString
        {
            if (!controller)
            {
                return FString(String::StringNull);
            }

            const APlayerState* playerState = controller->GetPlayerState<APlayerState>();
            if (!playerState)
            {
                return FString(String::StringNull);
            }

            return playerState->GetPlayerName();
        };

    return WriteToString<bufferLength>(
        TEXT("[Object: '"), GCUtils::String::GetUObjectNameSafe(inUObject), TEXT("']"),
        TEXT(' '),
        TEXT("[Net Mode: "), GCUtils::String::GetWorldNetModeString(inUObject), TEXT(']'),
        TEXT(' '),
        TEXT("[Net Role: "), GCUtils::String::GetObjectLocalRoleString(inUObject), TEXT(']'),
        TEXT(' '),
        TEXT("[Controller: '"), GCUtils::String::GetUObjectNameSafe(GCUtils::GetController(inUObject)), TEXT("']"),
        TEXT(' '),
        TEXT("[Is Controller Local: "), GCUtils::String::GetIsControllerLocalString(inUObject), TEXT(']'),
        TEXT(' '),
        TEXT("[Player Name: '"), getPlayerNameSafe(GCUtils::GetController(inUObject)), TEXT("']")
    );
}

template <GCConcepts::PointerToIInterface TIInterfacePtr, int32 bufferLength>
TStringBuilder<bufferLength> GCUtils::Log::GetIInterfaceLogInfoString(const TIInterfacePtr inIInterface)
{
    return GetUObjectLogInfoString(
        inIInterface ? inIInterface->_getUObject() : nullptr);
}
