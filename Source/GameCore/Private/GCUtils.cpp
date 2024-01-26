// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils.h"

FString GCUtils::GetUObjectNameSafe(const UObject* inUObject)
{
    return inUObject ? inUObject->GetName() : TEXT("NULL");
}

FString GCUtils::GetUObjectFullNameSafe(const UObject* inUObject, const UObject* StopOuter, EObjectFullNameFlags Flags)
{
    return inUObject ? inUObject->GetFullName(StopOuter, Flags) : TEXT("NULL");
}
