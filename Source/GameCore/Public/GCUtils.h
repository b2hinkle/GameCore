// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define GC_CSTRINGIZE(text) TEXT(PREPROCESSOR_TO_STRING(text))

/**
 * If you can't get ahold of a UObject, you may need to use this.
 */
#define GC_LOG_NO_CONTEXT(categoryName, verbosity, format, ...) UE_LOG(categoryName, verbosity, TEXT("[%s] ") format, ANSI_TO_TCHAR(__func__), ##__VA_ARGS__)

#define GC_CLOG_NO_CONTEXT(condition, categoryName, verbosity, format, ...) UE_CLOG(condition, categoryName, verbosity, TEXT("[%s] ") format, ANSI_TO_TCHAR(__func__), ##__VA_ARGS__)

/**
 * @param worldContextObject - UObject to get the world from so that we can output its net mode. May be a UWorld.
 */
#define GC_LOG(worldContextObject, categoryName, verbosity, format, ...) GC_LOG_NO_CONTEXT(categoryName, verbosity, TEXT("[%s] ") format, GCUtils::GetWorldNetModeCString(worldContextObject), ##__VA_ARGS__)

#define GC_CLOG(worldContextObject, condition, categoryName, verbosity, format, ...) GC_CLOG_NO_CONTEXT(condition, categoryName, verbosity, TEXT("[%s] ") format, GCUtils::GetWorldNetModeCString(worldContextObject), ##__VA_ARGS__)

/**
 * Returns the string representation of the specified ENetMode value.
 *
 * @param inNetMode The string representation of ENetMode.
 * @return A CString representation of the net mode.
 */
GAMECORE_API const TCHAR* LexToString(ENetMode inNetMode);

/**
 * Returns the string representation of the specified ENetRole value.
 *
 * @param inNetRole The string representation of ENetRole.
 * @return A CString representation of the net role.
 */
GAMECORE_API const TCHAR* LexToString(ENetRole inNetRole);

namespace GCUtils
{
    inline constexpr const TCHAR* CStringNull = TEXT("NULL");

    GAMECORE_API FString GetUObjectNameSafe(const UObject* inUObject);

    GAMECORE_API FString GetUObjectFullNameSafe(const UObject* inUObject, const UObject* StopOuter = NULL, EObjectFullNameFlags Flags = EObjectFullNameFlags::None);

    GAMECORE_API const TCHAR* BoolToCString(const bool inBool);

    GAMECORE_API const TCHAR* GetWorldNetModeCString(const UObject* inWorldContextObject);

    GAMECORE_API UWorld* GetWorldSafe(const UObject* inObject);

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
    GAMECORE_API UObject* GetTypedSelfOrOuterOrOwnerActorByInterface(UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    /**
     * Same as GetTypedSelfOrOuterOrOwnerActorByInterface() but doesn't include self.
     */
    GAMECORE_API UObject* GetTypedOuterOrOwnerActorByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    /**
     * Traverse the UObject outer chain to return the first AActor that we find.
     *
     * Same result as UActorComponent::GetOwner() when used on an actor component.
     */
    GAMECORE_API AActor* GetOwnerActorForObject(const UObject* inObject);

    GAMECORE_API AActor* GetTypedOwnerActorForObject(const UObject* inObject, const TSubclassOf<AActor>& inTargetClass);
    GAMECORE_API AActor* GetTypedOwnerActorForObjectByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    GAMECORE_API AActor* GetTypedSelfOrOwnerActor(AActor* inActor, const TSubclassOf<AActor>& inTargetClass);
    GAMECORE_API AActor* GetTypedSelfOrOwnerActorByInterface(AActor* inActor, const TSubclassOf<UInterface>& inTargetClass);

    GAMECORE_API AActor* GetTypedOwnerActor(const AActor* inActor, const TSubclassOf<AActor>& inTargetClass);
    GAMECORE_API AActor* GetTypedOwnerActorByInterface(const AActor* inActor, const TSubclassOf<UInterface>& inTargetClass);

    GAMECORE_API UObject* GetTypedSelfOrOuter(UObject* inObject, const TSubclassOf<UObject>& inTargetClass);
    GAMECORE_API UObject* GetTypedSelfOrOuterByInterface(UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);

    GAMECORE_API UObject* GetTypedOuter(const UObject* inObject, const TSubclassOf<UObject>& inTargetClass);
    GAMECORE_API UObject* GetTypedOuterByInterface(const UObject* inObject, const TSubclassOf<UInterface>& inTargetClass);
}
