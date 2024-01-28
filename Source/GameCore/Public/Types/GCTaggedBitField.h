// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FGameplayTag;

namespace GCUtils
{
    /**
     * Bit packing via gameplay tags
     */
    template <typename TBitFieldType>
    struct FGCTaggedBitField
    {
        FGCTaggedBitField()
        {
            BitField = 0u;
        }
        FGCTaggedBitField(const TArray<FGameplayTag>& tags)
            : FGCTaggedBitField()
        {
            InitializeFlags(tags);
        }

        FORCEINLINE operator TBitFieldType() const
        {
            return BitField;
        }
        FORCEINLINE FGCTaggedBitField& operator=(const FGCTaggedBitField& other)
        {
            BitField = other.BitField;
            return *this;
        }
        FORCEINLINE bool operator==(const FGCTaggedBitField& other)
        {
            return BitField == other.BitField;
        }
        FORCEINLINE bool operator!=(const FGCTaggedBitField& other)
        {
            return !(*this == other);
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

    template<typename TBitFieldType>
    void FGCTaggedBitField<TBitFieldType>::InitializeFlags(const TArray<FGameplayTag>& tags)
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

    template<typename TBitFieldType>
    void FGCTaggedBitField<TBitFieldType>::SetFlag(const FGameplayTag& tag)
    {
        const TBitFieldType* mask = Masks.Find(tag);
        if (mask)
        {
            BitField |= *mask;
        }

        checkf(mask, TEXT("Tried setting a flag that doesn't exist. Make sure the flags are initialized with tags via InitializeFlags()"));
    }

    template<typename TBitFieldType>
    void FGCTaggedBitField<TBitFieldType>::ClearFlag(const FGameplayTag& tag)
    {
        const TBitFieldType* mask = Masks.Find(tag);
        if (mask)
        {
            BitField &= ~(*mask);
        }

        checkf(mask, TEXT("Tried clearing a flag that doesn't exist. Make sure the flags are initialized with tags via InitializeFlags()"));
    }

    template<typename TBitFieldType>
    bool FGCTaggedBitField<TBitFieldType>::IsFlagSet(const FGameplayTag& tag) const
    {
        const TBitFieldType* mask = Masks.Find(tag);
        if (mask)
        {
            return BitField & *mask;
        }

        checkf(mask, TEXT("Tried checking a flag that doesn't exist. Make sure the flags are initialized with tags via InitializeFlags()"));
        return false;
    }

    template<typename TBitFieldType>
    void FGCTaggedBitField<TBitFieldType>::ClearFlags()
    {
        BitField = 0u;
    }

    template<typename TBitFieldType>
    FString FGCTaggedBitField<TBitFieldType>::GetDebugString()
    {
        TStringBuilder<TypeBitSize + 1> retVal;
        for (int32 i = 0; i < TypeBitSize; i++)
        {
            const bool isBitSet = (BitField >> i) & 1u;
            retVal.Append(isBitSet ? "1" : "0");
        }

        return retVal.ToString();
    }

    template<typename TBitFieldType>
    void FGCTaggedBitField<TBitFieldType>::Serialize(FArchive& ar, UPackageMap* map, bool& outSuccess)
    {
        outSuccess = true;
        SerializeOptionalValue<TBitFieldType>(ar.IsSaving(), ar, BitField, 0);
    }

}
