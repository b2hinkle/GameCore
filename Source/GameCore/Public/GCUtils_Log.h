// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GCUtils.h"
#include "GCUtils_String.h"
#include <source_location>

/**
 * @brief Format string literal used in no-context log macros. To output info about the current context.
 * @note Corresponds with `GC_FORMAT_NO_CONTEXT_ARGS`.
 */
#define GC_FORMAT_NO_CONTEXT_STRING_LITERAL TEXT("[%s]")

/**
 * @brief Variadic arguments used to output info about the current context.
 * @note Arguments correspond with `GC_FORMAT_NO_CONTEXT_STRING_LITERAL`.
 */
#define GC_FORMAT_NO_CONTEXT_ARGS StringCast<TCHAR, 512>(std::source_location::current().function_name()).Get()

/**
 * @brief Log macro with no additional context to output out extra info for.
 */
#define GC_LOG_NO_CONTEXT(categoryName, verbosity, format, ...) UE_LOG(categoryName, verbosity, GC_FORMAT_NO_CONTEXT_STRING_LITERAL TEXT(" ") format, GC_FORMAT_NO_CONTEXT_ARGS __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Conditional log version of `GC_LOG_NO_CONTEXT()` macro.
 */
#define GC_CLOG_NO_CONTEXT(condition, categoryName, verbosity, format, ...) UE_CLOG(condition, categoryName, verbosity, GC_FORMAT_NO_CONTEXT_STRING_LITERAL TEXT(" ") format, GC_FORMAT_NO_CONTEXT_ARGS __VA_OPT__(,) __VA_ARGS__)

/**
 * @brief Format string literal used in UObject-context log macros. To output info about the context.
 */
#define GC_FORMAT_UOBJECT_STRING_LITERAL TEXT("[Context Object Arg: `%s`]")

/**
 * @brief Log macro with a `UObject` for context to output out extra info.
 * @param contextObject The `UObject*` used to output out extra info. May be a `UWorld`.
 */
#define GC_LOG_UOBJECT(contextObject, categoryName, verbosity, format, ...) GC_LOG_NO_CONTEXT(categoryName, verbosity, format TEXT(" ") GC_FORMAT_UOBJECT_STRING_LITERAL TEXT(" %s"), __VA_ARGS__ __VA_OPT__(,) TEXT(PREPROCESSOR_TO_STRING(contextObject)), GCUtils::Log::GetObjectLogInfoString(contextObject).ToString())

 /**
  * @brief Conditional log version of `GC_LOG_UOBJECT()` macro.
  */
#define GC_CLOG_UOBJECT(contextObject, condition, categoryName, verbosity, format, ...) GC_CLOG_NO_CONTEXT(condition, categoryName, verbosity, format TEXT(" ") GC_FORMAT_UOBJECT_STRING_LITERAL TEXT(" %s"), __VA_ARGS__ __VA_OPT__(,) TEXT(PREPROCESSOR_TO_STRING(contextObject)), GCUtils::Log::GetObjectLogInfoString(contextObject).ToString())

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
