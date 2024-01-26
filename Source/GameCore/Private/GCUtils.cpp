// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils.h"

const TCHAR* LexToString(ENetMode inNetMode)
{
    switch (inNetMode)
    {
        case ENetMode::NM_Standalone:
            return TEXT("NM_Standalone");
        case ENetMode::NM_DedicatedServer:
            return TEXT("NM_DedicatedServer");
        case ENetMode::NM_ListenServer:
            return TEXT("NM_ListenServer");
        case ENetMode::NM_Client:
            return TEXT("NM_Client");
        case ENetMode::NM_MAX:
            return TEXT("NM_MAX");
    }

    return TEXT("Unknown");
}

const TCHAR* LexToString(ENetRole inNetRole)
{
    switch (inNetRole)
    {
        case ENetRole::ROLE_None:
            return TEXT("ROLE_None");
        case ENetRole::ROLE_SimulatedProxy:
            return TEXT("ROLE_SimulatedProxy");
        case ENetRole::ROLE_AutonomousProxy:
            return TEXT("ROLE_AutonomousProxy");
        case ENetRole::ROLE_Authority:
            return TEXT("ROLE_Authority");
        case ENetRole::ROLE_MAX:
            return TEXT("ROLE_MAX");
    }

    return TEXT("Unknown");
}

FString GCUtils::GetUObjectNameSafe(const UObject* inUObject)
{
    return inUObject ? inUObject->GetName() : TEXT("NULL");
}

FString GCUtils::GetUObjectFullNameSafe(const UObject* inUObject, const UObject* StopOuter, EObjectFullNameFlags Flags)
{
    return inUObject ? inUObject->GetFullName(StopOuter, Flags) : TEXT("NULL");
}

const TCHAR* GCUtils::BoolToCString(const bool inBool)
{
    return inBool ? TEXT("True") : TEXT("False");
}

const TCHAR* GCUtils::GetWorldNetMode(const UWorld* inWorld)
{
    if (!inWorld)
    {
        return TEXT("Null World");
    }

    return LexToString(inWorld->GetNetMode());
}
