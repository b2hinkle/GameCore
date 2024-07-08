// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_Plugin.h"

#include "Interfaces/IPluginManager.h"
#include "PluginDescriptor.h"
#include "PluginReferenceDescriptor.h"

void GCUtils::Plugin::UseContentFromDependentPlugins(
    const FStringView& inSelfPluginNameString,
    FPluginRefNativeDelegate inOnPluginAddContentCallback,
    FPluginRefNativeDelegate inOnPluginRemoveContentCallback)
{
    // Look for plugins that have content for us to use.
    for (const TSharedRef<IPlugin>& plugin : IPluginManager::Get().GetEnabledPluginsWithContent())
    {
        if (FPackageName::MountPointExists(plugin->GetMountedAssetPath()) == false)
        {
            // This plugin's content is not currently mounted.
            continue;
        }

        if (DoesPluginUseUs(inSelfPluginNameString, plugin))
        {
            // Use this plugin's content.
            inOnPluginAddContentCallback.ExecuteIfBound(plugin);
        }
    }

    // Listen for dynamically loaded and unloaded plugins, e.g., game features.

    constexpr int32 pluginNameStringBufferLength = 64;

    FPackageName::OnContentPathMounted().AddLambda(
        [onPluginAddContentCallback = MoveTemp(inOnPluginAddContentCallback),
        selfPluginNameCharArray = TArray<TCHAR, TInlineAllocator<pluginNameStringBufferLength>>(
            TArrayView<const TCHAR>(inSelfPluginNameString))]
        (const FString& inAssetPath, const FString& inContentPath)
        {
            TSharedPtr<IPlugin> pluginPtr = IPluginManager::Get().FindPluginFromPath(inAssetPath);
            if (pluginPtr.IsValid() == false)
            {
                // No plugin found.
                return;
            }

            TSharedRef<IPlugin> pluginRef = MoveTemp(pluginPtr).ToSharedRef();

            if (DoesPluginUseUs(selfPluginNameCharArray, pluginRef))
            {
                // Use this plugin's content.
                onPluginAddContentCallback.ExecuteIfBound(MoveTemp(pluginRef));
            }
        }
        );

    FPackageName::OnContentPathDismounted().AddLambda(
        [onPluginRemoveContentCallback = MoveTemp(inOnPluginRemoveContentCallback),
        selfPluginNameCharArray = TArray<TCHAR, TInlineAllocator<pluginNameStringBufferLength>>(
            TArrayView<const TCHAR>(inSelfPluginNameString))]
        (const FString& inAssetPath, const FString& inContentPath)
        {
            TSharedPtr<IPlugin> pluginPtr = IPluginManager::Get().FindPluginFromPath(inAssetPath);
            if (pluginPtr.IsValid() == false)
            {
                // No plugin found.
                return;
            }

            TSharedRef<IPlugin> pluginRef = MoveTemp(pluginPtr).ToSharedRef();

            if (DoesPluginUseUs(selfPluginNameCharArray, pluginRef))
            {
                // Use this plugin's content.
                onPluginRemoveContentCallback.ExecuteIfBound(MoveTemp(pluginRef));
            }
        }
        );
}

bool GCUtils::Plugin::DoesPluginUseUs(
    const FStringView& inSelfPluginNameString,
    const TSharedRef<const IPlugin>& inPlugin)
{
    if (inPlugin->GetName() == inSelfPluginNameString)
    {
        // We are the plugin.
        return true;
    }

    if (inPlugin->GetDescriptor().Plugins.ContainsByPredicate(
            [&inSelfPluginNameString](const FPluginReferenceDescriptor& inPluginReferenceDescriptor)
            {
                if (inPluginReferenceDescriptor.bEnabled == false)
                {
                    // This plugin is disabled. Ignore it.
                    return false;
                }

                // Return whether this plugin is us.
                return inPluginReferenceDescriptor.Name == inSelfPluginNameString;
            }
            )
        )
    {
        // The plugin depends on us.
        return true;
    }

    return false;
}
