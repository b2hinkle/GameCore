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
 * Format string to output info about a context object. Corresponds with GC_FORMAT_CONTEXT_OBJECT_ARGS().
 */
#define GC_FORMAT_CONTEXT_OBJECT_STRING_LITERAL TEXT("[Context Object Arg: `%s`] [Context Object: %s] [Context Object NetMode: %s] [Context Object NetRole: %s] [Context Object Controller: %s] [Context Object Controller Is Local: %s]")

/**
 * Variadic arguments which corresponds with GC_FORMAT_CONTEXT_OBJECT_STRING_LITERAL.
 */
#define GC_FORMAT_CONTEXT_OBJECT_ARGS(contextObject, contextObjectArgCString) contextObjectArgCString, GCUtils::String::GetUObjectNameSafe(contextObject).ToString(), GCUtils::String::GetWorldNetModeString(contextObject).GetData(), GCUtils::String::GetObjectLocalRoleString(contextObject).GetData(), GCUtils::String::GetUObjectNameSafe(GCUtils::GetController(contextObject)).ToString(), GCUtils::String::GetIsControllerLocalString(contextObject).GetData()

/**
 * @param contextObject UObject which will be used when our log gathers extra data to help us out. May be a UWorld.
 */
#define GC_LOG(contextObject, categoryName, verbosity, format, ...) GC_LOG_NO_CONTEXT(categoryName, verbosity, format TEXT(" ") GC_FORMAT_CONTEXT_OBJECT_STRING_LITERAL, __VA_ARGS__ __VA_OPT__(,) GC_FORMAT_CONTEXT_OBJECT_ARGS(contextObject, TEXT(PREPROCESSOR_TO_STRING(contextObject))))

/**
 * Conditional log version of GC_LOG().
 */
#define GC_CLOG(contextObject, condition, categoryName, verbosity, format, ...) GC_CLOG_NO_CONTEXT(condition, categoryName, verbosity, format TEXT(" ") GC_FORMAT_CONTEXT_OBJECT_STRING_LITERAL, __VA_ARGS__ __VA_OPT__(,) GC_FORMAT_CONTEXT_OBJECT_ARGS(contextObject, TEXT(PREPROCESSOR_TO_STRING(contextObject))))
