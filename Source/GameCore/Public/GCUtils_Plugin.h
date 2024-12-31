// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Templates/SharedPointer.h"
#include "Containers/StringFwd.h"

class IPlugin;

/**
 * @brief Utilities for project plugins.
 *
 * TODO: Revisit these utilities and maybe improve intended behavior and documentation.
 */
namespace GCUtils::Plugin
{
    DECLARE_DELEGATE_OneParam(FPluginRefNativeDelegate, TSharedRef<IPlugin>&&);

    /**
     * @brief Set up using content from plugins that "opt in" to us doing so. "Opting in" means
     *        they list us as a dependency.
     * @param inSelfPluginNameString Name of the plugin that wants to use content of its dependent plugins.
     * @param inOnPluginAddContent Executed when there is content for us to use.
     * @param inOnPluginRemoveContent Executed when there is content for us to stop using.
     */
    GAMECORE_API void UseContentFromDependentPlugins(
        const FStringView& inSelfPluginNameString,
        FPluginRefNativeDelegate&& inOnPluginAddContentCallback,
        FPluginRefNativeDelegate&& inOnPluginRemoveContentCallback);

    /**
     * @brief Determine whether a plugin "opts in" to us using their content. Plugins that depend on us "opt in".
     * @note Is unconcerned of whether the plugin content is mounted or not, e.g., via FPackageName::MountPointExists().
     * @return Whether the plugin "opts in" to us using their content.
     */
    GAMECORE_API bool DoesPluginUseUs(
        const FStringView& inSelfPluginNameString,
        const TSharedRef<const IPlugin>& inPlugin);
}
