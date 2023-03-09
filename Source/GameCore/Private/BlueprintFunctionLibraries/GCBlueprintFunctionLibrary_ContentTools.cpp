// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibraries/GCBlueprintFunctionLibrary_ContentTools.h"

#include "Interfaces/IPluginManager.h"
#include "PluginDescriptor.h"



void UGCBlueprintFunctionLibrary_ContentTools::UseContentFromDependentPlugins(const FString& InSelfPluginName, const TDelegate<void(const IPlugin&)>& InOnPluginAddContent, const TDelegate<void(const IPlugin&)>& InOnPluginRemoveContent)
{
	// Look for plugins that have content for us to use
	for (const TSharedRef<IPlugin>& Plugin : IPluginManager::Get().GetEnabledPluginsWithContent())
	{
		if (FPackageName::MountPointExists(Plugin->GetMountedAssetPath())) // if the plugin's content is mounted
		{
			if (PluginUsesUs(InSelfPluginName, Plugin.Get()))
			{
				// Use this plugin's content
				InOnPluginAddContent.Execute(Plugin.Get());
			}
		}
	}

	// Listen for dynamically loaded and unloaded plugins, e.g., Game Features
	FPackageName::OnContentPathMounted().AddLambda(
		[InOnPluginAddContent, InSelfPluginName](const FString& InAssetPath, const FString& InContentPath)
		{
			if (const IPlugin* Plugin = IPluginManager::Get().FindPluginFromPath(InAssetPath).Get())
			{
				if (PluginUsesUs(InSelfPluginName, *Plugin))
				{
					// Use this plugin's content
					InOnPluginAddContent.ExecuteIfBound(*Plugin);
				}
			}
		}
	);

	FPackageName::OnContentPathDismounted().AddLambda(
		[InOnPluginRemoveContent, InSelfPluginName](const FString& InAssetPath, const FString& InContentPath)
		{
			if (const IPlugin* Plugin = IPluginManager::Get().FindPluginFromPath(InAssetPath).Get())
			{
				if (PluginUsesUs(InSelfPluginName, *Plugin))
				{
					// Stop using this plugin's content
					InOnPluginRemoveContent.ExecuteIfBound(*Plugin);
				}
			}
		}
	);
}

bool UGCBlueprintFunctionLibrary_ContentTools::PluginUsesUs(const FString& InSelfPluginName, const IPlugin& InPlugin)
{
	if (InPlugin.GetName() == InSelfPluginName)
	{
		// We are the plugin
		return true;
	}

	if (InPlugin.GetDescriptor().Plugins.ContainsByPredicate(
				[&InSelfPluginName](const FPluginReferenceDescriptor& InPluginReferenceDescriptor)
				{
					// If this plugin dependency is us
					return (InPluginReferenceDescriptor.Name == InSelfPluginName) && (InPluginReferenceDescriptor.bEnabled);
				}
			)
		)
	{
		// The plugin depends on us
		return true;
	}

	return false;
}
