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

UObject* GCUtils::TraverseSelfToOutersToOwnerActorsBreakable(UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate)
{
    UObject* objectBrokenOn = nullptr;

    TraverseSelfToOutersBreakable(inObject,
        [&inShouldBreakPredicate, &objectBrokenOn](UObject& outer) -> bool
        {
            if (AActor* actor = Cast<AActor>(&outer))
            {
                // We've reached an actor in the outer chain. Stop traversing outers and
                // switch to traversing the actor owner chain.
                TraverseSelfToOwnerActorsBreakable(actor,
                    [&inShouldBreakPredicate, &objectBrokenOn](AActor& owner) -> bool
                    {
                        if (inShouldBreakPredicate(owner))
                        {
                            objectBrokenOn = &owner;
                            return true;
                        }

                        return false;
                    });

                // Don't continue with traversing the outer chain.
                return true;
            }

            if (inShouldBreakPredicate(outer))
            {
                objectBrokenOn = &outer;
                return true;
            }

            return false;
        });

    return objectBrokenOn;
}

UObject* GCUtils::TraverseOutersToOwnerActorsBreakable(const UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate)
{
    if (!inObject)
    {
        return nullptr;
    }

    if (const AActor* actor = Cast<AActor>(inObject))
    {
        return TraverseSelfToOutersToOwnerActorsBreakable(actor->GetOwner(), inShouldBreakPredicate);
    }

    return TraverseSelfToOutersToOwnerActorsBreakable(inObject->GetOuter(), inShouldBreakPredicate);
}

UObject* GCUtils::TraverseSelfToOutersBreakable(UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate)
{
    for (UObject* outer = inObject; outer; outer = outer->GetOuter())
    {
        if (inShouldBreakPredicate(*outer))
        {
            return outer;
        }
    }

    return nullptr;
}

UObject* GCUtils::TraverseOutersBreakable(const UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate)
{
    if (!inObject)
    {
        return nullptr;
    }

    return TraverseSelfToOutersBreakable(inObject->GetOuter(), inShouldBreakPredicate);
}

AActor* GCUtils::TraverseSelfToOwnerActorsBreakable(AActor* inActor, const TFunctionRef<bool(AActor&)>& inShouldBreakPredicate)
{
    for (AActor* owner = inActor; owner; owner = owner->GetOwner())
    {
        if (inShouldBreakPredicate(*owner))
        {
            return owner;
        }
    }

    return nullptr;
}

AActor* GCUtils::TraverseOwnerActorsBreakable(const AActor* inActor, const TFunctionRef<bool(AActor&)>& inShouldBreakPredicate)
{
    if (!inActor)
    {
        return nullptr;
    }

    return TraverseSelfToOwnerActorsBreakable(inActor->GetOwner(), inShouldBreakPredicate);
}

UObject* GCUtils::GetSelfOrOuterOrOwnerActorByInterface(UObject* inObject, const TSubclassOf<UInterface>& inTargetClass)
{
    return TraverseSelfToOutersToOwnerActorsBreakable(inObject,
        [&inTargetClass](UObject& object) -> bool
        {
            check(object.GetClass());
            return object.GetClass()->ImplementsInterface(inTargetClass);
        });
}

UObject* GCUtils::GetOuterOrOwnerActorByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass)
{
    if (!inObject)
    {
        return nullptr;
    }

    if (const AActor* actor = Cast<AActor>(inObject))
    {
        return GetSelfOrOuterOrOwnerActorByInterface(actor->GetOwner(), inTargetClass);
    }

    return GetSelfOrOuterOrOwnerActorByInterface(inObject->GetOuter(), inTargetClass);
}

AActor* GCUtils::GetOwnerActorForObject(const UObject* inObject)
{
    UObject* result = GetOuterByClass(inObject, AActor::StaticClass());
    check(!(result && result->IsA<AActor>() == false));
    return static_cast<AActor*>(result);
}

AActor* GCUtils::GetOwnerActorForObjectByClass(const UObject* inObject, const TSubclassOf<AActor>& inTargetClass)
{
    AActor* actor = GetOwnerActorForObject(inObject);
    return GetSelfOrOwnerActorByClass(actor, inTargetClass);
}

AActor* GCUtils::GetOwnerActorForObjectByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass)
{
    AActor* actor = GetOwnerActorForObject(inObject);
    return GetSelfOrOwnerActorByInterface(actor, inTargetClass);
}

AActor* GCUtils::GetSelfOrOwnerActorByClass(AActor* inActor, const TSubclassOf<AActor>& inTargetClass)
{
    return TraverseSelfToOwnerActorsBreakable(inActor,
        [&inTargetClass](AActor& actor) -> bool
        {
            return actor.IsA(inTargetClass);
        });
}

AActor* GCUtils::GetSelfOrOwnerActorByInterface(AActor* inActor, const TSubclassOf<UInterface>& inTargetClass)
{
    return TraverseSelfToOwnerActorsBreakable(inActor,
        [&inTargetClass](AActor& actor) -> bool
        {
            check(actor.GetClass());
            return actor.GetClass()->ImplementsInterface(inTargetClass);
        });
}

AActor* GCUtils::GetOwnerActorByClass(const AActor* inActor, const TSubclassOf<AActor>& inTargetClass)
{
    if (!inActor)
    {
        return nullptr;
    }

    return GetSelfOrOwnerActorByClass(inActor->GetOwner(), inTargetClass);
}

AActor* GCUtils::GetOwnerActorByInterface(const AActor* inActor, const TSubclassOf<UInterface>& inTargetClass)
{
    if (!inActor)
    {
        return nullptr;
    }

    return GetSelfOrOwnerActorByInterface(inActor->GetOwner(), inTargetClass);
}

UObject* GCUtils::GetSelfOrOuterByClass(UObject* inObject, const TSubclassOf<UObject>& inTargetClass)
{
    return TraverseSelfToOutersBreakable(inObject,
        [&inTargetClass](UObject& object) -> bool
        {
            return object.IsA(inTargetClass);
        });
}

UObject* GCUtils::GetSelfOrOuterByInterface(UObject* inObject, const TSubclassOf<UInterface>& inTargetClass)
{
    return TraverseSelfToOutersBreakable(inObject,
        [&inTargetClass](UObject& object) -> bool
        {
            check(object.GetClass());
            return object.GetClass()->ImplementsInterface(inTargetClass);
        });
}

UObject* GCUtils::GetOuterByClass(const UObject* inObject, const TSubclassOf<UObject>& inTargetClass)
{
    if (!inObject)
    {
        return nullptr;
    }

    return GetSelfOrOuterByClass(inObject->GetOuter(), inTargetClass);
}

UObject* GCUtils::GetOuterByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass)
{
    if (!inObject)
    {
        return nullptr;
    }

    return GetSelfOrOuterByInterface(inObject->GetOuter(), inTargetClass);
}
