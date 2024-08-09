// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Concepts/StaticStructProvider.h"
#include "Concepts/BaseStructureProvider.h"

/**
 *
 */
template <class T>
consteval bool GCHasStaticScriptStruct()
{
    return TModels<CStaticStructProvider, T>::Value || TModels<CBaseStructureProvider, T>::Value;
}
