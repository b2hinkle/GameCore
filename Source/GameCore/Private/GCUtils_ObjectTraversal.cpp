// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_ObjectTraversal.h"

UObject* GCUtils::ObjectTraversal::TraverseSelfToOutersToOwnerActorsBreakable(UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate)
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

UObject* GCUtils::ObjectTraversal::TraverseOutersToOwnerActorsBreakable(const UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate)
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

UObject* GCUtils::ObjectTraversal::TraverseSelfToOutersBreakable(UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate)
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

UObject* GCUtils::ObjectTraversal::TraverseOutersBreakable(const UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate)
{
    if (!inObject)
    {
        return nullptr;
    }

    return TraverseSelfToOutersBreakable(inObject->GetOuter(), inShouldBreakPredicate);
}

AActor* GCUtils::ObjectTraversal::TraverseSelfToOwnerActorsBreakable(AActor* inActor, const TFunctionRef<bool(AActor&)>& inShouldBreakPredicate)
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

AActor* GCUtils::ObjectTraversal::TraverseOwnerActorsBreakable(const AActor* inActor, const TFunctionRef<bool(AActor&)>& inShouldBreakPredicate)
{
    if (!inActor)
    {
        return nullptr;
    }

    return TraverseSelfToOwnerActorsBreakable(inActor->GetOwner(), inShouldBreakPredicate);
}

UObject* GCUtils::ObjectTraversal::GetSelfOrOuterOrOwnerActorByInterface(UObject* inObject, const TSubclassOf<UInterface>& inTargetClass)
{
    return TraverseSelfToOutersToOwnerActorsBreakable(inObject,
        [&inTargetClass](UObject& object) -> bool
        {
            check(object.GetClass());
            return object.GetClass()->ImplementsInterface(inTargetClass);
        });
}

UObject* GCUtils::ObjectTraversal::GetOuterOrOwnerActorByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass)
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

AActor* GCUtils::ObjectTraversal::GetOwnerActorForObject(const UObject* inObject)
{
    UObject* result = GetOuterByClass(inObject, AActor::StaticClass());
    check(!(result && result->IsA<AActor>() == false));
    return static_cast<AActor*>(result);
}

AActor* GCUtils::ObjectTraversal::GetOwnerActorForObjectByClass(const UObject* inObject, const TSubclassOf<AActor>& inTargetClass)
{
    AActor* actor = GetOwnerActorForObject(inObject);
    return GetSelfOrOwnerActorByClass(actor, inTargetClass);
}

AActor* GCUtils::ObjectTraversal::GetOwnerActorForObjectByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass)
{
    AActor* actor = GetOwnerActorForObject(inObject);
    return GetSelfOrOwnerActorByInterface(actor, inTargetClass);
}

AActor* GCUtils::ObjectTraversal::GetSelfOrOwnerActorByClass(AActor* inActor, const TSubclassOf<AActor>& inTargetClass)
{
    return TraverseSelfToOwnerActorsBreakable(inActor,
        [&inTargetClass](AActor& actor) -> bool
        {
            return actor.IsA(inTargetClass);
        });
}

AActor* GCUtils::ObjectTraversal::GetSelfOrOwnerActorByInterface(AActor* inActor, const TSubclassOf<UInterface>& inTargetClass)
{
    return TraverseSelfToOwnerActorsBreakable(inActor,
        [&inTargetClass](AActor& actor) -> bool
        {
            check(actor.GetClass());
            return actor.GetClass()->ImplementsInterface(inTargetClass);
        });
}

AActor* GCUtils::ObjectTraversal::GetOwnerActorByClass(const AActor* inActor, const TSubclassOf<AActor>& inTargetClass)
{
    if (!inActor)
    {
        return nullptr;
    }

    return GetSelfOrOwnerActorByClass(inActor->GetOwner(), inTargetClass);
}

AActor* GCUtils::ObjectTraversal::GetOwnerActorByInterface(const AActor* inActor, const TSubclassOf<UInterface>& inTargetClass)
{
    if (!inActor)
    {
        return nullptr;
    }

    return GetSelfOrOwnerActorByInterface(inActor->GetOwner(), inTargetClass);
}

UObject* GCUtils::ObjectTraversal::GetSelfOrOuterByClass(UObject* inObject, const TSubclassOf<UObject>& inTargetClass)
{
    return TraverseSelfToOutersBreakable(inObject,
        [&inTargetClass](UObject& object) -> bool
        {
            return object.IsA(inTargetClass);
        });
}

UObject* GCUtils::ObjectTraversal::GetSelfOrOuterByInterface(UObject* inObject, const TSubclassOf<UInterface>& inTargetClass)
{
    return TraverseSelfToOutersBreakable(inObject,
        [&inTargetClass](UObject& object) -> bool
        {
            check(object.GetClass());
            return object.GetClass()->ImplementsInterface(inTargetClass);
        });
}

UObject* GCUtils::ObjectTraversal::GetOuterByClass(const UObject* inObject, const TSubclassOf<UObject>& inTargetClass)
{
    if (!inObject)
    {
        return nullptr;
    }

    return GetSelfOrOuterByClass(inObject->GetOuter(), inTargetClass);
}

UObject* GCUtils::ObjectTraversal::GetOuterByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass)
{
    if (!inObject)
    {
        return nullptr;
    }

    return GetSelfOrOuterByInterface(inObject->GetOuter(), inTargetClass);
}
