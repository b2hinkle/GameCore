// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Core/PushModel/PushModel.h"
#include "GameCore/Private/Utilities/GCLogCategories.h"
#include "Kismet/KismetSystemLibrary.h"

#include "GCPropertyWrapperBase.generated.h"




/**
 * FGCPropertyWrapperBase
 * 
 * Property wrappers give you more control over your variables. It provides a delegate that notifies when the value of the property changes.
 * For its implementation, since UStructs don't support templates, we are making use of inheritence and a generic GC_PROPERTY_WRAPPER_CHILD_BODY macro.
 * 
 * Subclass' responsibilities:
 *	- Put GC_PROPERTY_WRAPPER_CHILD_BODY() anywhere in the struct body and provide it with the required parameters to generate required boilerplate code.
 *	- Declare your Value member as a UPROPERTY. EditAnywhere and BlueprintReadOnly is genrally what we use for it so it's functional in blueprint. Child classes have to declare the value themselves since Unreal Header Tool can't see generated code.
 *	- Implement the pure virtual ToString()
 */
USTRUCT(BlueprintType)
struct GAMECORE_API FGCPropertyWrapperBase
{
	GENERATED_BODY()

public:
	FGCPropertyWrapperBase();
	virtual ~FGCPropertyWrapperBase() { }
protected:
	FGCPropertyWrapperBase(UObject* InOuter, const FName& InPropertyName, const UScriptStruct* InChildScriptStruct); // initialization is intended only for child structs
public:
	/** Marks the property dirty */
	void MarkNetDirty();

	FProperty* GetSelfPropertyPointer() const { return SelfPropertyPointer.Get(); }
	UObject* GetOuter() const { return Outer.Get(); }
	FName GetPropertyName() const { return SelfPropertyPointer->GetFName(); }

	/**
	 * Our custom serialization for this struct
	 * It makes more sense to do serialization work in here since the engine uses this more than operator<<().
	 */
	virtual bool Serialize(FArchive& InOutArchive) PURE_VIRTUAL(FGCPropertyWrapperBase::Serialize, return false; );

	/** Our custom replication for this struct */
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) PURE_VIRTUAL(FGCPropertyWrapperBase::NetSerialize, return false; );

	/** Return the ToString() of Value. */
	virtual FString ToString() const PURE_VIRTUAL(FGCPropertyWrapperBase::ToString, return FString(); );

	/** Debug string displaying our name and value */
	FString GetDebugString(bool bDetailedDebugString = false) const;

protected:
	/** The pointer to our outer - used for push model's marking net dirty */
	UPROPERTY(Transient)
		TWeakObjectPtr<UObject> Outer;

	/** The pointer to the FProperty on our outer's UClass */
	UPROPERTY(Transient)
		TFieldPath<FProperty> SelfPropertyPointer;

	/** The pointer to the Value FProperty in subclasses. Having FProperty is nice because it allows us to implement Serialize() and NetSerialize() generically */
	UPROPERTY(Transient)
		TFieldPath<FProperty> ValueProperty;
};


#define GC_PROPERTY_WRAPPER_CHILD_BODY(PropertyWrapperType, ValueType, DefaultValue) \
private:\
void InitializeValueProperty()\
{\
	ValueProperty = FindFProperty<FProperty>(StaticStruct(), GET_MEMBER_NAME_CHECKED(PropertyWrapperType, Value));\
}\
\
public:\
PropertyWrapperType()\
	: Value(DefaultValue)\
{\
	InitializeValueProperty();\
}\
PropertyWrapperType(UObject* InOuter, const FName& InPropertyName, const ValueType& InValue = DefaultValue)\
	: FGCPropertyWrapperBase(InOuter, InPropertyName, GetScriptStruct())\
	, Value(InValue)\
{\
	InitializeValueProperty();\
}\
\
/** Broadcasted whenever Value changes */\
TMulticastDelegate<void(PropertyWrapperType&, const ValueType&, const ValueType&)> ValueChangeDelegate;\
\
virtual UScriptStruct* GetScriptStruct() const { return StaticStruct(); }\
\
/** Implements implicit conversion from this struct to Value's type. Allows you to treat this struct as its Value's type in code */\
operator ValueType() const\
{\
	return Value;\
}\
\
/** Broadcasts ValueChangeDelegate */\
ValueType operator=(const ValueType& NewValue)\
{\
	const ValueType OldValue = Value;\
	Value = NewValue;\
\
	if (NewValue != OldValue)\
	{\
		ValueChangeDelegate.Broadcast(*this, OldValue, NewValue);\
	}\
\
	return Value;\
}\
\
/** Uses our custom serialization */\
friend FArchive& operator<<(FArchive& InOutArchive, PropertyWrapperType& InOutPropertyWrapper)\
{\
	InOutPropertyWrapper.Serialize(InOutArchive);\
	return InOutArchive;\
}\
\
/* Implements a generic Serialize() by making use of Value's FProperty */ \
virtual bool Serialize(FArchive& Ar) override\
{\
	if (Ar.IsSaving())\
	{\
		ValueProperty->SerializeItem(FStructuredArchiveFromArchive(Ar).GetSlot(), &Value);\
	}\
\
	if (Ar.IsLoading())\
	{\
		ValueType NewValue;\
		ValueProperty->SerializeItem(FStructuredArchiveFromArchive(Ar).GetSlot(), &NewValue);\
		operator=(NewValue);\
	}\
\
	return true;\
}\
\
/* Implements a generic NetSerialize() by making use of Value's FProperty */ \
virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override\
{\
	bool bSuccess = true;\
\
	if (Ar.IsSaving())\
	{\
		bSuccess = ValueProperty->NetSerializeItem(Ar, Map, &Value);\
	}\
\
	if (Ar.IsLoading())\
	{\
		ValueType NewValue;\
		bSuccess = ValueProperty->NetSerializeItem(Ar, Map, &NewValue);\
		operator=(NewValue); /* use our correct path for setting Value */ \
	}\
\
	return bSuccess;\
}

// BEGIN Property wrapper on change helpers
template <class TPropertyWrapperType, class TPropertyWrapperValueType>
static void GCPropertyWrapperOnChangeMarkNetDirty(TPropertyWrapperType& InPropertyWrapper, const TPropertyWrapperValueType& InOldValue, const TPropertyWrapperValueType& InNewValue)
{
	InPropertyWrapper.MarkNetDirty();
}

template <class TPropertyWrapperType, class TPropertyWrapperValueType>
static void GCPropertyWrapperOnChangePrintString(TPropertyWrapperType& InPropertyWrapper, const TPropertyWrapperValueType& InOldValue, const TPropertyWrapperValueType& InNewValue)
{
	UKismetSystemLibrary::PrintString(InPropertyWrapper.GetOuter(), InPropertyWrapper.GetDebugString(), true, false);
}

template <class TPropertyWrapperType, class TPropertyWrapperValueType>
static void GCPropertyWrapperOnChangeLog(TPropertyWrapperType& InPropertyWrapper, const TPropertyWrapperValueType& InOldValue, const TPropertyWrapperValueType& InNewValue)
{
	UE_LOG(LogGCPropertyWrapper, Log, InPropertyWrapper.GetDebugString());
}
// END Property wrapper on change helpers
