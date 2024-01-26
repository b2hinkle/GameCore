// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Note: Using `##` before `__VA_ARGS__` is not officially supported by the standard but is supported by
// major compilers. UE_LOG() uses though so we will do the same.

// TODO: Output netmode and as much info as we can in these logs.

#define GC_LOG(categoryName, verbosity, format, ...) UE_LOG(categoryName, verbosity, TEXT("[%s] ") format, ANSI_TO_TCHAR(__func__), ##__VA_ARGS__)

#define GC_CLOG(condition, categoryName, verbosity, format, ...) UE_CLOG(condition, categoryName, verbosity, TEXT("[%s] ") format, ANSI_TO_TCHAR(__func__), ##__VA_ARGS__)

namespace GCLogUtils
{
    inline constexpr const TCHAR* CStringNull = TEXT("NULL");
}
