// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameDelegates.h"
#include "ShooterGame.h"
#include "Online/ShooterGameState.h"
#include "Online/ShooterPlayerState.h"
#include "GameDelegates.h"
#include "IPlatformFilePak.h"

#include "UObject/PackageReload.h"

//#include "Runtime/RHI/Public/RHICommandlist.h"

// Global struct for registering delegates super early
struct FShooterGameGlobalDelegateInit
{
	FShooterGameGlobalDelegateInit()
	{
		FPakPlatformFile::FPakSigningFailureHandlerData& HandlerData = FPakPlatformFile::GetPakSigningFailureHandlerData();
		{
			FScopeLock Lock(&HandlerData.Lock);
			HandlerData.ChunkSignatureCheckFailedDelegate.AddStatic(FShooterGameGlobalDelegateInit::HandlePakChunkSignatureCheckFailed);
			HandlerData.MasterSignatureTableCheckFailedDelegate.AddStatic(FShooterGameGlobalDelegateInit::HandlePakMasterSignatureTableCheckFailure);
		}

		FPakPlatformFile::GetPakSetIndexSettingsDelegate().BindStatic(GetPakSetIndexSettings);
	}

	static void HandlePakChunkSignatureCheckFailed(const FPakChunkSignatureCheckFailedData& Data)
	{
		UE_LOG(LogShooter, Fatal, TEXT("Pak chunk signature check failed!"));
	}

	static void HandlePakMasterSignatureTableCheckFailure(const FString& InPakFilename)
	{
		UE_LOG(LogShooter, Fatal, TEXT("Pak master signature table check failed for pak '%s'"), *InPakFilename);
	}

	static void GetPakSetIndexSettings(bool& bKeepFullDirectory, bool& bValidatePruning, bool& bDelayPruning)
	{
		// Keep the full directory of filenames in PakFileIndexes, so that FindOrLoadAssetsByPath will be able to find files in a given path
		bKeepFullDirectory = true;
	}
}
GShooterGameGlobalDelegateInit;


// respond to requests from a companion app
static void WebServerDelegate(int32 UserIndex, const FString& Action, const FString& URL, const TMap<FString, FString>& Params, TMap<FString, FString>& Response)
{
	if (URL == TEXT("/index.html?scoreboard"))
	{
		FString ScoreboardStr = TEXT("{ \"scoreboard\" : [ ");

		// you shouldn't normally use this method to get a UWorld as it won't always be correct in a PIE context.
		// However, the PS4 companion app server will never run in the Editor.
		UGameEngine* GameEngine = CastChecked<UGameEngine>(GEngine);
		if (GameEngine)
		{
			UWorld* World = GameEngine->GetGameWorld();
			if (World)
			{
				ULocalPlayer *Player = GEngine->GetFirstGamePlayer(World);
				if (Player)
				{
					// get the shoter game
					AShooterGameState* const GameState = Player->PlayerController->GetWorld()->GetGameState<AShooterGameState>();


					RankedPlayerMap Players;
					GameState->GetRankedMap(0, Players);

					bool bNeedsComma = false;
					for (auto It = Players.CreateIterator(); It; ++It)
					{
						if (bNeedsComma)
						{
							ScoreboardStr += TEXT(" ,");
						}
						ScoreboardStr += FString::Printf(TEXT(" { \"n\" : \"%s\" , \"k\" : \"%d\" , \"d\" : \"%d\" }"), *It.Value()->GetShortPlayerName(), It.Value()->GetKills(), It.Value()->GetDeaths());
						bNeedsComma = true;
					}
				}

				ScoreboardStr += TEXT(" ] }");

				Response.Add(TEXT("Content-Type"), TEXT("text/html; charset=utf-8"));
				Response.Add(TEXT("Body"), ScoreboardStr);
			}
		}
	}
}

static void ExtendedSaveGameInfoDelegate(const TCHAR* SaveName, const EGameDelegates_SaveGame Key, FString& Value)
{
	static const int32 MAX_SAVEGAME_SIZE = 100 * 1024;
	switch(Key)
	{
		case EGameDelegates_SaveGame::MaxSize:
			Value = FString::Printf(TEXT("%i"), MAX_SAVEGAME_SIZE);
			break;
		case EGameDelegates_SaveGame::Title:
			Value = TEXT("ShooterGame");
			break;
		case EGameDelegates_SaveGame::SubTitle:
			Value = TEXT("The Shootening");
			break;
		case EGameDelegates_SaveGame::Detail:
			Value = TEXT("ShooterGame User Settings");
			break;
		default:
			break;
	}
}

static void ReloadHandler( EPackageReloadPhase ReloadPhase, FPackageReloadedEvent* Event)
{
	if ( ReloadPhase == EPackageReloadPhase::PostPackageFixup)
	{
		// reinitialize allthe material instances


		/*{
			// fixup uniform expressions
			UMaterialInterface::RecacheAllMaterialUniformExpressions();
		}*/

		/*for (TObjectIterator<UMaterialInstance> It; It; ++It)
		{
			UMaterialInstance* Material = *It;
			//Material->InitResources();
			Material->RebuildResource();
		}*/
	}
}

#define EXPERIMENTAL_ENABLEHOTRELOAD 0
static void ReloadPackagesCallback( const TArray<FString>& PackageNames)
{
#if EXPERIMENTAL_ENABLEHOTRELOAD
	TArray<UPackage*> PackagesToReload;
	TArray<UPackage*> MaterialPackagesToReload;
	for (const FString& PackageName : PackageNames)
	{
		UPackage* Package = FindPackage(nullptr, *PackageName);

		if (Package == nullptr)
		{
			// UE_LOG(, Log, TEXT("Unable to find package in memory %s"), *PackageName);
		}
		else
		{
			if ( Package->HasAnyPackageFlags(PKG_ContainsMap || PKG_ContainsMap) )
			{
				continue;
			}
			PackagesToReload.Add(Package);
		}
	}


	// see what's in these packages

	if (PackagesToReload.Num())
	{
		SortPackagesForReload(PackagesToReload);

		TArray<FReloadPackageData> PackagesToReloadData;
		PackagesToReloadData.Empty(PackagesToReload.Num());
		for (UPackage* PackageToReload : PackagesToReload)
		{
			PackagesToReloadData.Emplace(PackageToReload, LOAD_None);
		}

		TArray<UPackage*> ReloadedPackages;

		FDelegateHandle Handle = FCoreUObjectDelegates::OnPackageReloaded.AddStatic(&ReloadHandler);

		FText ErrorMessage;
		GShouldVerifyGCAssumptions = false;
		GUObjectArray.DisableDisregardForGC();

		::ReloadPackages(PackagesToReloadData, ReloadedPackages, 500);

		FCoreUObjectDelegates::OnPackageReloaded.Remove(Handle);
	}
#endif
}

void InitializeShooterGameDelegates()
{
	FGameDelegates::Get().GetWebServerActionDelegate() = FWebServerActionDelegate::CreateStatic(WebServerDelegate);
	FGameDelegates::Get().GetExtendedSaveGameInfoDelegate() = FExtendedSaveGameInfoDelegate::CreateStatic(ExtendedSaveGameInfoDelegate);

	FCoreUObjectDelegates::NetworkFileRequestPackageReload.BindStatic(&ReloadPackagesCallback);
}
