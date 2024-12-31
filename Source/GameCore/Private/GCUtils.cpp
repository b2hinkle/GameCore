// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils.h"

#include "GCUtils_ObjectTraversal.h"

const AController* GCUtils::GetController(const UObject* inObject)
{
    const AActor* actor = ObjectTraversal::GetTypedSelfOrOuter<AActor>(inObject);
    return ObjectTraversal::GetTypedSelfOrOwnerActor<AController>(actor);
}
AController* GCUtils::GetController(UObject* inObject)
{
    return const_cast<AController*>(GetController(const_cast<const UObject*>(inObject)));
}
