// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



class FGameCoreModule : public FDefaultModuleImpl
{
public:
    //  BEGIN IModuleInterface
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    //  END IModuleInterface
};
