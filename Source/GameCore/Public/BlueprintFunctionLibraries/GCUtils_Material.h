// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"

/**
 * A collection of helper functions related to Materials
 */
namespace GCUtils::Material
{
    /** Returns the MaterialIndex given the material's section index */
    GAMECORE_API int32 GetMaterialIndexFromSectionIndex(const UStaticMeshComponent* StaticMeshComponent, const int32 SectionIndex);
}
