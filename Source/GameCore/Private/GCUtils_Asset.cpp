// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_Asset.h"

#include "Engine/AssetManager.h"

bool GCUtils::Asset::SoftIsA(FSoftObjectPath objectPath, const UClass* targetClass)
{
    return SoftIsA(MoveTemp(objectPath), FSoftObjectPath(targetClass));
}

bool GCUtils::Asset::SoftIsA(FSoftObjectPath objectPath, const FSoftObjectPath& targetClassPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftIsA);

#if DO_ENSURE
    if (targetClassPath.IsValid())
    {
        ensureAlways(IsClassPath(targetClassPath));
    }
#endif // DO_ENSURE

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

bool GCUtils::Asset::SoftIsChildOf(FSoftObjectPath classPath, const UClass* targetClass)
{
    return SoftIsChildOf(MoveTemp(classPath), FSoftObjectPath(targetClass));
}

bool GCUtils::Asset::SoftIsChildOf(FSoftObjectPath classPath, const FSoftObjectPath& targetClassPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftIsChildOf);

#if DO_ENSURE
    if (classPath.IsValid())
    {
        ensureAlways(IsClassPath(classPath));
    }
#endif // DO_ENSURE

#if DO_ENSURE
    if (targetClassPath.IsValid())
    {
        ensureAlways(IsClassPath(targetClassPath));
    }
#endif // DO_ENSURE

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

FSoftObjectPath GCUtils::Asset::SoftGetSuperClass(const FSoftObjectPath& classPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftGetSuperClass);

#if DO_ENSURE
    if (classPath.IsValid())
    {
        ensureAlways(IsClassPath(classPath));
    }
#endif // DO_ENSURE

    if (classPath.IsValid() == false)
    {
        return nullptr;
    }

    // If the class can be resolved, use it.
    // - E.g., the class is a native class.
    // - E.g., the class is a serialized asset (blueprint generated class) that just happens to be loaded.
    {
        TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftGetSuperClass - Resolve Attempt);

        const UObject* resolvedObject = classPath.ResolveObject();
        const UClass* resolvedClass = Cast<UClass>(resolvedObject);

        if (resolvedClass)
        {
            return resolvedClass->GetSuperClass();
        }

        if (IsNativeClassPath(classPath))
        {
            ensureAlwaysMsgf(!resolvedObject, TEXT("If no resolved class, no object should've been resolvable either."));

            // No valid class from this path.
            return nullptr;
        }
    }

    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftGetSuperClass - On Disk Asset Data);

    // The class must be a serialized asset (blueprint generated class) that's unloaded.

    // TODO @techdebt: Account for redirectors!

    const IAssetRegistry& assetRegistry = UAssetManager::Get().GetAssetRegistry();

    // Look for asset data on disk.
    constexpr bool shouldIncludeOnlyOnDiskAssets = true; // Skip looking in memory because we've already tried.
    constexpr bool shouldSkipARFilteredAssets = false;
    FAssetData assetData = assetRegistry.GetAssetByObjectPath(classPath,
        shouldIncludeOnlyOnDiskAssets,
        shouldSkipARFilteredAssets);

    return assetData.GetTagValueRef<FString>(FBlueprintTags::ParentClassPath);
}

FSoftObjectPath GCUtils::Asset::SoftGetClass(const FSoftObjectPath& objectPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftGetClass);

    if (objectPath.IsValid() == false)
    {
        return nullptr;
    }

    // If the object can be resolved, use it.
    // - E.g., the object is a CDO of a native class.
    // - E.g., the object is a transient object.
    // - E.g., the object is a serialized asset that just happens to be loaded.
    {
        TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftGetClass - Resolve Attempt);

        const UObject* resolvedObject = objectPath.ResolveObject();
        if (resolvedObject)
        {
            return resolvedObject->GetClass();
        }
    }

    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftGetClass - On Disk Asset Data);

    // The object must be a serialized asset that's unloaded.

    // TODO @techdebt: Account for redirectors!

    const IAssetRegistry& assetRegistry = UAssetManager::Get().GetAssetRegistry();

    // Look for asset data on disk.
    constexpr bool shouldIncludeOnlyOnDiskAssets = true; // Skip looking in memory because we've already tried.
    constexpr bool shouldSkipARFilteredAssets = false;
    FAssetData assetData = assetRegistry.GetAssetByObjectPath(objectPath,
        shouldIncludeOnlyOnDiskAssets,
        shouldSkipARFilteredAssets);

    return assetData.GetTagValueRef<FString>(FBlueprintTags::ParentClassPath);
}

bool GCUtils::Asset::IsClassPath(const FSoftObjectPath& path)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::IsClassPath);

    return IsBlueprintGeneratedClassPath(path) || IsNativeClassPath(path);
}

bool GCUtils::Asset::IsNativeClassPath(const FSoftObjectPath& path)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::IsNativeClassPath);

    // Note: We make an effort to reliably determine this without using FSoftObjectPath::ResolveObject().

    if (path.IsValid() == false)
    {
        return false;
    }

    if (path.IsSubobject())
    {
        // Classes are never subobjects.
        return false;
    }

    FTopLevelAssetPath assetPath = path.GetAssetPath();
    const FName packageName = assetPath.GetPackageName();
    const FName assetName = assetPath.GetAssetName();

    if (assetName.IsNone())
    {
        // This is just a package.
        return false;
    }

    TStringBuilder<256> packageNameString;
    packageName.ToString(packageNameString);

    if (FPackageName::IsScriptPackage(packageNameString) == false)
    {
        // Not a native object.
        return false;
    }

    TStringBuilder<128> assetNameString;
    assetName.ToString(assetNameString);

    if (IsClassDefaultObjectName(assetNameString))
    {
        // This is a class default object.
        return false;
    }

    return true;
}

bool GCUtils::Asset::IsBlueprintGeneratedClassPath(const FSoftObjectPath& path)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::IsBlueprintGeneratedClassPath);

    if (path.IsValid() == false)
    {
        return false;
    }

    const FName assetName = path.GetAssetPath().GetAssetName();
    TStringBuilder<128> assetNameString;
    assetName.ToString(assetNameString);

    return IsBlueprintGeneratedClassName(assetNameString);
}

bool GCUtils::Asset::IsBlueprintGeneratedClassName(const FStringView& nameString)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::IsBlueprintGeneratedClassName);

    return nameString.EndsWith(BlueprintGeneratedClassPostfix);
}

bool GCUtils::Asset::IsClassDefaultObjectName(const FStringView& nameString)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::IsClassDefaultObjectName);

    return nameString.StartsWith(DEFAULT_OBJECT_PREFIX);
}
