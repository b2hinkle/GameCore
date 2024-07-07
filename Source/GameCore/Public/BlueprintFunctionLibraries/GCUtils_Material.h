// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GCUtils_Material.generated.h"



/**
 * A collection of helper functions related to Materials
 */
UCLASS()
class GAMECORE_API UGCBlueprintFunctionLibrary_MaterialHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Returns the MaterialIndex given the material's section index */
    UFUNCTION(BlueprintPure, Category = "MaterialHelpers|MaterialFinding")
        static int32 GetMaterialIndexFromSectionIndex(const UStaticMeshComponent* StaticMeshComponent, const int32 SectionIndex);

};
