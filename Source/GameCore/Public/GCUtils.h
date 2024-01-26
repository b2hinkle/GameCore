// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace GCUtils
{
    inline constexpr const TCHAR* CStringNull = TEXT("NULL");

    FString GetUObjectNameSafe(const UObject* inUObject);

    FString GetUObjectFullNameSafe(const UObject* inUObject, const UObject* StopOuter=NULL, EObjectFullNameFlags Flags = EObjectFullNameFlags::None);
}
