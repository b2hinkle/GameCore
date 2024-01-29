// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class IPlugin;

/**
 * Tools for working with content.
 */
namespace GCUtils::Plugin
{
	/**
	 * Set up using content from plugins that "opt in" to us doing so. "Opting in" means that they list us as a dependency.
	 *
	 * @param  InSelfPluginName           Name of the plugin that wants to use content of its dependent plugins.
	 * @param  InOnPluginAddContent       Executed when there is content for us to use.
	 * @param  InOnPluginRemoveContent    Executed when there is content for us to stop using.
	 */
	GAMECORE_API void UseContentFromDependentPlugins(const FString& InSelfPluginName, const TDelegate<void(const IPlugin&)>& InOnPluginAddContent, const TDelegate<void(const IPlugin&)>& InOnPluginRemoveContent);

	/**
	 * Returns if a plugin "opts in" to us using their content. Plugins that depend on us "opt in".
	 *
	 * NOTE: Does not check whether the plugin content is mounted or not, e.g., via FPackageName::MountPointExists().
	 */
	GAMECORE_API bool PluginUsesUs(const FString& InSelfPluginName, const IPlugin& InPlugin);
}
