// Fill out your copyright notice in the Description page of Project Settings.

#include "GCLogUtils.h"

FString GCLogUtils::GetUObjectNameSafe(const UObject* inUObject)
{
    return inUObject ? inUObject->GetName() : TEXT("NULL");
}

FString GCLogUtils::GetUObjectFullNameSafe(const UObject* inUObject, const UObject* StopOuter, EObjectFullNameFlags Flags)
{
    return inUObject ? inUObject->GetFullName() : TEXT("NULL");
}
