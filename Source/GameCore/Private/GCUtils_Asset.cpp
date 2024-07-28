// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_Asset.h"

#include "Engine/AssetManager.h"
#include "GCUtils_Log.h"

DEFINE_LOG_CATEGORY(LogGCUtils_Asset);

/**
 * @brief Private utilities.
 */
namespace GCUtils::Asset
{
    static FSoftObjectPath GetClassPathForObjectPathUnloaded(
        const FSoftObjectPath& objectPath);

    static FSoftObjectPath GetNextClassPathTowardsTargetParentUnloaded(
        const FSoftObjectPath& currentClassPath,
        const FSoftObjectPath& targetClassPath);
}

bool GCUtils::Asset::SoftIsA(const FSoftObjectPath& objectPath, const UClass* targetClass)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftIsA(const FSoftObjectPath&, const UClass*));

    if (objectPath.IsValid() == false)
    {
        return false;
    }

    if (!targetClass)
    {
        return false;
    }

    // If the object is currently in memory, take advantage of that.
    if (const UObject* resolvedObject = objectPath.ResolveObject())
    {
        return resolvedObject->IsA(targetClass);
    }

    // Rely on on-disk asset data.
    return SoftIsChildOf(
        GetClassPathForObjectPathUnloaded(objectPath),
        targetClass);
}

bool GCUtils::Asset::SoftIsA(const UObject* objectPtr, const FSoftObjectPath& targetClassPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftIsA(const UObject*, const FSoftObjectPath&));

#if DO_ENSURE
    if (targetClassPath.IsValid())
    {
        ensureAlways(IsClassPath(targetClassPath));
    }
#endif // DO_ENSURE

    if (!objectPtr)
    {
        return false;
    }

    if (targetClassPath.IsValid() == false)
    {
        return false;
    }

    return SoftIsChildOf(objectPtr->GetClass(), targetClassPath);
}

bool GCUtils::Asset::SoftIsA(const FSoftObjectPath& objectPath, const FSoftObjectPath& targetClassPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftIsA(const FSoftObjectPath&, const FSoftObjectPath&));

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

    // If the object is currently in memory, take advantage of that.
    if (const UObject* resolvedObject = objectPath.ResolveObject())
    {
        return SoftIsChildOf(resolvedObject->GetClass(), targetClassPath);
    }

    // Rely on on-disk asset data.
    return SoftIsChildOf(
        GetClassPathForObjectPathUnloaded(objectPath),
        targetClassPath);
}

bool GCUtils::Asset::SoftIsChildOf(FSoftObjectPath classPath, const UClass* targetClass)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftIsChildOf(FSoftObjectPath, const UClass*));

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

    if (targetClass->IsNative() == false && IsNativeClassPath(classPath))
    {
        // No way it can be one of our parent classes. It's impossible for a native class to
        // be a child of a non-native.
        return false;
    }

    const FSoftObjectPath targetClassPath = FSoftObjectPath(targetClass);

    for (FSoftObjectPath currentClassPath = MoveTemp(classPath);
        currentClassPath.IsValid();
        currentClassPath = GetNextClassPathTowardsTargetParentUnloaded(
            MoveTemp(currentClassPath),
            targetClassPath))
    {
        // If the class is currently in memory, take advantage of that.
        if (const UClass* resolvedClass = ResolveClass(currentClassPath))
        {
            return resolvedClass->IsChildOf(targetClass);
        }

        if (currentClassPath == targetClassPath)
        {
            return true;
        }
    }

    // Not a child of the target class.
    return false;
}

bool GCUtils::Asset::SoftIsChildOf(const UClass* classPtr, const FSoftObjectPath& targetClassPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftIsChildOf(const UClass*, const FSoftObjectPath&));

#if DO_ENSURE
    if (targetClassPath.IsValid())
    {
        ensureAlways(IsClassPath(targetClassPath));
    }
#endif // DO_ENSURE

    if (!classPtr)
    {
        return false;
    }

    if (targetClassPath.IsValid() == false)
    {
        return false;
    }

    const UClass* resolvedTargetClass = ResolveClass(targetClassPath);
    if (!resolvedTargetClass)
    {
        // No way it can be one of our parent classes. The parent class would've
        // been in memory if the child was.
        return false;
    }

    return classPtr->IsChildOf(resolvedTargetClass);
}

bool GCUtils::Asset::SoftIsChildOf(FSoftObjectPath classPath, const FSoftObjectPath& targetClassPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::SoftIsChildOf(FSoftObjectPath, const FSoftObjectPath&));

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

    if (!IsNativeClassPath(targetClassPath) && IsNativeClassPath(classPath))
    {
        // No way it can be one of our parent classes. It's impossible for a native class to
        // be a child of a non-native.
        return false;
    }

    for (FSoftObjectPath currentClassPath = MoveTemp(classPath);
        currentClassPath.IsValid();
        currentClassPath = GetNextClassPathTowardsTargetParentUnloaded(
            MoveTemp(currentClassPath),
            targetClassPath))
    {
        // If the class is currently in memory, take advantage of that.
        if (const UClass* resolvedClass = ResolveClass(currentClassPath))
        {
            return SoftIsChildOf(resolvedClass, targetClassPath);
        }

        if (currentClassPath == targetClassPath)
        {
            return true;
        }
    }

    // Not a child of the target class.
    return false;
}

FAssetData GCUtils::Asset::GetAssetByObjectPathUnloaded(const FSoftObjectPath& path)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::GetAssetByObjectPathUnloaded);

#if !NO_LOGGING || DO_ENSURE
    if (!ensureAlways(path.ResolveObject() == nullptr))
    {
        GC_LOG_STR_NO_CONTEXT(
            LogGCUtils_Asset,
            Warning,
            GCUtils::Materialize(TStringBuilder<512>())
                << TEXT("The path given is expected to be unresolvable but that was not the case. Path: '")
                << path
                << TEXT("'. Please try resolving the object before resorting to this function.")
            );
    }
#endif // !NO_LOGGING || DO_ENSURE

    if (path.IsValid() == false)
    {
        return FAssetData();
    }

    const FTopLevelAssetPath& assetPath = path.GetAssetPath();
    const FName& packageName = assetPath.GetPackageName();

    check(GetTransientPackage());
    if (packageName == GetTransientPackage()->GetFName())
    {
        // This is a transient package path. So we know there won't be on-disk asset data for it. Must've been
        // a bad path. Return.
        return FAssetData();
    }

    FStringBuilderBase&& packageNameString = WriteToString<256>(packageName);

    if (FPackageName::IsScriptPackage(packageNameString))
    {
        // This is a script path. So we know there won't be on-disk asset data for it. Must've been
        // a bad path. Return.
        return FAssetData();
    }

    // The object must be a serialized asset that's unloaded.

    const IAssetRegistry& assetRegistry = UAssetManager::Get().GetAssetRegistry();

    // TODO @techdebt: Account for redirectors!

    // Look for asset data on disk.
    constexpr bool shouldIncludeOnlyOnDiskAssets = true; // Skip looking in memory.
    constexpr bool shouldSkipARFilteredAssets = false;
    return assetRegistry.GetAssetByObjectPath(path,
        shouldIncludeOnlyOnDiskAssets,
        shouldSkipARFilteredAssets);
}

UClass* GCUtils::Asset::ResolveClass(const FSoftObjectPath& classPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::ResolveClass);

#if !NO_LOGGING || DO_ENSURE
    if (classPath.IsValid())
    {
        if (!ensureAlways(IsClassPath(classPath)))
        {
            GC_LOG_STR_NO_CONTEXT(
                LogGCUtils_Asset,
                Error,
                GCUtils::Materialize(TStringBuilder<512>())
                    << TEXT("The path given is not a class path. `classPath`: '")
                    << classPath
                    << TEXT("'. Expect side effects.")
                );
        }
    }
#endif // !NO_LOGGING || DO_ENSURE

    UObject* resolvedObject = classPath.ResolveObject();
    if (!resolvedObject)
    {
        return nullptr;
    }

    UClass* resolvedClass = Cast<UClass>(resolvedObject);

#if !NO_LOGGING || DO_ENSURE
    if (!ensureAlways(resolvedClass))
    {
        GC_LOG_STR_NO_CONTEXT(
            LogGCUtils_Asset,
            Error,
            GCUtils::Materialize(TStringBuilder<512>())
                << TEXT("Resolved object is supposed to be a class. NULL will be returned. `classPath`: '")
                << classPath
                << TEXT("'.")
            );
    }
#endif // !NO_LOGGING || DO_ENSURE

    return resolvedClass;
}

bool GCUtils::Asset::IsClassPath(const FSoftObjectPath& path)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::IsClassPath);

    return IsNativeClassPath(path) || IsBlueprintGeneratedClassPath(path);
}

bool GCUtils::Asset::IsNativeClassPath(const FSoftObjectPath& path)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::IsNativeClassPath);

    // Goal: We make an effort to reliably determine this without using `FSoftObjectPath::ResolveObject()`.

    if (path.IsValid() == false)
    {
        return false;
    }

    if (path.IsSubobject())
    {
        // Classes are never subobjects.
        return false;
    }

    const FTopLevelAssetPath& assetPath = path.GetAssetPath();
    const FName& packageName = assetPath.GetPackageName();
    const FName& assetName = assetPath.GetAssetName();

    if (assetName.IsNone())
    {
        // This is just a package.
        return false;
    }

    if (IsClassDefaultObjectName(assetName))
    {
        // This is a class default object, not a class.
        return false;
    }

    FStringBuilderBase&& packageNameString = WriteToString<256>(packageName);

    if (FPackageName::IsScriptPackage(packageNameString) == false)
    {
        // Not a native.
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

    if (path.IsSubobject())
    {
        // Classes are never subobjects.
        return false;
    }

    const FTopLevelAssetPath& assetPath = path.GetAssetPath();
    const FName& packageName = assetPath.GetPackageName();
    const FName& assetName = assetPath.GetAssetName();

    if (assetName.IsNone())
    {
        // This is just a package.
        return false;
    }

    FStringBuilderBase&& packageNameString = WriteToString<256>(packageName);

    if (FPackageName::IsScriptPackage(packageNameString))
    {
        // This is a native.
        return false;
    }

    return IsBlueprintGeneratedClassName(assetName);
}

bool GCUtils::Asset::IsBlueprintGeneratedClassName(const FName& name)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::IsBlueprintGeneratedClassName);

    if (IsClassDefaultObjectName(name))
    {
        // This is a class default object, not a class.
        return false;
    }

    FStringBuilderBase&& nameString = WriteToString<128>(name);
    return FStringView(nameString).EndsWith(BlueprintGeneratedClassPostfixString);
}

bool GCUtils::Asset::IsClassDefaultObjectName(const FName& name)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::IsClassDefaultObjectName);

    FStringBuilderBase&& nameString = WriteToString<128>(name);
    return FStringView(nameString).StartsWith(ClassDefaultObjectPrefixString);
}

FSoftObjectPath GCUtils::Asset::GetClassPathForObjectPathUnloaded(const FSoftObjectPath& objectPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::GetClassPathForObjectPathUnloaded);

    ensureAlways(objectPath.IsValid());

    // Use on-disk asset data.
    FAssetData assetData = GetAssetByObjectPathUnloaded(objectPath);
    if (assetData.IsValid() == false)
    {
        // No on-disk asset data found. The path is likely a bad path overall that
        // could neither be resolved in memory nor found on disk.
        return nullptr;
    }

    // TODO @techdebt: Account for redirectors!

    return FSoftObjectPath(MoveTemp(assetData.AssetClassPath));
}

FSoftObjectPath GCUtils::Asset::GetNextClassPathTowardsTargetParentUnloaded(
    const FSoftObjectPath& currentClassPath,
    const FSoftObjectPath& targetClassPath)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(GCUtils::Asset::GetNextClassPathTowardsTargetParentUnloaded);

    ensureAlways(currentClassPath.IsValid());
    ensureAlways(targetClassPath.IsValid());
    ensureAlways(IsClassPath(currentClassPath));
    ensureAlways(IsClassPath(targetClassPath));

    // Use on-disk asset data.
    FAssetData assetData = GetAssetByObjectPathUnloaded(currentClassPath);
    if (!assetData.IsValid())
    {
        // No on-disk asset data found. The path is likely a bad path overall that
        // could neither be resolved in memory nor found on disk.
        return nullptr;
    }

    // TODO @techdebt: Account for redirectors!

    FName assetTagNameForNextClass = FBlueprintTags::ParentClassPath;

    // Skip to native classes if possible.
    if (IsNativeClassPath(targetClassPath))
    {
        assetTagNameForNextClass = FBlueprintTags::NativeParentClassPath;
    }

    const FName& nextClassPathName = assetData.GetTagValueRef<FName>(assetTagNameForNextClass);
    FStringBuilderBase&& nextClassPathString = WriteToString<256>(nextClassPathName);

    return FSoftObjectPath(MoveTemp(nextClassPathString));
}
