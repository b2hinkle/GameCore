// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_ObjectTraversal.h"

const UObject* GCUtils::ObjectTraversal::TraverseSelfToOutersToOwnerActorsBreakable(const UObject* inObject, const TFunctionRef<bool(const UObject&)>& inShouldBreakPredicate)
{
    const UObject* objectBrokenOn = nullptr;

    TraverseSelfToOutersBreakable(inObject,
        [&inShouldBreakPredicate, &objectBrokenOn](const UObject& outer) -> bool
        {
            if (const AActor* actor = Cast<AActor>(&outer))
            {
                // We've reached an actor in the outer chain. Stop traversing outers and
                // switch to traversing the actor owner chain.
                TraverseSelfToOwnerActorsBreakable(actor,
                    [&inShouldBreakPredicate, &objectBrokenOn](const AActor& owner) -> bool
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

UObject* GCUtils::ObjectTraversal::TraverseSelfToOutersToOwnerActorsBreakable(UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate)
{
    return const_cast<UObject*>(
        TraverseSelfToOutersToOwnerActorsBreakable(
            const_cast<const UObject*>(inObject),
            [&inShouldBreakPredicate](const UObject& object)
            {
                return inShouldBreakPredicate(const_cast<UObject&>(object));
            }
            )
        );
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

const UObject* GCUtils::ObjectTraversal::TraverseSelfToOutersBreakable(const UObject* inObject, const TFunctionRef<bool(const UObject&)>& inShouldBreakPredicate)
{
    for (const UObject* outer = inObject; outer; outer = outer->GetOuter())
    {
        if (inShouldBreakPredicate(*outer))
        {
            return outer;
        }
    }

    return nullptr;
}

UObject* GCUtils::ObjectTraversal::TraverseSelfToOutersBreakable(UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate)
{
    return const_cast<UObject*>(
        TraverseSelfToOutersBreakable(
            const_cast<const UObject*>(inObject),
            [&inShouldBreakPredicate](const UObject& object)
            {
                return inShouldBreakPredicate(const_cast<UObject&>(object));
            }
            )
        );
}

UObject* GCUtils::ObjectTraversal::TraverseOutersBreakable(const UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate)
{
    if (!inObject)
    {
        return nullptr;
    }

    return TraverseSelfToOutersBreakable(inObject->GetOuter(), inShouldBreakPredicate);
}

const AActor* GCUtils::ObjectTraversal::TraverseSelfToOwnerActorsBreakable(const AActor* inActor, const TFunctionRef<bool(const AActor&)>& inShouldBreakPredicate)
{
    for (const AActor* owner = inActor; owner; owner = owner->GetOwner())
    {
        if (inShouldBreakPredicate(*owner))
        {
            return owner;
        }
    }

    return nullptr;
}

AActor* GCUtils::ObjectTraversal::TraverseSelfToOwnerActorsBreakable(AActor* inActor, const TFunctionRef<bool(AActor&)>& inShouldBreakPredicate)
{
    return const_cast<AActor*>(
        TraverseSelfToOwnerActorsBreakable(
            const_cast<const AActor*>(inActor),
            [&inShouldBreakPredicate](const AActor& actor)
            {
                return inShouldBreakPredicate(const_cast<AActor&>(actor));
            }
            )
        );
}

AActor* GCUtils::ObjectTraversal::TraverseOwnerActorsBreakable(const AActor* inActor, const TFunctionRef<bool(AActor&)>& inShouldBreakPredicate)
{
    if (!inActor)
    {
        return nullptr;
    }

    return TraverseSelfToOwnerActorsBreakable(inActor->GetOwner(), inShouldBreakPredicate);
}

AActor* GCUtils::ObjectTraversal::GetOwnerActorForObject(const UObject* inObject)
{
    return GetTypedOuter<AActor>(inObject);
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

const UObject* GCUtils::ObjectTraversal::GetSelfOrOuterOrOwnerActorByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass)
{
    return TraverseSelfToOutersToOwnerActorsBreakable(inObject,
        [&inTargetClass](const UObject& object) -> bool
        {
            check(object.GetClass());
            return object.GetClass()->ImplementsInterface(inTargetClass);
        });
}

UObject* GCUtils::ObjectTraversal::GetSelfOrOuterOrOwnerActorByInterface(UObject* inObject, const TSubclassOf<UInterface>& inTargetClass)
{
    return const_cast<UObject*>(GetSelfOrOuterOrOwnerActorByInterface(const_cast<const UObject*>(inObject), inTargetClass));
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

const AActor* GCUtils::ObjectTraversal::GetSelfOrOwnerActorByClass(const AActor* inActor, const TSubclassOf<AActor>& inTargetClass)
{
    return TraverseSelfToOwnerActorsBreakable(inActor,
        [&inTargetClass](const AActor& actor) -> bool
        {
            return actor.IsA(inTargetClass);
        });
}

AActor* GCUtils::ObjectTraversal::GetSelfOrOwnerActorByClass(AActor* inActor, const TSubclassOf<AActor>& inTargetClass)
{
    return const_cast<AActor*>(GetSelfOrOwnerActorByClass(const_cast<const AActor*>(inActor), inTargetClass));
}

const AActor* GCUtils::ObjectTraversal::GetSelfOrOwnerActorByInterface(const AActor* inActor, const TSubclassOf<UInterface>& inTargetClass)
{
    return TraverseSelfToOwnerActorsBreakable(inActor,
        [&inTargetClass](const AActor& actor) -> bool
        {
            check(actor.GetClass());
            return actor.GetClass()->ImplementsInterface(inTargetClass);
        });
}

AActor* GCUtils::ObjectTraversal::GetSelfOrOwnerActorByInterface(AActor* inActor, const TSubclassOf<UInterface>& inTargetClass)
{
    return const_cast<AActor*>(GetSelfOrOwnerActorByInterface(const_cast<const AActor*>(inActor), inTargetClass));
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

const UObject* GCUtils::ObjectTraversal::GetSelfOrOuterByClass(const UObject* inObject, const TSubclassOf<UObject>& inTargetClass)
{
    return TraverseSelfToOutersBreakable(inObject,
        [&inTargetClass](const UObject& object) -> bool
        {
            return object.IsA(inTargetClass);
        });
}

UObject* GCUtils::ObjectTraversal::GetSelfOrOuterByClass(UObject* inObject, const TSubclassOf<UObject>& inTargetClass)
{
    return const_cast<UObject*>(GetSelfOrOuterByClass(const_cast<const UObject*>(inObject), inTargetClass));
}

const UObject* GCUtils::ObjectTraversal::GetSelfOrOuterByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass)
{
    return TraverseSelfToOutersBreakable(inObject,
        [&inTargetClass](const UObject& object) -> bool
        {
            check(object.GetClass());
            return object.GetClass()->ImplementsInterface(inTargetClass);
        });
}

UObject* GCUtils::ObjectTraversal::GetSelfOrOuterByInterface(UObject* inObject, const TSubclassOf<UInterface>& inTargetClass)
{
    return const_cast<UObject*>(GetSelfOrOuterByInterface(const_cast<const UObject*>(inObject), inTargetClass));
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
