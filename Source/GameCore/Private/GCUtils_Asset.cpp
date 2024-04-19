// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_Asset.h"

#include "Engine/AssetManager.h"

bool GCUtils::Asset::SoftIsA(FSoftObjectPath objectPath, const UClass* targetClass)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftIsA);

    if (objectPath.IsValid() == false)
    {
        return false;
    }

    if (!targetClass)
    {
        return false;
    }

    // If the target class is native, skip to native classes and work with those.
    if (targetClass->IsNative())
    {
        const UClass* nativeClass = GetNativeClassForObjectPath(MoveTemp(objectPath));
        if (!nativeClass)
        {
            return false;
        }

        return nativeClass->IsChildOf(targetClass);
    }

    // Use the entirely path-based way.
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

    // If the target class is native, skip to native classes and work with those.
    if (IsNativeClassPath(targetClassPath))
    {
        const UClass* nativeClass = GetNativeClassForObjectPath(MoveTemp(objectPath));
        if (!nativeClass)
        {
            return false;
        }

        const UClass* nativeTargetClass = GetNativeClassForClassPath(targetClassPath);
        return nativeClass->IsChildOf(nativeTargetClass);
    }

    // We are dealing with a non-native target class. Traverse the super class paths until
    // we hit the target class path.
    for (FSoftObjectPath classPath = SoftGetClass(MoveTemp(objectPath));
        classPath.IsValid();
        classPath = SoftGetSuperClass(MoveTemp(classPath)))
    {
        if (classPath == targetClassPath)
        {
            return true;
        }
    }

    // Not of the target class.
    return false;
}

bool GCUtils::Asset::SoftIsChildOf(FSoftObjectPath classPath, const UClass* targetClass)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftIsChildOf);

#if DO_ENSURE
    if (classPath.IsValid())
    {
        ensureAlways(IsClassPath(classPath));
    }
#endif // DO_ENSURE

    if (classPath.IsValid() == false)
    {
        return false;
    }

    if (!targetClass)
    {
        return false;
    }

    // If the target class is native, skip to native classes and work with those.
    if (targetClass->IsNative())
    {
        const UClass* nativeClass = GetNativeClassForClassPath(MoveTemp(classPath));
        if (!nativeClass)
        {
            return false;
        }

        return nativeClass->IsChildOf(targetClass);
    }

    if (IsNativeClassPath(classPath))
    {
        // It's impossible for native to be a child of non-native.
        return false;
    }

    // Use the entirely path-based way.
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

    if (classPath.IsValid() == false)
    {
        return false;
    }

    if (targetClassPath.IsValid() == false)
    {
        return false;
    }

    // If the target class is native, skip to native classes and work with those.
    if (IsNativeClassPath(targetClassPath))
    {
        const UClass* nativeClass = GetNativeClassForClassPath(MoveTemp(classPath));
        if (!nativeClass)
        {
            return false;
        }

        const UClass* nativeTargetClass = GetNativeClassForClassPath(targetClassPath);
        return nativeClass->IsChildOf(nativeTargetClass);
    }

    // We are dealing with a non-native target class. Traverse the super class paths until
    // we hit the target class path.
    for (classPath; classPath.IsValid(); classPath = SoftGetSuperClass(MoveTemp(classPath)))
    {
        if (classPath == targetClassPath)
        {
            return true;
        }
    }

    // Not a child of the target class.
    return false;
}

UClass* GCUtils::Asset::GetNativeClassForObjectPath(const FSoftObjectPath& objectPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::GetNativeClassForObjectPath);

    UClass* nativeClass = nullptr;

    ResolveObjectOrGetAssetData(objectPath,
        [&nativeClass](UObject& resolvedObject)
        {
            // Go up the super classes until we get a native one, setting our native class pointer as we go.
            for (nativeClass = resolvedObject.GetClass();
                nativeClass && nativeClass->IsNative() == false;
                nativeClass = nativeClass->GetSuperClass());
        },
        [&nativeClass](FAssetData assetData)
        {
            FSoftObjectPath nativeClassPath = assetData.GetTagValueRef<FString>(FBlueprintTags::NativeParentClassPath);
            nativeClass = Cast<UClass>(nativeClassPath.ResolveObject());
            ensureAlways(nativeClass);
        });

#if DO_ENSURE
    if (nativeClass)
    {
        ensureAlways(nativeClass->IsNative());
    }
#endif // DO_ENSURE

    return nativeClass;
}

UClass* GCUtils::Asset::GetNativeClassForClassPath(const FSoftObjectPath& classPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::GetNativeClassForClassPath);

#if DO_ENSURE
    if (classPath.IsValid())
    {
        ensureAlways(IsClassPath(classPath));
    }
#endif // DO_ENSURE

    UClass* nativeClass = nullptr;

    ResolveObjectOrGetAssetData(classPath,
        [&nativeClass](UObject& resolvedObject)
        {
            UClass* resolvedClass = Cast<UClass>(&resolvedObject);
            if (!ensureAlwaysMsgf(resolvedClass, TEXT("Resolved object should be a class.")))
            {
                return;
            }

            // Go up the super classes until we get a native one, setting our native class pointer as we go.
            for (nativeClass = resolvedClass;
                nativeClass && nativeClass->IsNative() == false;
                nativeClass = nativeClass->GetSuperClass());
        },
        [&nativeClass](FAssetData assetData)
        {
            FSoftObjectPath nativeClassPath = assetData.GetTagValueRef<FString>(FBlueprintTags::NativeParentClassPath);
            nativeClass = Cast<UClass>(nativeClassPath.ResolveObject());
            ensureAlways(nativeClass);
        });

#if DO_ENSURE
    if (nativeClass)
    {
        ensureAlways(nativeClass->IsNative());
    }
#endif // DO_ENSURE

    return nativeClass;
}

FSoftObjectPath GCUtils::Asset::SoftGetClass(const FSoftObjectPath& objectPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftGetClass);

    FSoftObjectPath classPath;

    ResolveObjectOrGetAssetData(objectPath,
        [&classPath](UObject& resolvedObject)
        {
            classPath = resolvedObject.GetClass();
        },
        [&classPath](FAssetData assetData)
        {
            classPath = assetData.GetTagValueRef<FString>(FBlueprintTags::ParentClassPath);
        });

    return classPath;
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

    FSoftObjectPath superClassPath = nullptr;

    ResolveObjectOrGetAssetData(classPath,
        [&superClassPath](UObject& resolvedObject)
        {
            const UClass* resolvedClass = Cast<UClass>(&resolvedObject);
            if (!ensureAlwaysMsgf(resolvedClass, TEXT("Resolved object should be a class.")))
            {
                return;
            }

            superClassPath = resolvedClass->GetSuperClass();
        },
        [&superClassPath](FAssetData assetData)
        {
            superClassPath = assetData.GetTagValueRef<FString>(FBlueprintTags::ParentClassPath);
        });

    return superClassPath;
}

bool GCUtils::Asset::ResolveObjectOrGetAssetData(const FSoftObjectPath& path,
    FObjectResolvedCallbackFunctionRef onObjectResolvedCallback,
    FAssetDataGottenCallbackFunctionRef onAssetDataGottenCallback)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::ResolveObjectOrGetAssetData);

    if (path.IsValid() == false)
    {
        return false;
    }

    // If the object can be resolved, use it.
    // - E.g., the object is from a native script package.
    // - E.g., the object is a transient object.
    // - E.g., the object is a serialized asset that just happens to be loaded.
    {
        UObject* resolvedObject = nullptr;
        {
            TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::ResolveObjectOrGetAssetData - Resolve Attempt);
            resolvedObject = path.ResolveObject();
        }

        if (resolvedObject)
        {
            onObjectResolvedCallback(*resolvedObject);
            return true;
        }
    }

    FTopLevelAssetPath assetPath = path.GetAssetPath();
    const FName packageName = assetPath.GetPackageName();

    if (packageName == GetTransientPackage()->GetFName())
    {
        // This is a transient package path. So know we there won't be on-disk asset data for it. Must've been
        // a bad path. Return.
        return false;
    }

    TStringBuilder<256> packageNameString;
    packageName.ToString(packageNameString);
    if (FPackageName::IsScriptPackage(packageNameString))
    {
        // This is a script path. So know we there won't be on-disk asset data for it. Must've been
        // a bad path. Return.
        return false;
    }

    // The object must be a serialized asset that's unloaded.

    FAssetData assetData;
    {
        TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::ResolveObjectOrGetAssetData - On Disk Asset Data);

        const IAssetRegistry& assetRegistry = UAssetManager::Get().GetAssetRegistry();

        // Look for asset data on disk.
        constexpr bool shouldIncludeOnlyOnDiskAssets = true; // Skip looking in memory because we've already tried.
        constexpr bool shouldSkipARFilteredAssets = false;
        assetData = assetRegistry.GetAssetByObjectPath(path,
            shouldIncludeOnlyOnDiskAssets,
            shouldSkipARFilteredAssets);

        // TODO @techdebt: Account for redirectors!
    }

    if (assetData.IsValid() == false)
    {
        return false;
    }

    onAssetDataGottenCallback(MoveTemp(assetData));
    return true;
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
