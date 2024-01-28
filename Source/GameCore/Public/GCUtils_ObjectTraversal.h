// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace GCUtils::ObjectTraversal
{
    // Begin Traversal Functions.

    /**
     * Traverses the UObject outer chain then the AActor owner chain. Including self.
     *
     * Breaks when shouldBreakPredicate returns true.
     *
     * Returns object/actor that was broken on or nullptr.
     */
    GAMECORE_API UObject* TraverseSelfToOutersToOwnerActorsBreakable(UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate);

    /**
     * Same as TraverseSelfToOutersToOwnerActorsBreakable() but doesn't include self.
     */
    GAMECORE_API UObject* TraverseOutersToOwnerActorsBreakable(const UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate);

    /**
     * Traverses the UObject outer chain. Including self.
     *
     * Breaks when shouldBreakPredicate returns true.
     *
     * Returns object that was broken on or nullptr.
     */
    GAMECORE_API UObject* TraverseSelfToOutersBreakable(UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate);

    /**
     * Same as TraverseSelfToOutersBreakable() but doesn't include self.
     */
    GAMECORE_API UObject* TraverseOutersBreakable(const UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate);

    /**
     * Traverses the AActor owner chain. Including self.
     *
     * Breaks when shouldBreakPredicate returns true.
     *
     * Returns actor that was broken on or nullptr.
     */
    GAMECORE_API AActor* TraverseSelfToOwnerActorsBreakable(AActor* inActor, const TFunctionRef<bool(AActor&)>& inShouldBreakPredicate);

    /**
     * Same as TraverseSelfToOwnerActorsBreakable() but doesn't include self.
     */
    GAMECORE_API AActor* TraverseOwnerActorsBreakable(const AActor* inActor, const TFunctionRef<bool(AActor&)>& inShouldBreakPredicate);

    // End Traversal Functions.

    /**
     * Traverse the UObject outer chain then the AActor owner chain to return the
     * first thing that implements the interface. Including self.
     */
    GAMECORE_API UObject* GetSelfOrOuterOrOwnerActorByInterface(UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    /**
     * Same as GetSelfOrOuterOrOwnerActorByInterface() but doesn't include self.
     */
    GAMECORE_API UObject* GetOuterOrOwnerActorByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    /**
     * Traverse the UObject outer chain to return the first AActor that we find.
     *
     * Same result as UActorComponent::GetOwner() when used on an actor component.
     */
    GAMECORE_API AActor* GetOwnerActorForObject(const UObject* inObject);

    GAMECORE_API AActor* GetOwnerActorForObjectByClass(const UObject* inObject, const TSubclassOf<AActor>& inTargetClass);
    GAMECORE_API AActor* GetOwnerActorForObjectByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    GAMECORE_API AActor* GetSelfOrOwnerActorByClass(AActor* inActor, const TSubclassOf<AActor>& inTargetClass);
    GAMECORE_API AActor* GetSelfOrOwnerActorByInterface(AActor* inActor, const TSubclassOf<UInterface>& inTargetClass);

    GAMECORE_API AActor* GetOwnerActorByClass(const AActor* inActor, const TSubclassOf<AActor>& inTargetClass);
    GAMECORE_API AActor* GetOwnerActorByInterface(const AActor* inActor, const TSubclassOf<UInterface>& inTargetClass);

    GAMECORE_API UObject* GetSelfOrOuterByClass(UObject* inObject, const TSubclassOf<UObject>& inTargetClass);
    GAMECORE_API UObject* GetSelfOrOuterByInterface(UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    GAMECORE_API UObject* GetOuterByClass(const UObject* inObject, const TSubclassOf<UObject>& inTargetClass);
    GAMECORE_API UObject* GetOuterByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);
}
