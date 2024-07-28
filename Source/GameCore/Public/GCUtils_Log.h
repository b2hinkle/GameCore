// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GCUtils.h"
#include "GCUtils_String.h"
#include <source_location>

/**
 * @brief Plain log. Printf style.
 * @note Internal macro! Not intended for use.
 */
#define GC_PRIVATE_LOG_FMT(categoryName, verbosity, format, ...) UE_LOG(categoryName, verbosity, format __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Plain log. From string.
 * @note Internal macro! Not intended for use.
 * @param string String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_PRIVATE_LOG_STR(categoryName, verbosity, string) { const auto& stringBoundToReference = string; GC_PRIVATE_LOG_FMT(categoryName, verbosity, TEXT("%.*s"), GetNum(stringBoundToReference), GetData(stringBoundToReference)); }

/**
 * @brief Plain conditional log. Printf style.
 * @note Internal macro! Not intended for use.
 */
#define GC_PRIVATE_CLOG_FMT(condition, categoryName, verbosity, format, ...) UE_CLOG(condition, categoryName, verbosity, format __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Plain conditional log. From string.
 * @note Internal macro! Not intended for use.
 * @param string String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_PRIVATE_CLOG_STR(condition, categoryName, verbosity, string) { const auto& stringBoundToReference = string; GC_PRIVATE_CLOG_FMT(condition, categoryName, verbosity, TEXT("%.*s"), GetNum(stringBoundToReference), GetData(stringBoundToReference)); }

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
#define GC_LOG_FMT_NO_CONTEXT(categoryName, verbosity, format, ...) GC_PRIVATE_LOG_FMT(categoryName, verbosity, GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_STRING_LITERAL TEXT(" ") format, GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_ARGS __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Log with no extra context to output info for. From string.
 * @param string String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_LOG_STR_NO_CONTEXT(categoryName, verbosity, string) { const auto& stringBoundToReference = string; GC_LOG_FMT_NO_CONTEXT(categoryName, verbosity, TEXT("%.*s"), GetNum(stringBoundToReference), GetData(stringBoundToReference)); }

/**
 * @brief Conditional log with no extra context to output info for. Printf style.
 */
#define GC_CLOG_FMT_NO_CONTEXT(condition, categoryName, verbosity, format, ...) GC_PRIVATE_CLOG_FMT(condition, categoryName, verbosity, GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_STRING_LITERAL TEXT(" ") format, GC_PRIVATE_INFO_NO_CONTEXT_FORMAT_ARGS __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Conditional log with no extra context to output info for. From string.
 * @param string String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_CLOG_STR_NO_CONTEXT(condition, categoryName, verbosity, string) { const auto& stringBoundToReference = string; GC_CLOG_FMT_NO_CONTEXT(condition, categoryName, verbosity, TEXT("%.*s"), GetNum(stringBoundToReference), GetData(stringBoundToReference)); }

/**
 * @brief Extra format string literal used in UObject-context logs. For outputting info about the current context.
 * @note Internal macro! Not intended for use.
 */
#define GC_PRIVATE_INFO_UOBJECT_FORMAT_STRING_LITERAL TEXT("[Context Object Arg: `%s`]")

/**
 * @brief Log with a `UObject` for context to output extra info for. Printf style.
 * @param contextObject The `UObject*` used to output extra info.
 */
#define GC_LOG_FMT_UOBJECT(contextObject, categoryName, verbosity, format, ...) GC_LOG_FMT_NO_CONTEXT(categoryName, verbosity, format TEXT(" ") GC_PRIVATE_INFO_UOBJECT_FORMAT_STRING_LITERAL TEXT(" %s"), __VA_ARGS__ __VA_OPT__(,) TEXT(PREPROCESSOR_TO_STRING(contextObject)), GCUtils::Log::GetObjectLogInfoString(contextObject).ToString())

/**
 * @brief Log with a `UObject` for context to output extra info for. From string.
 * @param contextObject The `UObject*` used to output extra info.
 * @param string String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_LOG_STR_UOBJECT(contextObject, categoryName, verbosity, string) { const auto& stringBoundToReference = string; GC_LOG_FMT_UOBJECT(contextObject, categoryName, verbosity, TEXT("%.*s"), GetNum(stringBoundToReference), GetData(stringBoundToReference)); }

/**
 * @brief Conditional log with a `UObject` for context to output extra info for. Printf style.
 * @note Internal macro! Not intended for use.
 * @param contextObject The `UObject*` used to output extra info.
 */
#define GC_CLOG_FMT_UOBJECT(contextObject, condition, categoryName, verbosity, format, ...) GC_CLOG_FMT_NO_CONTEXT(condition, categoryName, verbosity, format TEXT(" ") GC_PRIVATE_INFO_UOBJECT_FORMAT_STRING_LITERAL TEXT(" %s"), __VA_ARGS__ __VA_OPT__(,) TEXT(PREPROCESSOR_TO_STRING(contextObject)), GCUtils::Log::GetObjectLogInfoString(contextObject).ToString())

/**
 * @brief Conditional log with a `UObject` for context to output extra info for. From string.
 * @param contextObject The `UObject*` used to output extra info.
 * @param string String to output. Overloads of functions `::GetNum()` and `::GetData()` must exist for it.
 */
#define GC_CLOG_STR_UOBJECT(contextObject, condition, categoryName, verbosity, string) { const auto& stringBoundToReference = string; GC_CLOG_FMT_UOBJECT(contextObject, condition, categoryName, verbosity, TEXT("%.*s"), GetNum(stringBoundToReference), GetData(stringBoundToReference)); }

namespace GCUtils::Log
{
    /**
     * @brief Build a debug string given a `UObject`.
     */
    template <int32 bufferLength = 1024>
    TStringBuilder<bufferLength> GetObjectLogInfoString(const UObject* object);
}

template <int32 bufferLength>
TStringBuilder<bufferLength> GCUtils::Log::GetObjectLogInfoString(const UObject* object)
{
    return WriteToString<bufferLength>(
        TEXT("[Context Object: "), GCUtils::String::GetUObjectNameSafe(object), TEXT(']'),
        TEXT(' '),
        TEXT("[Context Object NetMode: "), GCUtils::String::GetWorldNetModeString(object), TEXT(']'),
        TEXT(' '),
        TEXT("[Context Object NetRole: "), GCUtils::String::GetObjectLocalRoleString(object), TEXT(']'),
        TEXT(' '),
        TEXT("[Context Object Controller: "), GCUtils::String::GetUObjectNameSafe(GCUtils::GetController(object)), TEXT(']'),
        TEXT(' '),
        TEXT("[Context Object Controller Is Local: "), GCUtils::String::GetIsControllerLocalString(object), TEXT(']')
    );
}
