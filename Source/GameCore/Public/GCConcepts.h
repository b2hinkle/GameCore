// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "type_traits"

namespace GCConcepts
{
    template <class T>
    concept Pointer = std::is_pointer_v<T>;

    template <class T>
    concept Reference = std::is_reference_v<T>;
}
