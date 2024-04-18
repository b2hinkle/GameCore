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

    // If the class can be resolved, return its super class.
    // - E.g., the class is a native class.
    // - Also, e.g., the class is an asset (blueprint generated class) that happens to be loaded.
    if (const UClass* resolvedClass = Cast<UClass>(classPath.ResolveObject()))
    {
        return resolvedClass->GetSuperClass();
    }

    // The class must be an asset (blueprint generated class) that's unloaded.

    // TODO @techdebt: Account for redirectors!

    const IAssetRegistry& assetRegistry = UAssetManager::Get().GetAssetRegistry();

    constexpr bool shouldIncludeOnlyOnDiskAssets = false;
    constexpr bool shouldSkipARFilteredAssets = false;
    FAssetData assetData = assetRegistry.GetAssetByObjectPath(classPath,
        shouldIncludeOnlyOnDiskAssets,
        shouldSkipARFilteredAssets);

    return assetData.GetTagValueRef<FString>(FBlueprintTags::ParentClassPath);
}

FSoftObjectPath GCUtils::Asset::SoftGetClass(const FSoftObjectPath& objectPath)
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

    // TODO @techdebt: Account for redirectors!

    const IAssetRegistry& assetRegistry = UAssetManager::Get().GetAssetRegistry();

    constexpr bool shouldIncludeOnlyOnDiskAssets = false;
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
