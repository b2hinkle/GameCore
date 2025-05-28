// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Types/GCPropertyWrapperBase2.h"

#include "GCPropertyWrappers2.generated.h"

GAMECORE_API DECLARE_LOG_CATEGORY_EXTERN(LogGCPropertyWrappers2, Log, All);

struct FGCInt32PropertyWrapper2;

// Define our value type.
template <>
struct GAMECORE_API TGCPropertyWrapperValueTypeTrait<FGCInt32PropertyWrapper2>
{
    using FValueType = int32;
};

/**
 *
 */
USTRUCT(BlueprintType)
struct GAMECORE_API FGCInt32PropertyWrapper2
#if CPP // Templates confuse UHT. UHT doesn't need to do anything with it anyway so have it ignore.
    : public TGCPropertyWrapperBase2<FGCInt32PropertyWrapper2>
#endif // #if CPP
{
    GENERATED_BODY()

    friend TGCPropertyWrapperBase2<FGCInt32PropertyWrapper2>;

public:

    bool Serialize(FArchive& ar)
    {
        // Serialize the value.
        ar << Value;
        return true;
    }

    bool NetSerialize(FArchive& ar, UPackageMap* map, bool& outSuccess)
    {
        // Serialize the value.
        ar << Value;
        outSuccess = true;
        return outSuccess;
    }

protected:

    /**
     * The actual value of this float property. No BP write access since we must go through the setter.
     */
    //UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Value = 0;
};

template <>
struct TStructOpsTypeTraits<FGCInt32PropertyWrapper2> : public TStructOpsTypeTraitsBase2<FGCInt32PropertyWrapper2>
{
    enum
    {
        WithSerializer = true,
        WithNetSerializer = true
    };
};
