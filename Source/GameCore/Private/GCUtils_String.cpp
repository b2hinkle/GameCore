// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_String.h"

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

const FStringView GCUtils::String::GetWorldNetModeString(const UObject* inWorldContextObject)
{
    if (!inWorldContextObject)
    {
        return TEXT("Null ") GC_STRING_LITERALIZE(inWorldContextObject);
    }

    const UWorld* world = inWorldContextObject->GetWorld();

    if (!world)
    {
        return TEXT("Null ") GC_STRING_LITERALIZE(world);
    }

    return LexToString(world->GetNetMode());
}

const FStringView GCUtils::String::GetObjectLocalRoleString(const UObject* inActorContextObject)
{
    if (!inActorContextObject)
    {
        return TEXT("Null ") GC_STRING_LITERALIZE(inActorContextObject);
    }

    const AActor* contextActor = ObjectTraversal::GetTypedSelfOrOuter<AActor>(inActorContextObject);

    if (!contextActor)
    {
        return TEXT("Null ") GC_STRING_LITERALIZE(contextActor);
    }

    return LexToString(contextActor->GetLocalRole());
}

const FStringView GCUtils::String::GetIsControllerLocalString(const UObject* inControllerContextObject)
{
    if (!inControllerContextObject)
    {
        return TEXT("Null ") GC_STRING_LITERALIZE(inControllerContextObject);
    }

    const AController* controller = GetController(inControllerContextObject);
    if (!controller)
    {
        return TEXT("Null ") GC_STRING_LITERALIZE(controller);
    }

    return BoolToString(controller->IsLocalController());
}

constexpr const FStringView GCUtils::String::BoolToString(const bool inBool)
{
    return inBool ? StringTrue : StringFalse;
}
