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
#define GC_PLAIN_LOG_FMT(categoryName, verbosity, format, ...) UE_LOG(categoryName, verbosity, format __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Plain log. From string.
 * @note Prefer the contextual log macros over this one.
 * @param string String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_PLAIN_LOG_STR(categoryName, verbosity, string) { const auto& refBoundString = string; GC_PLAIN_LOG_FMT(categoryName, verbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

/**
 * @brief Plain conditional log. Printf style.
 * @note Prefer the contextual log macros over this one.
 * @remark This is just a wrapper of the engine's log macro but it exists to show a writer's
 *         intentions and to demonstrate consistency with the naming of our log macros.
 */
#define GC_PLAIN_CLOG_FMT(condition, categoryName, verbosity, format, ...) UE_CLOG(condition, categoryName, verbosity, format __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Plain conditional log. From string.
 * @note Prefer the contextual log macros over this one.
 * @param string String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_PLAIN_CLOG_STR(condition, categoryName, verbosity, string) { const auto& refBoundString = string; GC_PLAIN_CLOG_FMT(condition, categoryName, verbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

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
#define GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_ARGS StringCast<TCHAR, 512>(std::source_location::current().function_name()).Get()

/**
 * @brief Log with no extra context to output info for. Printf style.
 */
#define GC_LOG_FMT_NO_CONTEXT(categoryName, verbosity, format, ...) GC_PLAIN_LOG_FMT(categoryName, verbosity, GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_STRING_LITERAL TEXT(" ") format, GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_ARGS __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Log with no extra context to output info for. From string.
 * @param string String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_LOG_STR_NO_CONTEXT(categoryName, verbosity, string) { const auto& refBoundString = string; GC_LOG_FMT_NO_CONTEXT(categoryName, verbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

/**
 * @brief Conditional log with no extra context to output info for. Printf style.
 */
#define GC_CLOG_FMT_NO_CONTEXT(condition, categoryName, verbosity, format, ...) GC_PLAIN_CLOG_FMT(condition, categoryName, verbosity, GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_STRING_LITERAL TEXT(" ") format, GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_ARGS __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Conditional log with no extra context to output info for. From string.
 * @param string String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_CLOG_STR_NO_CONTEXT(condition, categoryName, verbosity, string) { const auto& refBoundString = string; GC_CLOG_FMT_NO_CONTEXT(condition, categoryName, verbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

/**
 * @brief Extra format string literal used in UObject-context logs. For outputting info about the current context.
 * @note Internal macro! Not intended for use.
 */
#define GC_PRIVATE_INFO_UOBJECT_FORMAT_STRING_LITERAL TEXT("[Context UObject Arg: `%s`]")

/**
 * @brief Log with a `UObject` for context to output extra info for. Printf style.
 * @param contextUObject The `UObject*` used to output extra info.
 */
#define GC_LOG_FMT_UOBJECT(contextUObject, categoryName, verbosity, format, ...) GC_LOG_FMT_NO_CONTEXT(categoryName, verbosity, format TEXT(" ") GC_PRIVATE_INFO_UOBJECT_FORMAT_STRING_LITERAL TEXT(" %s"), __VA_ARGS__ __VA_OPT__(,) TEXT(PREPROCESSOR_TO_STRING(contextUObject)), GCUtils::Log::GetUObjectLogInfoString(contextUObject).ToString())

/**
 * @brief Log with a `UObject` for context to output extra info for. From string.
 * @param contextUObject The `UObject*` used to output extra info.
 * @param string String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_LOG_STR_UOBJECT(contextUObject, categoryName, verbosity, string) { const auto& refBoundString = string; GC_LOG_FMT_UOBJECT(contextUObject, categoryName, verbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

/**
 * @brief Conditional log with a `UObject` for context to output extra info for. Printf style.
 * @param contextUObject The `UObject*` used to output extra info.
 */
#define GC_CLOG_FMT_UOBJECT(contextUObject, condition, categoryName, verbosity, format, ...) GC_CLOG_FMT_NO_CONTEXT(condition, categoryName, verbosity, format TEXT(" ") GC_PRIVATE_INFO_UOBJECT_FORMAT_STRING_LITERAL TEXT(" %s"), __VA_ARGS__ __VA_OPT__(,) TEXT(PREPROCESSOR_TO_STRING(contextUObject)), GCUtils::Log::GetUObjectLogInfoString(contextUObject).ToString())

/**
 * @brief Conditional log with a `UObject` for context to output extra info for. From string.
 * @param contextUObject The `UObject*` used to output extra info.
 * @param string String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_CLOG_STR_UOBJECT(contextUObject, condition, categoryName, verbosity, string) { const auto& refBoundString = string; GC_CLOG_FMT_UOBJECT(contextUObject, condition, categoryName, verbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

/**
 * @brief Extra format string literal used in IInterface-context logs. For outputting info about the current context.
 * @note Internal macro! Not intended for use.
 */
#define GC_PRIVATE_INFO_IINTERFACE_FORMAT_STRING_LITERAL TEXT("[Context IInterface Arg: `%s`]")

/**
 * @brief Version of `GC_LOG_FMT_UOBJECT()` for `IInterface`s.
 */
#define GC_LOG_FMT_IINTERFACE(contextIInterface, categoryName, verbosity, format, ...) GC_LOG_FMT_NO_CONTEXT(categoryName, verbosity, format TEXT(" ") GC_PRIVATE_INFO_IINTERFACE_FORMAT_STRING_LITERAL TEXT(" %s"), __VA_ARGS__ __VA_OPT__(,) TEXT(PREPROCESSOR_TO_STRING(contextIInterface)), GCUtils::Log::GetIInterfaceLogInfoString(contextIInterface).ToString())

/**
 * @brief Version of `GC_LOG_STR_UOBJECT()` for `IInterface`s.
 */
#define GC_LOG_STR_IINTERFACE(contextIInterface, categoryName, verbosity, string) { const auto& refBoundString = string; GC_LOG_FMT_IINTERFACE(contextIInterface, categoryName, verbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

/**
 * @brief Version of `GC_CLOG_FMT_UOBJECT()` for `IInterface`s.
 */
#define GC_CLOG_FMT_IINTERFACE(contextIInterface, condition, categoryName, verbosity, format, ...) GC_CLOG_FMT_NO_CONTEXT(condition, categoryName, verbosity, format TEXT(" ") GC_PRIVATE_INFO_IINTERFACE_FORMAT_STRING_LITERAL TEXT(" %s"), __VA_ARGS__ __VA_OPT__(,) TEXT(PREPROCESSOR_TO_STRING(contextIInterface)), GCUtils::Log::GetIInterfaceLogInfoString(contextIInterface).ToString())

/**
 * @brief Version of `GC_CLOG_STR_UOBJECT()` for `IInterface`s.
 */
#define GC_CLOG_STR_IINTERFACE(contextIInterface, condition, categoryName, verbosity, string) { const auto& refBoundString = string; GC_CLOG_FMT_IINTERFACE(contextIInterface, condition, categoryName, verbosity, TEXT("%.*s"), ::GetNum(refBoundString), ::GetData(refBoundString)); }

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
