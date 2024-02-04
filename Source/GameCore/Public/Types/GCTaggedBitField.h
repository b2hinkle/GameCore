// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GCUtils.h"

struct FGameplayTag;

// Primary template defined here to avoid clutter for the function implementations.
template
    <
    typename TBitFieldType,
    class = typename TEnableIf
        <
        TIsIntegral<TBitFieldType>::Value // Ensure integral type to avoid instantiations for types with custom operator overloads.
        >::Type
    >
struct TGCTaggedBitField;

/**
* Bit packing via gameplay tags.
*
* This can come in use especially when you want the benefits of sending packed bits over
* the network while still using gameplay tags.
*/
template <typename TBitFieldType>
struct TGCTaggedBitField<TBitFieldType>
{
    TGCTaggedBitField()
    {
        BitField = 0u;
    }
    TGCTaggedBitField(const TArray<FGameplayTag>& tags)
        : TGCTaggedBitField()
    {
        InitializeFlags(tags);
    }

    FORCEINLINE operator TBitFieldType() const
    {
        return BitField;
    }
    FORCEINLINE TGCTaggedBitField& operator=(const TGCTaggedBitField& other)
    {
        BitField = other.BitField;
        return *this;
    }
    FORCEINLINE bool operator==(const TGCTaggedBitField& other)
    {
        return BitField == other.BitField;
    }
    FORCEINLINE bool operator!=(const TGCTaggedBitField& other)
    {
        return !operator==(*this, other);
    }

    // Sets up how we interpret the bitfield (resets any previous interpretation)
    void InitializeFlags(const TArray<FGameplayTag>& tags);

    // Sets specified bit
    void SetFlag(const FGameplayTag& tag);

    // Clears specified bit
    void ClearFlag(const FGameplayTag& tag);

    // Checks specified bit
    bool IsFlagSet(const FGameplayTag& tag) const;

    // Unsets all bits
    void ClearFlags();

    FString GetDebugString();

    void Serialize(FArchive& ar, class UPackageMap* map, bool& outSuccess);
private:
    // Size of this type in bits
    static constexpr int32 TypeBitSize = sizeof(TBitFieldType) * 8;

private:
    TBitFieldType BitField = 0u;
    TMap<FGameplayTag, TBitFieldType> Masks;
};

template <typename TBitFieldType>
void TGCTaggedBitField<TBitFieldType>::InitializeFlags(const TArray<FGameplayTag>& tags)
{
    if (Masks.Num() > TypeBitSize)
    {
        checkf(0, TEXT("The amount of tags being used exceeds the amount of flags in the bit field. May want to consider using a larger type."))
            return;
    }

    Masks.Empty(tags.Num());
    for (int32 i = 0; i < tags.Num(); i++)
    {
        const TBitFieldType mask = 1u << i;
        Masks.Add(tags[i], mask);
    }
}

template <typename TBitFieldType>
void TGCTaggedBitField<TBitFieldType>::SetFlag(const FGameplayTag& tag)
{
    const TBitFieldType* mask = Masks.Find(tag);
    if (mask)
    {
        BitField |= *mask;
    }

    checkf(mask, TEXT("Tried setting a flag that doesn't exist. Make sure the flags are initialized with tags via ") GC_CSTRINGIZE(InitializeFlags()));
}

template <typename TBitFieldType>
void TGCTaggedBitField<TBitFieldType>::ClearFlag(const FGameplayTag& tag)
{
    const TBitFieldType* mask = Masks.Find(tag);
    if (mask)
    {
        BitField &= ~(*mask);
    }

    checkf(mask, TEXT("Tried clearing a flag that doesn't exist. Make sure the flags are initialized with tags via ") GC_CSTRINGIZE(InitializeFlags()));
}

template <typename TBitFieldType>
bool TGCTaggedBitField<TBitFieldType>::IsFlagSet(const FGameplayTag& tag) const
{
    const TBitFieldType* mask = Masks.Find(tag);
    if (mask)
    {
        return BitField & *mask;
    }

    checkf(mask, TEXT("Tried checking a flag that doesn't exist. Make sure the flags are initialized with tags via ") GC_CSTRINGIZE(InitializeFlags()));
    return false;
}

template <typename TBitFieldType>
void TGCTaggedBitField<TBitFieldType>::ClearFlags()
{
    BitField = 0u;
}

template <typename TBitFieldType>
FString TGCTaggedBitField<TBitFieldType>::GetDebugString()
{
    TStringBuilder<TypeBitSize + 1> retVal;
    for (int32 i = 0; i < TypeBitSize; i++)
    {
        const bool isBitSet = (BitField >> i) & 1u;
        retVal.Append(isBitSet ? "1" : "0");
    }

    return retVal.ToString();
}

template <typename TBitFieldType>
void TGCTaggedBitField<TBitFieldType>::Serialize(FArchive& ar, UPackageMap* map, bool& outSuccess)
{
    outSuccess = true;
    SerializeOptionalValue<TBitFieldType>(ar.IsSaving(), ar, BitField, 0);
}
