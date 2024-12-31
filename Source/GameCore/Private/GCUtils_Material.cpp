// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_Material.h"

int32 GCUtils::Material::GetMaterialIndexFromSectionIndex(
    const UStaticMeshComponent& staticMeshComponent,
    const int32 sectionIndex)
{
    // Adapted from UStaticMeshComponent::GetMaterialFromCollisionFaceIndex().

    if (sectionIndex < 0)
    {
        return INDEX_NONE;
    }

    const UStaticMesh* staticMesh = staticMeshComponent.GetStaticMesh();
    if (!staticMesh)
    {
        return INDEX_NONE;
    }

    const FStaticMeshRenderData* staticMeshRenderData = staticMesh->GetRenderData();
    if (!staticMeshRenderData)
    {
        return INDEX_NONE;
    }

    const FStaticMeshLODResourcesArray& lodResourcesArray = staticMeshRenderData->LODResources;

    const int32 lodIndex = staticMesh->LODForCollision;
    if (lodResourcesArray.IsValidIndex(lodIndex) == false)
    {
        return INDEX_NONE;
    }

    const FStaticMeshLODResources& lodResource = lodResourcesArray[lodIndex];

    if (sectionIndex >= lodResource.Sections.Num())
    {
        return INDEX_NONE;
    }

    return lodResource.Sections[sectionIndex].MaterialIndex;
}
