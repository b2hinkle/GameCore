// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils.h"

#include "GCUtils_ObjectTraversal.h"

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
    return inUObject ? inUObject->GetName() : CStringNull;
}

FString GCUtils::GetUObjectFullNameSafe(const UObject* inUObject, const UObject* inStopOuter, EObjectFullNameFlags inFlags)
{
    return inUObject ? inUObject->GetFullName(inStopOuter, inFlags) : CStringNull;
}

const TCHAR* GCUtils::BoolToCString(const bool inBool)
{
    return inBool ? TEXT("True") : TEXT("False");
}

const TCHAR* GCUtils::GetWorldNetModeCString(const UObject* inWorldContextObject)
{
    if (!inWorldContextObject)
    {
        return TEXT("Null ") GC_CSTRINGIZE(inWorldContextObject);
    }

    const UWorld* world = GetWorldSafe(inWorldContextObject);

    if (!world)
    {
        return TEXT("Null ") GC_CSTRINGIZE(world);
    }

    return LexToString(world->GetNetMode());
}

const TCHAR* GCUtils::GetObjectLocalRoleCString(const UObject* inContextObject)
{
    if (!inContextObject)
    {
        return TEXT("Null ") GC_CSTRINGIZE(inContextObject);
    }

    const AActor* contextActor = ObjectTraversal::GetTypedSelfOrOuter<AActor>(inContextObject);

    if (!contextActor)
    {
        return TEXT("Null ") GC_CSTRINGIZE(contextActor);
    }

    return LexToString(contextActor->GetLocalRole());
}

UWorld* GCUtils::GetWorldSafe(const UObject* inContextObject)
{
    return inContextObject ? inContextObject->GetWorld() : nullptr;
}

const AController* GCUtils::GetController(const UObject* inObject)
{
    const AActor* actor = ObjectTraversal::GetTypedSelfOrOuter<AActor>(inObject);
    return ObjectTraversal::GetTypedSelfOrOwnerActor<AController>(actor);
}
AController* GCUtils::GetController(UObject* inObject)
{
    return const_cast<AController*>(GetController(const_cast<const UObject*>(inObject)));
}
