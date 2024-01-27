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

    // Begin "GetTyped" Utils.

    /**
     * Traverse the UObject outer chain then the AActor owner chain to return the
     * first thing that implements the interface.
     */
    GAMECORE_API UObject* GetInterfaceTypedOuterOrOwnerActor(const UObject* inObject, TSubclassOf<UInterface> inTargetClass);

    /**
     * Traverse the UObject outer chain to return the first AActor that we find.
     *
     * Result of using this on an actor component is consistent with UActorComponent::GetOwner().
     */
    GAMECORE_API AActor* GetOuterActor(const UObject* inObject);

    // Begin "GetTyped" OWNER For OBJECT Utils.
    GAMECORE_API AActor* GetTypedOwnerActorForObject(const UObject* inObject, TSubclassOf<AActor> inTargetClass);

    GAMECORE_API AActor* GetInterfaceTypedOwnerActorForObject(const UObject* inObject, TSubclassOf<UInterface> inTargetClass);
    // End "GetTyped" OWNER For OBJECT Utils.

    // Begin "GetTyped" OUTER Utils.
    GAMECORE_API UObject* GetTypedOuter(const UObject* inObject, UClass* inTargetClass);

    GAMECORE_API UObject* GetTypedOuterIncludingSelf(UObject* inObject, UClass* inTargetClass);

    GAMECORE_API UObject* GetInterfaceTypedOuter(const UObject* inObject, TSubclassOf<UInterface> inTargetClass);

    GAMECORE_API UObject* GetInterfaceTypedOuterIncludingSelf(UObject* inObject, TSubclassOf<UInterface> inTargetClass);
    // End "GetTyped" OUTER Utils.

    // Begin "GetTyped" OWNER Utils.
    GAMECORE_API AActor* GetTypedOwnerActor(const AActor* inActor, TSubclassOf<AActor> inTargetClass);

    GAMECORE_API AActor* GetTypedOwnerActorIncludingSelf(AActor* inActor, TSubclassOf<AActor> inTargetClass);

    GAMECORE_API AActor* GetInterfaceTypedOwnerActor(const AActor* inActor, TSubclassOf<UInterface> inTargetClass);

    GAMECORE_API AActor* GetInterfaceTypedOwnerActorIncludingSelf(AActor* inActor, TSubclassOf<UInterface> inTargetClass);
    // End "GetTyped" OWNER Utils.

    // End "GetTyped" Utils.
}
