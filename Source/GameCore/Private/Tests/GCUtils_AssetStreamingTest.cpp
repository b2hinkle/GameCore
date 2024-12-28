// Fill out your copyright notice in the Description page of Project Settings.

#if WITH_AUTOMATION_WORKER

#include "Misc/AutomationTest.h"
#include "GCUtils_AssetStreaming.h"
#include "GCUtils_AssetStreaming.inl"
#include "Engine/AssetManager.h"
#include "GCUtils_Asset.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Pawn.h"
#include "AI/Navigation/NavAgentInterface.h"

#define TEST_STATIC_MESH_ACTOR_BP_PACKAGE_PATH_STRING_LITERAL TEXT("/GameCore/Tests/BP_GameCore_StaticMeshActor_Test")

#define TEST_STATIC_MESH_ACTOR_BP_CLASS_NAME_STRING_LITERAL TEXT("BP_GameCore_StaticMeshActor_Test") GC_BLUEPRINT_GENERATED_CLASS_POSTFIX_STRING_LITERAL

#define TEST_STATIC_MESH_ACTOR_BP_CDO_NAME_STRING_LITERAL DEFAULT_OBJECT_PREFIX TEST_STATIC_MESH_ACTOR_BP_CLASS_NAME_STRING_LITERAL

#define TEST_CHARACTER_BP_PACKAGE_PATH_STRING_LITERAL TEXT("/GameCore/Tests/BP_GameCore_Character_Test")

#define TEST_CHARACTER_BP_CLASS_NAME_STRING_LITERAL TEXT("BP_GameCore_Character_Test") GC_BLUEPRINT_GENERATED_CLASS_POSTFIX_STRING_LITERAL

#define TEST_CHARACTER_BP_CDO_NAME_STRING_LITERAL DEFAULT_OBJECT_PREFIX TEST_CHARACTER_BP_CLASS_NAME_STRING_LITERAL

namespace
{
    constexpr FStringView TestStaticMeshActorBPPackagePath =
        PREPROCESSOR_JOIN(TEST_STATIC_MESH_ACTOR_BP_PACKAGE_PATH_STRING_LITERAL, _PrivateSV);

    constexpr FStringView TestStaticMeshActorBPClassName =
        PREPROCESSOR_JOIN(TEST_STATIC_MESH_ACTOR_BP_CLASS_NAME_STRING_LITERAL, _PrivateSV);

    constexpr FStringView TestStaticMeshActorBPCDOName =
        PREPROCESSOR_JOIN(TEST_STATIC_MESH_ACTOR_BP_CDO_NAME_STRING_LITERAL, _PrivateSV);

    constexpr FStringView TestCharacterBPPackagePath =
        PREPROCESSOR_JOIN(TEST_CHARACTER_BP_PACKAGE_PATH_STRING_LITERAL, _PrivateSV);

    constexpr FStringView TestCharacterBPClassName =
        PREPROCESSOR_JOIN(TEST_CHARACTER_BP_CLASS_NAME_STRING_LITERAL, _PrivateSV);

    constexpr FStringView TestCharacterBPCDOName =
        PREPROCESSOR_JOIN(TEST_CHARACTER_BP_CDO_NAME_STRING_LITERAL, _PrivateSV);

    IMPLEMENT_SIMPLE_AUTOMATION_TEST(
        FAutomationTest_GameCore_GCUtils_AssetStreaming_LoadSync,
        "GameCore.GCUtils.AssetStreaming.LoadSync",
        EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

    bool FAutomationTest_GameCore_GCUtils_AssetStreaming_LoadSync::RunTest(const FString& parameters)
    {
        bool isSuccess = true;

        // Test loading bad asset paths.
        {
            using TAllocator = TFixedAllocator<1u>;
            constexpr bool shouldManageActiveHandle = true;
            constexpr bool shouldReportErrors = false;

            TArray<FSoftObjectPath> assetPaths;
            assetPaths.Emplace(
                FSoftObjectPath(
                    FTopLevelAssetPath(
                        FName(TEXT("/Game/kajflkdsjafk/gibberish/yooahd/beem")),
                        FName(TEXT("boom"))
                        )
                    )
                );

            const int32 numAssetsToLoad = assetPaths.Num();

            TSharedPtr<FStreamableHandle> streamableHandle =
                GCUtils::AssetStreaming::LoadSync<shouldManageActiveHandle, shouldReportErrors>(
                    UAssetManager::Get().GetStreamableManager(),
                    MoveTemp(assetPaths));

            isSuccess = TestTrueExpr(streamableHandle.IsValid()) && isSuccess;

            TArray<UObject*, TAllocator> loadedAssets;
            if (streamableHandle)
            {
                loadedAssets =
                    GCUtils::AssetStreaming::GetLoadedAssets<TAllocator, shouldReportErrors>(
                        streamableHandle.ToSharedRef());
            }

            isSuccess = TestTrueExpr(loadedAssets.Num() == numAssetsToLoad) && isSuccess;

            if (loadedAssets.IsEmpty() == false)
            {
                // Since our asset path was gibberish, the loaded asset should be null.
                isSuccess = TestTrueExpr(loadedAssets[0] == nullptr) && isSuccess;
            }
        }

        // Test loading good asset path.
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

            TSharedPtr<FStreamableHandle> streamableHandle =
                GCUtils::AssetStreaming::LoadSync<shouldManageActiveHandle, shouldReportErrors>(
                    UAssetManager::Get().GetStreamableManager(),
                    MoveTemp(assetPaths));

            isSuccess = TestTrueExpr(streamableHandle.IsValid()) && isSuccess;

            TArray<UClass*, TAllocator> loadedAssets;
            if (streamableHandle)
            {
                loadedAssets =
                    GCUtils::AssetStreaming::GetLoadedAssets<TAllocator, shouldReportErrors, UClass>(
                        streamableHandle.ToSharedRef());
            }

            isSuccess = TestTrueExpr(loadedAssets.Num() == numAssetsToLoad) && isSuccess;

            if (loadedAssets.IsEmpty() == false)
            {
                isSuccess = TestTrueExpr(loadedAssets[0] != nullptr) && isSuccess;
            }
        }

        // Test "checked" function.
        {
            using TAllocator = TFixedAllocator<1u>;
            constexpr bool shouldManageActiveHandle = true;

            TArray<FSoftObjectPath> assetPaths;
            assetPaths.Emplace(
                FSoftObjectPath(
                    FTopLevelAssetPath(
                        FName(TestStaticMeshActorBPPackagePath),
                        FName(TestStaticMeshActorBPCDOName)
                        )
                    )
                );

            const int32 numAssetsToLoad = assetPaths.Num();

            TSharedRef<FStreamableHandle> streamableHandle =
                GCUtils::AssetStreaming::LoadSyncChecked<shouldManageActiveHandle>(
                    UAssetManager::Get().GetStreamableManager(),
                    MoveTemp(assetPaths));

            TArray<std::reference_wrapper<AStaticMeshActor>, TAllocator> loadedAssets =
                GCUtils::AssetStreaming::GetLoadedAssetsChecked<TAllocator, AStaticMeshActor>(
                    streamableHandle);

            isSuccess = TestTrueExpr(loadedAssets.Num() == numAssetsToLoad) && isSuccess;
        }

        // Test loading good asset path.
        {
            using TAllocator = TFixedAllocator<1u>;
            constexpr bool shouldManageActiveHandle = true;
            constexpr bool shouldReportErrors = true;

            TArray<FSoftObjectPath> assetPaths;
            assetPaths.Emplace(
                FSoftObjectPath(
                    FTopLevelAssetPath(
                        FName(TestCharacterBPPackagePath),
                        FName(TestCharacterBPCDOName)
                        )
                    )
                );

            const int32 numAssetsToLoad = assetPaths.Num();

            TSharedPtr<FStreamableHandle> streamableHandle =
                GCUtils::AssetStreaming::LoadSync<shouldManageActiveHandle, shouldReportErrors>(
                    UAssetManager::Get().GetStreamableManager(),
                    MoveTemp(assetPaths));

            isSuccess = TestTrueExpr(streamableHandle.IsValid()) && isSuccess;

            TArray<APawn*, TAllocator> loadedAssets;
            if (streamableHandle)
            {
                loadedAssets =
                    GCUtils::AssetStreaming::GetLoadedAssets<TAllocator, shouldReportErrors, APawn>(
                        streamableHandle.ToSharedRef());
            }

            isSuccess = TestTrueExpr(loadedAssets.Num() == numAssetsToLoad) && isSuccess;

            if (loadedAssets.IsEmpty() == false)
            {
                isSuccess = TestTrueExpr(loadedAssets[0] != nullptr) && isSuccess;
            }
        }

        // Test "checked" function.
        {
            using TAllocator = TFixedAllocator<1u>;
            constexpr bool shouldManageActiveHandle = true;

            TArray<FSoftObjectPath> assetPaths;
            assetPaths.Emplace(
                FSoftObjectPath(
                    FTopLevelAssetPath(
                        FName(TestCharacterBPPackagePath),
                        FName(TestCharacterBPCDOName)
                        )
                    )
                );

            const int32 numAssetsToLoad = assetPaths.Num();

            TSharedRef<FStreamableHandle> streamableHandle =
                GCUtils::AssetStreaming::LoadSyncChecked<shouldManageActiveHandle>(
                    UAssetManager::Get().GetStreamableManager(),
                    MoveTemp(assetPaths));

            TArray<std::reference_wrapper<INavAgentInterface>, TAllocator> loadedAssets =
                GCUtils::AssetStreaming::GetLoadedAssetsChecked<TAllocator, INavAgentInterface>(
                    streamableHandle);

            isSuccess = TestTrueExpr(loadedAssets.Num() == numAssetsToLoad) && isSuccess;
        }

        return isSuccess;
    }
}

#endif // #if WITH_AUTOMATION_WORKER
