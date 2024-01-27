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
    return inUObject ? inUObject->GetName() : CStringNull;
}

FString GCUtils::GetUObjectFullNameSafe(const UObject* inUObject, const UObject* StopOuter, EObjectFullNameFlags Flags)
{
    return inUObject ? inUObject->GetFullName(StopOuter, Flags) : CStringNull;
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
        return TEXT("Null") GC_CSTRINGIZE(world);
    }

    return LexToString(world->GetNetMode());
}

UWorld* GCUtils::GetWorldSafe(const UObject* inObject)
{
    return inObject ? inObject->GetWorld() : nullptr;
}

UObject* GCUtils::GetInterfaceTypedOuterOrOwnerActor(const UObject* inObject, TSubclassOf<UInterface> inTargetClass)
{
    if (!inObject)
    {
        return nullptr;
    }

    for (UObject* outer = inObject->GetOuter(); outer; outer = outer->GetOuter())
    {
        AActor* actor = Cast<AActor>(outer);
        if (actor)
        {
            // We've reached an actor in the outer chain. Stop traversing outers and
            // switch to traversing the actor owner chain.
            for (AActor* owner = actor; owner; owner = owner->GetOwner())
            {
                check(owner->GetClass());
                if (owner->GetClass()->ImplementsInterface(inTargetClass))
                {
                    return owner;
                }
            }

            // Don't continue with traversing the outer chain.
            return nullptr;
        }

        check(outer->GetClass());
        if (outer->GetClass()->ImplementsInterface(inTargetClass))
        {
            return outer;
        }
    }

    return nullptr;
}

AActor* GCUtils::GetOuterActor(const UObject* inObject)
{
    if (!inObject)
    {
        return nullptr;
    }

    return inObject->GetTypedOuter<AActor>();
}

AActor* GCUtils::GetTypedOwnerActorForObject(const UObject* inObject, TSubclassOf<AActor> inTargetClass)
{
    AActor* actor = GetOuterActor(inObject);
    if (!actor)
    {
        return nullptr;
    }

    return GetTypedOwnerActorIncludingSelf(actor, inTargetClass);
}

AActor* GCUtils::GetInterfaceTypedOwnerActorForObject(const UObject* inObject, TSubclassOf<UInterface> inTargetClass)
{
    AActor* actor = GetOuterActor(inObject);
    if (!actor)
    {
        return nullptr;
    }

    return GetInterfaceTypedOwnerActorIncludingSelf(actor, inTargetClass);
}

UObject* GCUtils::GetTypedOuter(const UObject* inObject, UClass* inTargetClass)
{
    if (!inObject)
    {
        return nullptr;
    }

    return inObject->GetTypedOuter(inTargetClass);
}

UObject* GCUtils::GetTypedOuterIncludingSelf(UObject* inObject, UClass* inTargetClass)
{
    if (!inObject)
    {
        return nullptr;
    }

    if (inObject->IsA(inTargetClass))
    {
        return inObject;
    }

    return inObject->GetTypedOuter(inTargetClass);
}

UObject* GCUtils::GetInterfaceTypedOuter(const UObject* inObject, TSubclassOf<UInterface> inTargetClass)
{
    if (!inObject)
    {
        return nullptr;
    }

    // Use some similar code to UObjectBaseUtility::GetTypedOuter().
    for (UObject* outer = inObject->GetOuter(); outer; outer = outer->GetOuter())
    {
        // TODO: Check if we can do IsA<>().
        check(outer->GetClass());
        if (outer->GetClass()->ImplementsInterface(inTargetClass))
        {
            return outer;
        }
    }

    return nullptr;
}

UObject* GCUtils::GetInterfaceTypedOuterIncludingSelf(UObject* inObject, TSubclassOf<UInterface> inTargetClass)
{
    if (!inObject)
    {
        return nullptr;
    }

    check(inObject->GetClass());
    if (inObject->GetClass()->ImplementsInterface(inTargetClass))
    {
        return inObject;
    }

    return GetInterfaceTypedOuter(inObject, inTargetClass);
}

AActor* GCUtils::GetTypedOwnerActor(const AActor* inActor, TSubclassOf<AActor> inTargetClass)
{
    if (!inActor)
    {
        return nullptr;
    }

    for (AActor* owner = inActor->GetOwner(); owner; owner = owner->GetOwner())
    {
        if (owner->IsA(inTargetClass))
        {
            return owner;
        }
    }

    return nullptr;
}

AActor* GCUtils::GetTypedOwnerActorIncludingSelf(AActor* inActor, TSubclassOf<AActor> inTargetClass)
{
    if (!inActor)
    {
        return nullptr;
    }

    if (inActor->IsA(inTargetClass))
    {
        return inActor;
    }

    return GetTypedOwnerActor(inActor, inTargetClass);
}

AActor* GCUtils::GetInterfaceTypedOwnerActor(const AActor* inActor, TSubclassOf<UInterface> inTargetClass)
{
    if (!inActor)
    {
        return nullptr;
    }

    for (AActor* owner = inActor->GetOwner(); owner; owner = owner->GetOwner())
    {
        check(owner->GetClass());
        if (owner->GetClass()->ImplementsInterface(inTargetClass))
        {
            return owner;
        }
    }

    return nullptr;
}

AActor* GCUtils::GetInterfaceTypedOwnerActorIncludingSelf(AActor* inActor, TSubclassOf<UInterface> inTargetClass)
{
    if (!inActor)
    {
        return nullptr;
    }

    check(inActor->GetClass());
    if (inActor->GetClass()->ImplementsInterface(inTargetClass))
    {
        return inActor;
    }


    return GetInterfaceTypedOwnerActor(inActor, inTargetClass);
}
