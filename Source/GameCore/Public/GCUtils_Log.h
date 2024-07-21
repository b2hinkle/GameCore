// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GCUtils.h"
#include "GCUtils_String.h"
#include <source_location>

/**
 * Format string to output info about the current context. Corresponds with GC_FORMAT_NO_CONTEXT_ARGS.
 */
#define GC_FORMAT_NO_CONTEXT_STRING_LITERAL TEXT("[%s]")

/**
 * Variadic arguments which corresponds with GC_FORMAT_NO_CONTEXT_STRING_LITERAL.
 */
#define GC_FORMAT_NO_CONTEXT_ARGS StringCast<TCHAR, 512>(std::source_location::current().function_name()).Get()

/**
 * If you can't get ahold of a UObject, you may need to use this.
 */
#define GC_LOG_NO_CONTEXT(categoryName, verbosity, format, ...) UE_LOG(categoryName, verbosity, GC_FORMAT_NO_CONTEXT_STRING_LITERAL TEXT(" ") format, GC_FORMAT_NO_CONTEXT_ARGS __VA_OPT__(,) __VA_ARGS__)

/**
 * Conditional log version of GC_LOG_NO_CONTEXT().
 */
#define GC_CLOG_NO_CONTEXT(condition, categoryName, verbosity, format, ...) UE_CLOG(condition, categoryName, verbosity, GC_FORMAT_NO_CONTEXT_STRING_LITERAL TEXT(" ") format, GC_FORMAT_NO_CONTEXT_ARGS __VA_OPT__(,) __VA_ARGS__)

/**
 * Format string for logging output about a context object.
 */
#define GC_FORMAT_CONTEXT_OBJECT_STRING_LITERAL TEXT("[Context Object Arg: `%s`]")

/**
 * @param contextObject UObject which will be used when our log gathers extra data to help us out. May be a UWorld.
 */
#define GC_LOG(contextObject, categoryName, verbosity, format, ...) GC_LOG_NO_CONTEXT(categoryName, verbosity, format TEXT(" ") GC_FORMAT_CONTEXT_OBJECT_STRING_LITERAL TEXT(" %s"), __VA_ARGS__ __VA_OPT__(,) TEXT(PREPROCESSOR_TO_STRING(contextObject)), GCUtils::Log::GetObjectLogInfoString(contextObject).ToString())

 /**
  * Conditional log version of GC_LOG().
  */
#define GC_CLOG(contextObject, condition, categoryName, verbosity, format, ...) GC_CLOG_NO_CONTEXT(condition, categoryName, verbosity, format TEXT(" ") GC_FORMAT_CONTEXT_OBJECT_STRING_LITERAL TEXT(" %s"), __VA_ARGS__ __VA_OPT__(,) TEXT(PREPROCESSOR_TO_STRING(contextObject)), GCUtils::Log::GetObjectLogInfoString(contextObject).ToString())

namespace GCUtils::Log
{
    /**
     * Builds a debug string given a UObject.
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
