// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCConcepts.h"
#include "GCUtils.h"

namespace GCUtils::ObjectTraversal
{
    // Begin traversal functions.

    /**
     * Traverses the UObject outer chain then the AActor owner chain. Including self.
     *
     * Breaks when shouldBreakPredicate returns true.
     *
     * Returns object/actor that was broken on or nullptr.
     */
    GAMECORE_API const UObject* TraverseSelfToOutersToOwnerActorsBreakable(const UObject* inObject, const TFunctionRef<bool(const UObject&)>& inShouldBreakPredicate);
    GAMECORE_API UObject* TraverseSelfToOutersToOwnerActorsBreakable(UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate);

    /**
     * Same as TraverseSelfToOutersToOwnerActorsBreakable() but doesn't include self.
     */
    GAMECORE_API UObject* TraverseOutersToOwnerActorsBreakable(const UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate);

    /**
     * Traverses the AActor owner chain. Including self.
     *
     * Breaks when shouldBreakPredicate returns true.
     *
     * Returns actor that was broken on or nullptr.
     */
    GAMECORE_API const AActor* TraverseSelfToOwnerActorsBreakable(const AActor* inActor, const TFunctionRef<bool(const AActor&)>& inShouldBreakPredicate);
    GAMECORE_API AActor* TraverseSelfToOwnerActorsBreakable(AActor* inActor, const TFunctionRef<bool(AActor&)>& inShouldBreakPredicate);

    /**
     * Same as TraverseSelfToOwnerActorsBreakable() but doesn't include self.
     */
    GAMECORE_API AActor* TraverseOwnerActorsBreakable(const AActor* inActor, const TFunctionRef<bool(AActor&)>& inShouldBreakPredicate);

    /**
     * Traverses the UObject outer chain. Including self.
     *
     * Breaks when shouldBreakPredicate returns true.
     *
     * Returns object that was broken on or nullptr.
     */
    GAMECORE_API const UObject* TraverseSelfToOutersBreakable(const UObject* inObject, const TFunctionRef<bool(const UObject&)>& inShouldBreakPredicate);
    GAMECORE_API UObject* TraverseSelfToOutersBreakable(UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate);

    /**
     * Same as TraverseSelfToOutersBreakable() but doesn't include self.
     */
    GAMECORE_API UObject* TraverseOutersBreakable(const UObject* inObject, const TFunctionRef<bool(UObject&)>& inShouldBreakPredicate);

    // End traversal functions.

    // Begin "outer or owner actor" functions.

    /**
     * Traverse the UObject outer chain then the AActor owner chain to return the
     * first thing that implements the interface. Including self.
     */
    template <GCConcepts::IInterface TTargetClass>
    const TTargetClass* GetTypedSelfOrOuterOrOwnerActorByInterface(const UObject* inObject);
    template <GCConcepts::IInterface TTargetClass>
    TTargetClass* GetTypedSelfOrOuterOrOwnerActorByInterface(UObject* inObject);

    GAMECORE_API const UObject* GetSelfOrOuterOrOwnerActorByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);
    GAMECORE_API UObject* GetSelfOrOuterOrOwnerActorByInterface(UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    // End "outer or owner actor" functions.

    // Begin "outer or owner actor" functions.

    /**
     * Same as GetSelfOrOuterOrOwnerActorByInterface() but doesn't include self.
     */
    template <GCConcepts::IInterface TTargetClass>
    TTargetClass* GetTypedOuterOrOwnerActorByInterface(const UObject* inObject);

    GAMECORE_API UObject* GetOuterOrOwnerActorByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    // End "outer or owner actor" functions.

    // Begin "self or owner actor" functions.

    template <GCConcepts::UObjectDerivedOrIInterface TTargetClass>
    const TTargetClass* GetTypedSelfOrOwnerActor(const AActor* inActor);
    template <GCConcepts::UObjectDerivedOrIInterface TTargetClass>
    TTargetClass* GetTypedSelfOrOwnerActor(AActor* inActor);

    GAMECORE_API const AActor* GetSelfOrOwnerActorByClass(const AActor* inActor, const TSubclassOf<AActor>& inTargetClass);
    GAMECORE_API AActor* GetSelfOrOwnerActorByClass(AActor* inActor, const TSubclassOf<AActor>& inTargetClass);

    GAMECORE_API const AActor* GetSelfOrOwnerActorByInterface(const AActor* inActor, const TSubclassOf<UInterface>& inTargetClass);
    GAMECORE_API AActor* GetSelfOrOwnerActorByInterface(AActor* inActor, const TSubclassOf<UInterface>& inTargetClass);

    // End "self or owner actor" functions.

    // Begin "owner actor" functions.

    template <GCConcepts::UObjectDerivedOrIInterface TTargetClass>
    TTargetClass* GetTypedOwnerActor(const AActor* inActor);

    GAMECORE_API AActor* GetOwnerActorByClass(const AActor* inActor, const TSubclassOf<AActor>& inTargetClass);

    GAMECORE_API AActor* GetOwnerActorByInterface(const AActor* inActor, const TSubclassOf<UInterface>& inTargetClass);

    // End "owner actor" functions.

    // Begin "self or outer" functions.

    template <GCConcepts::UObjectDerivedOrIInterface TTargetClass>
    const TTargetClass* GetTypedSelfOrOuter(const UObject* inObject);
    template <GCConcepts::UObjectDerivedOrIInterface TTargetClass>
    TTargetClass* GetTypedSelfOrOuter(UObject* inObject);

    GAMECORE_API const UObject* GetSelfOrOuterByClass(const UObject* inObject, const TSubclassOf<UObject>& inTargetClass);
    GAMECORE_API UObject* GetSelfOrOuterByClass(UObject* inObject, const TSubclassOf<UObject>& inTargetClass);

    GAMECORE_API const UObject* GetSelfOrOuterByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);
    GAMECORE_API UObject* GetSelfOrOuterByInterface(UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    // End "self or outer" functions.

    // Begin "outer" functions.

    template <GCConcepts::UObjectDerivedOrIInterface TTargetClass>
    TTargetClass* GetTypedOuter(const UObject* inObject);

    GAMECORE_API UObject* GetOuterByClass(const UObject* inObject, const TSubclassOf<UObject>& inTargetClass);

    GAMECORE_API UObject* GetOuterByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    // End "outer" functions.
}

template <GCConcepts::IInterface TTargetClass>
const TTargetClass* GCUtils::ObjectTraversal::GetTypedSelfOrOuterOrOwnerActorByInterface(const UObject* inObject)
{
    return GCUtils::CastUObjectChecked<const TTargetClass*>(
        GetSelfOrOuterOrOwnerActorByInterface(inObject, TTargetClass::UClassType::StaticClass()));
}
template <GCConcepts::IInterface TTargetClass>
TTargetClass* GCUtils::ObjectTraversal::GetTypedSelfOrOuterOrOwnerActorByInterface(UObject* inObject)
{
    return const_cast<TTargetClass*>(GetTypedSelfOrOuterOrOwnerActorByInterface<TTargetClass>(const_cast<const UObject*>(inObject)));
}

template <GCConcepts::IInterface TTargetClass>
TTargetClass* GCUtils::ObjectTraversal::GetTypedOuterOrOwnerActorByInterface(const UObject* inObject)
{
    return GCUtils::CastUObjectChecked<TTargetClass*>(
        GetOuterOrOwnerActorByInterface(inObject, TTargetClass::UClassType::StaticClass()));
}

template <GCConcepts::UObjectDerivedOrIInterface TTargetClass>
const TTargetClass* GCUtils::ObjectTraversal::GetTypedSelfOrOwnerActor(const AActor* inActor)
{
    if constexpr (TIsIInterface<TTargetClass>::Value == true)
    {
        return GCUtils::CastUObjectChecked<const TTargetClass*>(
            GetSelfOrOwnerActorByInterface(inActor, TTargetClass::UClassType::StaticClass()));
    }
    else
    {
        return GCUtils::CastUObjectChecked<const TTargetClass*>(
            GetSelfOrOwnerActorByClass(inActor, TTargetClass::StaticClass()));
    }
}
template <GCConcepts::UObjectDerivedOrIInterface TTargetClass>
TTargetClass* GCUtils::ObjectTraversal::GetTypedSelfOrOwnerActor(AActor* inActor)
{
    return const_cast<TTargetClass*>(GetTypedSelfOrOwnerActor<TTargetClass>(const_cast<const AActor*>(inActor)));
}

template <GCConcepts::UObjectDerivedOrIInterface TTargetClass>
TTargetClass* GCUtils::ObjectTraversal::GetTypedOwnerActor(const AActor* inActor)
{
    if constexpr (TIsIInterface<TTargetClass>::Value == true)
    {
        return GCUtils::CastUObjectChecked<TTargetClass*>(
            GetOwnerActorByInterface(inActor, TTargetClass::UClassType::StaticClass()));
    }
    else
    {
        return GCUtils::CastUObjectChecked<TTargetClass*>(
            GetOwnerActorByClass(inActor, TTargetClass::StaticClass()));
    }
}

template <GCConcepts::UObjectDerivedOrIInterface TTargetClass>
const TTargetClass* GCUtils::ObjectTraversal::GetTypedSelfOrOuter(const UObject* inObject)
{
    if constexpr (TIsIInterface<TTargetClass>::Value == true)
    {
        return GCUtils::CastUObjectChecked<const TTargetClass*>(
            GetSelfOrOuterByInterface(inObject, TTargetClass::UClassType::StaticClass()));
    }
    else
    {
        return GCUtils::CastUObjectChecked<const TTargetClass*>(
            GetSelfOrOuterByClass(inObject, TTargetClass::StaticClass()));
    }
}
template <GCConcepts::UObjectDerivedOrIInterface TTargetClass>
TTargetClass* GCUtils::ObjectTraversal::GetTypedSelfOrOuter(UObject* inObject)
{
    return const_cast<TTargetClass*>(GetTypedSelfOrOuter<TTargetClass>(const_cast<const UObject*>(inObject)));
}

template <GCConcepts::UObjectDerivedOrIInterface TTargetClass>
TTargetClass* GCUtils::ObjectTraversal::GetTypedOuter(const UObject* inObject)
{
    if constexpr (TIsIInterface<TTargetClass>::Value == true)
    {
        return GCUtils::CastUObjectChecked<TTargetClass*>(
            GetOuterByInterface(inObject, TTargetClass::UClassType::StaticClass()));
    }
    else
    {
        return GCUtils::CastUObjectChecked<TTargetClass*>(
            GetOuterByClass(inObject, TTargetClass::StaticClass()));
    }
}
