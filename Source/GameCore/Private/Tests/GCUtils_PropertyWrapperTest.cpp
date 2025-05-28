// Fill out your copyright notice in the Description page of Project Settings.

#include "Misc/AutomationTest.h"
#include "Types/GCPropertyWrapperNative.h"

#if WITH_AUTOMATION_WORKER

namespace
{
    IMPLEMENT_SIMPLE_AUTOMATION_TEST(
        FAutomationTest_GameCore_Types_PropertyWrapper,
        "GameCore.Types.PropertyWrapper",
        EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::SmokeFilter);

    bool FAutomationTest_GameCore_Types_PropertyWrapper::RunTest(const FString& parameters)
    {
        bool isSuccess = true;

        FGCPropertyWrapperNative<int32> int32Wrapper(7);

        // Test default value
        isSuccess = TestTrueExpr(int32Wrapper == 7 && isSuccess);

        // Test assignment
        {
            int32Wrapper = 42;
            isSuccess = TestTrueExpr(int32Wrapper == 42 && isSuccess);

            int32Wrapper = 1;
            isSuccess = TestTrueExpr(int32Wrapper == 1 && isSuccess);
        }

        return isSuccess;
    }
}

#endif // WITH_AUTOMATION_WORKER
