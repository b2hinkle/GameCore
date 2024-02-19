// Fill out your copyright notice in the Description page of Project Settings.

#include "GCUtils_Log.h"

const TCHAR* GCUtils::Log::GetIsControllerLocalCString(const UObject* inObject)
{
    if (!inObject)
    {
        return TEXT("Null ") GC_CSTRINGIZE(inObject);
    }

    const AController* controller = GetController(inObject);
    if (!controller)
    {
        return TEXT("Null ") GC_CSTRINGIZE(controller);
    }

    return BoolToCString(controller->IsLocalController());
}
