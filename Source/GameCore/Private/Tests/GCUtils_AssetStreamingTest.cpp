// Fill out your copyright notice in the Description page of Project Settings.

#if WITH_AUTOMATION_WORKER

#include "Misc/AutomationTest.h"
#include "GCUtils_AssetStreaming.h"
#include "GCUtils_AssetStreaming.inl"
#include "Engine/AssetManager.h"
#include "GCUtils_Asset.h"

namespace
{
    constexpr FStringView TestStaticMeshActorBPPackagePath =
        TEXTVIEW("/GameCore/Tests/BP_GameCore_StaticMeshActor_Test");

    constexpr FStringView TestStaticMeshActorBPClassName =
        PREPROCESSOR_JOIN(TEXT("BP_GameCore_StaticMeshActor_Test") GC_BLUEPRINT_GENERATED_CLASS_POSTFIX_STRING_LITERAL, _PrivateSV);

    IMPLEMENT_SIMPLE_AUTOMATION_TEST(
        FAutomationTest_GameCore_GCUtils_AssetStreaming_LoadSync,
        "GameCore.GCUtils.AssetStreaming.LoadSync",
        EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

    bool FAutomationTest_GameCore_GCUtils_AssetStreaming_LoadSync::RunTest(const FString& parameters)
    {
        bool isSuccess = true;

        {
            using TAllocator = TFixedAllocator<1u>;
            constexpr bool shouldManageActiveHandle = true;
            constexpr bool shouldReportErrors = true;

            TArray<FSoftObjectPath> assetPaths;
            assetPaths.Emplace(
                FSoftObjectPath(
                    FTopLevelAssetPath(
                        FName(TestStaticMeshActorBPPackagePath),
                        FName(TestStaticMeshActorBPClassName)
                        )
                    )
                );

            const int32 numAssetsToLoad = assetPaths.Num();

            TSharedPtr<FStreamableHandle> streamableHandle;

            TArray<UObject*, TAllocator> loadedAssets =
                GCUtils::AssetStreaming::LoadSync<TAllocator, shouldManageActiveHandle, shouldReportErrors>(
                    UAssetManager::Get().GetStreamableManager(),
                    MoveTemp(assetPaths),
                    streamableHandle);

            isSuccess = TestTrueExpr(streamableHandle.IsValid()) && isSuccess;
            isSuccess = TestTrueExpr(loadedAssets.Num() == numAssetsToLoad) && isSuccess;
        }

        {
            using TAllocator = TFixedAllocator<1u>;
            constexpr bool shouldManageActiveHandle = true;

            TArray<FSoftObjectPath> assetPaths;
            assetPaths.Emplace(
                FSoftObjectPath(
                    FTopLevelAssetPath(
                        FName(TestStaticMeshActorBPPackagePath),
                        FName(TestStaticMeshActorBPClassName)
                        )
                    )
                );

            const int32 numAssetsToLoad = assetPaths.Num();

            TSharedPtr<FStreamableHandle> streamableHandle;

            TArray<UObject*, TAllocator> loadedAssets =
                GCUtils::AssetStreaming::LoadSyncChecked<TAllocator, shouldManageActiveHandle>(
                    UAssetManager::Get().GetStreamableManager(),
                    MoveTemp(assetPaths),
                    streamableHandle);

            isSuccess = TestTrueExpr(streamableHandle.IsValid()) && isSuccess;
            isSuccess = TestTrueExpr(loadedAssets.Num() == numAssetsToLoad) && isSuccess;
        }

        return isSuccess;
    }
}

#endif // #if WITH_AUTOMATION_WORKER
