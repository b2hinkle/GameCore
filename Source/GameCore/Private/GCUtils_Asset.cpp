// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_Asset.h"

#include "Engine/AssetManager.h"

bool GCUtils::Asset::SoftIsA(FSoftObjectPath objectPath, const UClass* targetClass)
{
    return SoftIsA(MoveTemp(objectPath), FSoftClassPath(targetClass));
}

bool GCUtils::Asset::SoftIsA(FSoftObjectPath objectPath, const FSoftClassPath& targetClassPath)
{
    if (objectPath.IsValid() == false)
    {
        return false;
    }

    if (targetClassPath.IsValid() == false)
    {
        return false;
    }

    return SoftIsChildOf(SoftGetClass(objectPath), targetClassPath);
}

bool GCUtils::Asset::SoftIsChildOf(FSoftClassPath classPath, const UClass* targetClass)
{
    return SoftIsChildOf(MoveTemp(classPath), FSoftClassPath(targetClass));
}

bool GCUtils::Asset::SoftIsChildOf(FSoftClassPath classPath, const FSoftClassPath& targetClassPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftIsChildOf);

    if (targetClassPath.IsValid() == false)
    {
        return false;
    }

    for (classPath; classPath.IsValid(); classPath = SoftGetSuperClass(MoveTemp(classPath)))
    {
        if (classPath == targetClassPath)
        {
            return true;
        }
    }

    return false;
}

FSoftClassPath GCUtils::Asset::SoftGetSuperClass(const FSoftClassPath& classPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftGetSuperClass);

    // If the class can be resolved, return its super class.
    // - E.g., the class is a native class.
    // - Also, e.g., the class is an asset (blueprint generated class) that happens to be loaded.
    if (const UClass* resolvedClass = classPath.ResolveClass())
    {
        return resolvedClass->GetSuperClass();
    }

    // The class must be an asset (blueprint generated class) that's unloaded.

    const IAssetRegistry& assetRegistry = UAssetManager::Get().GetAssetRegistry();

    constexpr bool shouldIncludeOnlyOnDiskAssets = false;
    constexpr bool shouldSkipARFilteredAssets = false;
    FAssetData assetData = assetRegistry.GetAssetByObjectPath(classPath,
        shouldIncludeOnlyOnDiskAssets,
        shouldSkipARFilteredAssets);

    return assetData.GetTagValueRef<FString>(FBlueprintTags::ParentClassPath);
}

FSoftClassPath GCUtils::Asset::SoftGetClass(const FSoftObjectPath& objectPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftGetClass);

    // If the object can be resolved, return its class.
    // - E.g., the object is a CDO of a native class.
    // - E.g., the object is a transient object that's loaded.
    // - Also, e.g., the object is an asset that happens to be loaded.
    if (const UObject* resolvedObject = objectPath.ResolveObject())
    {
        return resolvedObject->GetClass();
    }

    // The object must be an asset that's unloaded.

    const IAssetRegistry& assetRegistry = UAssetManager::Get().GetAssetRegistry();

    constexpr bool shouldIncludeOnlyOnDiskAssets = false;
    constexpr bool shouldSkipARFilteredAssets = false;
    FAssetData assetData = assetRegistry.GetAssetByObjectPath(objectPath,
        shouldIncludeOnlyOnDiskAssets,
        shouldSkipARFilteredAssets);

    return assetData.GetTagValueRef<FString>(FBlueprintTags::ParentClassPath);
}

FSoftObjectPath GCUtils::Asset::MakeSoftObjectPathForSoftClassPath(FSoftClassPath classPath)
{
    return FSoftObjectPath(MoveTemp(classPath));
}

FSoftClassPath GCUtils::Asset::MakeSoftClassPathForSoftObjectPath(const FSoftObjectPath& objectPath)
{
    return FSoftClassPath(objectPath.ToString());
}
