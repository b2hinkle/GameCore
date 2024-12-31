// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"

class UStaticMeshComponent;

/**
 * @brief Material utilities.
 */
namespace GCUtils::Material
{
    /**
     * @brief Returns the material index given the material's section index.
     * @note This is an old function who's behavior hasn't been reviewed or improved.
     */
    GAMECORE_API int32 GetMaterialIndexFromSectionIndex(
        const UStaticMeshComponent& staticMeshComponent,
        const int32 sectionIndex);
}
