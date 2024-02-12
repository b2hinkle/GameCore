// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GCUtils.h"
#include <source_location>

/**
 * If you can't get ahold of a UObject, you may need to use this.
 */
#define GC_LOG_NO_CONTEXT(categoryName, verbosity, format, ...) UE_LOG(categoryName, verbosity, TEXT("[%s] ") format, StringCast<TCHAR>(std::source_location::current().function_name()).Get() __VA_OPT__(,) __VA_ARGS__)

/**
 * Conditional log version of GC_LOG_NO_CONTEXT().
 */
#define GC_CLOG_NO_CONTEXT(condition, categoryName, verbosity, format, ...) UE_CLOG(condition, categoryName, verbosity, TEXT("[%s] ") format, StringCast<TCHAR>(std::source_location::current().function_name()).Get() __VA_OPT__(,) __VA_ARGS__)

/**
 * Format cstring to print info of a context object.
 */
#define GC_CONTEXT_OBJECT_FORMAT_CSTRING TEXT("[Context Object: %s] [Context World NetMode: %s]")

/**
 * Variadic arguments that correspond with GC_CONTEXT_OBJECT_FORMAT_CSTRING.
 */
#define GC_CONTEXT_OBJECT_FORMAT_ARGS(contextObject) *GCUtils::GetUObjectNameSafe(contextObject), GCUtils::GetWorldNetModeCString(contextObject)

/**
 * @param contextObject - UObject which will be used when our log gathers extra data to help us out. May be a UWorld.
 */
#define GC_LOG(contextObject, categoryName, verbosity, format, ...) GC_LOG_NO_CONTEXT(categoryName, verbosity, format TEXT(" ") GC_CONTEXT_OBJECT_FORMAT_CSTRING, __VA_ARGS__ __VA_OPT__(,) GC_CONTEXT_OBJECT_FORMAT_ARGS(contextObject))

/**
 * Conditional log version of GC_LOG().
 */
#define GC_CLOG(contextObject, condition, categoryName, verbosity, format, ...) GC_CLOG_NO_CONTEXT(condition, categoryName, verbosity, format TEXT(" ") GC_CONTEXT_OBJECT_FORMAT_CSTRING, __VA_ARGS__ __VA_OPT__(,) GC_CONTEXT_OBJECT_FORMAT_ARGS(contextObject))

namespace GCUtils::Log
{
}
