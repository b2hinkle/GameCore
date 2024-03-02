// Fill out your copyright notice in the Description page of Project Settings.


#include "Types/PropertyWrappers/GCPropertyWrapperBase.h"



FGCPropertyWrapperBase::FGCPropertyWrapperBase()
    : Outer(nullptr)
    , SelfPropertyPointer(nullptr)
    , ValueProperty(nullptr)
{
}
FGCPropertyWrapperBase::FGCPropertyWrapperBase(UObject* InOuter, const FName& InPropertyName, const UScriptStruct* InChildScriptStruct)
    : FGCPropertyWrapperBase()
{
    Outer = InOuter;
    SelfPropertyPointer = FindFProperty<FProperty>(Outer->GetClass(), InPropertyName); // get the FProperty so we can use push model with it

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
    // Ensure this property exists on the owner and that it is an InChildScriptStruct!
    {
        if (!SelfPropertyPointer.Get())
        {
            UE_LOG(LogGCPropertyWrapper, Error, TEXT("%s(): The given InPropertyName \"%s\" was not found on the Outer ``%s``. Ensure correct spelling for the property you are looking for and make sure that it is a UPROPERTY so we can find it!"), ANSI_TO_TCHAR(__FUNCTION__), *(InPropertyName.ToString()), *(InOuter->GetName()));
            check(0);
        }
        const FStructProperty* StructProperty = CastField<FStructProperty>(SelfPropertyPointer.Get());
        if ((StructProperty && StructProperty->Struct == InChildScriptStruct) == false)
        {
            UE_LOG(LogGCPropertyWrapper, Error, TEXT("%s(): The given FProperty ``%s::%s`` is not a(n) %s!"), ANSI_TO_TCHAR(__FUNCTION__), *(InOuter->GetClass()->GetName()), *(SelfPropertyPointer->GetFName().ToString()), *(InChildScriptStruct->GetName()));
            check(0);
        }
    }
#endif // !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
}

void FGCPropertyWrapperBase::MarkNetDirty()
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
    // Ensure that this property is replicated
    if (!SelfPropertyPointer->HasAnyPropertyFlags(EPropertyFlags::CPF_Net)) // i want to use IS_PROPERTY_REPLICATED() here but, in non-Editor target, we for some reason get an "error C3861: 'IS_PROPERTY_REPLICATED': identifier not found"
    {
        UE_LOG(LogGCPropertyWrapper, Error, TEXT("%s(): Tried to mark property net dirty to replicate, but ``%s::%s`` is not replicated! Cannot replicate!"), ANSI_TO_TCHAR(__FUNCTION__), GetData(Outer->GetClass()->GetName()), GetData(SelfPropertyPointer->GetName()));
    }
    // Ensure Push Model is enabled
    if (!IS_PUSH_MODEL_ENABLED())
    {
        UE_LOG(LogGCPropertyWrapper, Error, TEXT("%s(): Tried to mark property ``%s::%s`` net dirty, but Push Model is disabled! Cannot replicate!"), ANSI_TO_TCHAR(__FUNCTION__), GetData(Outer->GetClass()->GetName()), GetData(SelfPropertyPointer->GetName()));
    }
#endif // !(UE_BUILD_SHIPPING || UE_BUILD_TEST)

    MARK_PROPERTY_DIRTY(Outer.Get(), SelfPropertyPointer.Get());
}

FString FGCPropertyWrapperBase::GetDebugString(bool bDetailedDebugString) const
{
    if (bDetailedDebugString)
    {
        return Outer->GetPathName(Outer->GetTypedOuter<ULevel>()) + TEXT(".") + SelfPropertyPointer->GetName() + TEXT(": ") + ToString();
    }

    return SelfPropertyPointer->GetName() + TEXT(": ") + ToString();
}
