// Fill out your copyright notice in the Description page of Project Settings.

#include "Misc/AutomationTest.h"
#include "GCUtils_Math.h"

namespace
{
    IMPLEMENT_SIMPLE_AUTOMATION_TEST(
        FAutomationTest_GameCore_GCUtils_Math_GetCollisionShapeBoundingSphereRadius,
        "GameCore.GCUtils.Math.GetCollisionShapeBoundingSphereRadius",
        EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter);

    bool FAutomationTest_GameCore_GCUtils_Math_GetCollisionShapeBoundingSphereRadius::RunTest(const FString& parameters)
    {
        bool isSuccess = true;

        isSuccess = TestTrueExpr(FMath::IsNearlyEqual(GCUtils::Math::GetCollisionShapeBoundingSphereRadius(FCollisionShape::MakeBox(FVector3f(3.f, 5.f, 7.f))), FVector3f(3.f, 5.f, 7.f).Size())) && isSuccess;

        return isSuccess;
    }

    IMPLEMENT_SIMPLE_AUTOMATION_TEST(
        FAutomationTest_GameCore_GCUtils_Math_ArePointsCollinear,
        "GameCore.GCUtils.Math.ArePointsCollinear",
        EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter);

    bool FAutomationTest_GameCore_GCUtils_Math_ArePointsCollinear::RunTest(const FString& parameters)
    {
        bool isSuccess = true;

        isSuccess = TestTrueExpr(GCUtils::Math::ArePointsCollinear({ FVector(1.0, 0.0, 0.0) })) && isSuccess;

        isSuccess = TestTrueExpr(GCUtils::Math::ArePointsCollinear({ FVector(1.0, 0.0, 0.0), FVector(2.0, 0.0, 0.0) })) && isSuccess;

        isSuccess = TestTrueExpr(GCUtils::Math::ArePointsCollinear({ FVector(1.0, 0.0, 0.0), FVector(2.0, 0.0, 0.0), FVector(3.0, 0.0, 0.0) })) && isSuccess;

        isSuccess = TestTrueExpr(!GCUtils::Math::ArePointsCollinear({ FVector(1.0, 0.0, 0.0), FVector(2.0, 0.0, 0.0), FVector(3.0, 1.0, 0.0) })) && isSuccess;

        isSuccess = TestTrueExpr(GCUtils::Math::ArePointsCollinear({ FVector(1.0, 0.0, 0.0), FVector(2.0, 0.0, 0.0), FVector(3.0, 0.0, 0.0), FVector(4.0, 0.0, 0.0), FVector(5.0, 0.0, 0.0) })) && isSuccess;

        return isSuccess;
    }

    IMPLEMENT_SIMPLE_AUTOMATION_TEST(
        FAutomationTest_GameCore_GCUtils_Math_DoesPointLieOnTriangle,
        "GameCore.GCUtils.Math.DoesPointLieOnTriangle",
        EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter);

    bool FAutomationTest_GameCore_GCUtils_Math_DoesPointLieOnTriangle::RunTest(const FString& parameters)
    {
        bool isSuccess = true;

        isSuccess = TestTrueExpr(GCUtils::Math::DoesPointLieOnTriangle(FVector(0.0, 0.0, 0.0), FVector(1.0, 0.0, 0.0), FVector(0.0, 1.0, 0.0), FVector(0.25, 0.25, 0.0))) && isSuccess;

        isSuccess = TestTrueExpr(!GCUtils::Math::DoesPointLieOnTriangle(FVector(0.0, 0.0, 0.0), FVector(1.0, 0.0, 0.0), FVector(0.0, 1.0, 0.0), FVector(1.0, 1.0, 0.0))) && isSuccess;

#if 0 // Disabled. TODO: Make "inclusive" and "exclusive" versions of the function to handle a case like this (where the point lies on the edge).
        isSuccess = TestTrueExpr(GCUtils::Math::DoesPointLieOnTriangle(FVector(0.0, 0.0, 0.0), FVector(1.0, 0.0, 0.0), FVector(0.0, 1.0, 0.0), FVector(0.5, 0.5, 0.0))) && isSuccess;
#endif // 0

        return isSuccess;
    }

    IMPLEMENT_SIMPLE_AUTOMATION_TEST(
        FAutomationTest_GameCore_GCUtils_Math_LerpMultiple,
        "GameCore.GCUtils.Math.LerpMultiple",
        EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter);

    bool FAutomationTest_GameCore_GCUtils_Math_LerpMultiple::RunTest(const FString& parameters)
    {
        bool isSuccess = true;

        isSuccess = TestTrueExpr(FMath::IsNearlyEqual(GCUtils::Math::LerpMultiple<float>({ 1.f, 2.f, 3.f }, 0.f), 1.f)) && isSuccess;

        isSuccess = TestTrueExpr(FMath::IsNearlyEqual(GCUtils::Math::LerpMultiple<float>({ 1.f, 2.f, 3.f }, 0.5f), 2.f)) && isSuccess;

        isSuccess = TestTrueExpr(FMath::IsNearlyEqual(GCUtils::Math::LerpMultiple<float>({ 1.f, 2.f, 3.f }, 1.f), 3.f)) && isSuccess;

        isSuccess = TestTrueExpr(FMath::IsNearlyEqual(GCUtils::Math::LerpMultiple<float>({ 1.f, 2.f, 3.f }, -0.5f), 0.f)) && isSuccess;

        isSuccess = TestTrueExpr(FMath::IsNearlyEqual(GCUtils::Math::LerpMultiple<float>({ 1.f, 2.f, 3.f }, 1.5f), 4.f)) && isSuccess;

        isSuccess = TestTrueExpr(FMath::IsNearlyEqual(GCUtils::Math::LerpMultiple<float>({ 1.f, 2.f }, 2.f), 3.f)) && isSuccess;

        isSuccess = TestTrueExpr(FMath::IsNearlyEqual(GCUtils::Math::LerpMultiple<float>({ 1.f }, 1.5f), 1.f)) && isSuccess;

        isSuccess = TestTrueExpr(FMath::IsNearlyEqual(GCUtils::Math::LerpMultiple<float>({}, 1.5f), 0.f)) && isSuccess;

        return isSuccess;
    }
}
