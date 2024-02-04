// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"

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
     * Traverse the UObject outer chain to return the first AActor that we find.
     *
     * Same result as UActorComponent::GetOwner() when used on an actor component.
     */
    GAMECORE_API AActor* GetOwnerActorForObject(const UObject* inObject);

    template <class TTargetClass>
    TTargetClass* GetTypedOwnerActorForObject(const UObject* inObject);
    GAMECORE_API AActor* GetOwnerActorForObjectByClass(const UObject* inObject, const TSubclassOf<AActor>& inTargetClass);
    GAMECORE_API AActor* GetOwnerActorForObjectByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    /**
     * Traverse the UObject outer chain then the AActor owner chain to return the
     * first thing that implements the interface. Including self.
     */
    template
        <
        class TTargetClass,
        class = typename TEnableIf<TIsIInterface<TTargetClass>::Value>::Type
        >
    TTargetClass* GetTypedSelfOrOuterOrOwnerActorByInterface(UObject* inObject);
    GAMECORE_API UObject* GetSelfOrOuterOrOwnerActorByInterface(UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    /**
     * Same as GetSelfOrOuterOrOwnerActorByInterface() but doesn't include self.
     */
    template
        <
        class TTargetClass,
        class = typename TEnableIf<TIsIInterface<TTargetClass>::Value>::Type
        >
    TTargetClass* GetTypedOuterOrOwnerActorByInterface(const UObject* inObject);
    GAMECORE_API UObject* GetOuterOrOwnerActorByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    template <class TTargetClass>
    TTargetClass* GetTypedSelfOrOwnerActor(AActor* inActor);
    GAMECORE_API AActor* GetSelfOrOwnerActorByClass(AActor* inActor, const TSubclassOf<AActor>& inTargetClass);
    GAMECORE_API AActor* GetSelfOrOwnerActorByInterface(AActor* inActor, const TSubclassOf<UInterface>& inTargetClass);

    template <class TTargetClass>
    TTargetClass* GetTypedOwnerActor(const AActor* inActor);
    GAMECORE_API AActor* GetOwnerActorByClass(const AActor* inActor, const TSubclassOf<AActor>& inTargetClass);
    GAMECORE_API AActor* GetOwnerActorByInterface(const AActor* inActor, const TSubclassOf<UInterface>& inTargetClass);

    template <class TTargetClass>
    TTargetClass* GetTypedSelfOrOuter(UObject* inObject);
    GAMECORE_API UObject* GetSelfOrOuterByClass(UObject* inObject, const TSubclassOf<UObject>& inTargetClass);
    GAMECORE_API UObject* GetSelfOrOuterByInterface(UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    template <class TTargetClass>
    TTargetClass* GetTypedOuter(const UObject* inObject);
    GAMECORE_API UObject* GetOuterByClass(const UObject* inObject, const TSubclassOf<UObject>& inTargetClass);
    GAMECORE_API UObject* GetOuterByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);
}

template <class TTargetClass>
TTargetClass* GCUtils::ObjectTraversal::GetTypedOwnerActorForObject(const UObject* inObject)
{
    if constexpr (TIsIInterface<TTargetClass>::Value == true)
    {
        return GCUtils::ReinterpretCastChecked<TTargetClass*>(
            GetOwnerActorForObjectByInterface(inObject, TTargetClass::UClassType::StaticClass()));
    }
    else
    {
        return GCUtils::StaticCastChecked<TTargetClass*>(
            GetOwnerActorForObjectByClass(inObject, TTargetClass::StaticClass()));
    }
}

template
    <
    class TTargetClass,
    class
    >
TTargetClass* GCUtils::ObjectTraversal::GetTypedSelfOrOuterOrOwnerActorByInterface(UObject* inObject)
{
    return GCUtils::ReinterpretCastChecked<TTargetClass*>(
        GetSelfOrOuterOrOwnerActorByInterface(inObject, TTargetClass::UClassType::StaticClass()));
}

template
    <
    class TTargetClass,
    class
    >
TTargetClass* GCUtils::ObjectTraversal::GetTypedOuterOrOwnerActorByInterface(const UObject* inObject)
{
    return GCUtils::ReinterpretCastChecked<TTargetClass*>(
        GetOuterOrOwnerActorByInterface(inObject, TTargetClass::UClassType::StaticClass()));
}

template <class TTargetClass>
TTargetClass* GCUtils::ObjectTraversal::GetTypedSelfOrOwnerActor(AActor* inActor)
{
    if constexpr (TIsIInterface<TTargetClass>::Value == true)
    {
        return GCUtils::ReinterpretCastChecked<TTargetClass*>(
            GetSelfOrOwnerActorByInterface(inActor, TTargetClass::UClassType::StaticClass()));
    }
    else
    {
        return GCUtils::StaticCastChecked<TTargetClass*>(
            GetSelfOrOwnerActorByClass(inActor, TTargetClass::StaticClass()));
    }
}

template <class TTargetClass>
TTargetClass* GCUtils::ObjectTraversal::GetTypedOwnerActor(const AActor* inActor)
{
    if constexpr (TIsIInterface<TTargetClass>::Value == true)
    {
        return GCUtils::ReinterpretCastChecked<TTargetClass*>(
            GetOwnerActorByInterface(inActor, TTargetClass::UClassType::StaticClass()));
    }
    else
    {
        return GCUtils::StaticCastChecked<TTargetClass*>(
            GetOwnerActorByClass(inActor, TTargetClass::StaticClass()));
    }
}

template <class TTargetClass>
TTargetClass* GCUtils::ObjectTraversal::GetTypedSelfOrOuter(UObject* inObject)
{
    if constexpr (TIsIInterface<TTargetClass>::Value == true)
    {
        return GCUtils::ReinterpretCastChecked<TTargetClass*>(
            GetSelfOrOuterByInterface(inObject, TTargetClass::UClassType::StaticClass()));
    }
    else
    {
        return GCUtils::StaticCastChecked<TTargetClass*>(
            GetSelfOrOuterByClass(inObject, TTargetClass::StaticClass()));
    }
}

template <class TTargetClass>
TTargetClass* GCUtils::ObjectTraversal::GetTypedOuter(const UObject* inObject)
{
    if constexpr (TIsIInterface<TTargetClass>::Value == true)
    {
        return GCUtils::ReinterpretCastChecked<TTargetClass*>(
            GetOuterByInterface(inObject, TTargetClass::UClassType::StaticClass()));
    }
    else
    {
        return GCUtils::StaticCastChecked<TTargetClass*>(
            GetOuterByClass(inObject, TTargetClass::StaticClass()));
    }
}
