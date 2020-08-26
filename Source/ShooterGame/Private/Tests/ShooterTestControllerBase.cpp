// Copyright 1998-2019 Epic Games, Inc.All Rights Reserved.
#include "ShooterTestControllerBase.h"
#include "ShooterGame.h"
#include "ShooterGameSession.h"
#include "Online/ShooterOnlineGameSettings.h"
#include "OnlineSubsystemSessionSettings.h"
#include "OnlineSubsystemUtils.h"

void UShooterTestControllerBase::OnInit()
{
	bIsLoggedIn              = false;
	bIsLoggingIn             = false;
	bInQuickMatchSearch      = false;
	bFoundQuickMatchGame     = false;
	bIsSearchingForGame      = false;
	bFoundGame               = false;
	NumOfCycledMatches       = 0;

	if (!FParse::Value(FCommandLine::Get(), TEXT("TargetNumOfCycledMatches"), TargetNumOfCycledMatches))
	{
		TargetNumOfCycledMatches = 2;
	}
}

void UShooterTestControllerBase::OnTick(float TimeDelta)
{
	const FName GameInstanceState = GetGameInstanceState();

	if (GameInstanceState == ShooterGameInstanceState::WelcomeScreen)
	{
		if (!bIsLoggedIn && !bIsLoggingIn)
		{
			bIsLoggingIn = true;
			StartPlayerLoginProcess();
		}
	}
	else if (GameInstanceState == ShooterGameInstanceState::MainMenu)
	{
		if (!bIsLoggedIn && !bIsLoggingIn)
		{
			ULocalPlayer* LP = GetFirstLocalPlayer();

			if (LP)
			{
				bIsLoggingIn = true;
				TSharedPtr<const FUniqueNetId> UserId = LP->GetPreferredUniqueNetId().GetUniqueNetId();
				OnUserCanPlay(*UserId, EUserPrivileges::CanPlay, (uint32)IOnlineIdentity::EPrivilegeResults::NoFailures);
			}
		}
	}
	else if (GameInstanceState == ShooterGameInstanceState::MessageMenu)
	{
		UE_LOG(LogGauntlet, Error, TEXT("Failing due to MessageMenu!"));
		EndTest(-1);
		return;
	}
}

void UShooterTestControllerBase::OnPostMapChange(UWorld* World)
{
	if (IsInGame())
	{
		if (++NumOfCycledMatches >= TargetNumOfCycledMatches)
		{
			EndTest(0);
		}
	}
	else if (NumOfCycledMatches > 0)
	{
		UE_LOG(LogGauntlet, Error, TEXT("Failed to cycle match TargetNumOfCycledMatches(%i)!  NumOfCycledMatches = %i"), TargetNumOfCycledMatches, NumOfCycledMatches);
		EndTest(-1);
	}
}

void UShooterTestControllerBase::StartPlayerLoginProcess()
{
	const IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(GetWorld());

	if (IdentityInterface.IsValid())
	{
		TSharedPtr<GenericApplication> GenericApplication = FSlateApplication::Get().GetPlatformApplication();
		const bool bIsLicensed = GenericApplication->ApplicationLicenseValid();

		const ELoginStatus::Type LoginStatus = IdentityInterface->GetLoginStatus(0);
		if (LoginStatus == ELoginStatus::NotLoggedIn || !bIsLicensed)
		{
			UE_LOG(LogGauntlet, Error, TEXT("Failed!  No player is logged in!"));
			EndTest(-1);
			return;
		}

		TSharedPtr<const FUniqueNetId> UserId = IdentityInterface->GetUniquePlayerId(0);

		if (UserId.IsValid())
		{
			IdentityInterface->GetUserPrivilege(*UserId, EUserPrivileges::CanPlay, IOnlineIdentity::FOnGetUserPrivilegeCompleteDelegate::CreateSP(this, &UShooterTestControllerBase::OnUserCanPlay));
		}
		else
		{
			UE_LOG(LogGauntlet, Error, TEXT("Failed!  Player has invalid UniqueNetId!"));
			EndTest(-1);
		}
	}
}

void UShooterTestControllerBase::OnUserCanPlay(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, uint32 PrivilegeResults)
{
	if (PrivilegeResults == (uint32)IOnlineIdentity::EPrivilegeResults::NoFailures)
	{
		ULocalPlayer* NewPlayerOwner = GetFirstLocalPlayer();

		if (NewPlayerOwner)
		{
			NewPlayerOwner->SetControllerId(0);
			NewPlayerOwner->SetCachedUniqueNetId(NewPlayerOwner->GetUniqueNetIdFromCachedControllerId().GetUniqueNetId());
		}
		else
		{
			UE_LOG(LogGauntlet, Error, TEXT("Failed!  Could not find LocalPlayer in OnUserCanPlay!"));
			EndTest(-1);
		}

#if SHOOTER_CONSOLE_UI
#if LOGIN_REQUIRED_FOR_ONLINE_PLAY
		StartLoginTask();
#else
		StartOnlinePrivilegeTask();
#endif //LOGIN_REQUIRED_FOR_ONLINE_PLAY
#else
		OnUserCanPlayOnline(*NewPlayerOwner->GetCachedUniqueNetId().GetUniqueNetId(), EUserPrivileges::CanPlayOnline, (uint32)IOnlineIdentity::EPrivilegeResults::NoFailures);
#endif //SHOOTER_CONSOLE_UI
	}
	else
	{
		UE_LOG(LogGauntlet, Error, TEXT("Failed!  Player does not have appropiate privileges to play!"));
		EndTest(-1);
	}
}

void UShooterTestControllerBase::StartLoginTask()
{
	const IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(GetWorld());

	if (IdentityInterface.IsValid())
	{
		OnLoginCompleteDelegateHandle = IdentityInterface->AddOnLoginCompleteDelegate_Handle(0, FOnLoginCompleteDelegate::CreateSP(this, &UShooterTestControllerBase::OnLoginTaskComplete));
		IdentityInterface->Login(0, FOnlineAccountCredentials());
	}
	else
	{
		UE_LOG(LogGauntlet, Error, TEXT("Failed!  IdentityInterface is not valid in StartLoginTask!"));
		EndTest(-1);
	}
}

void UShooterTestControllerBase::OnLoginTaskComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	const IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(GetWorld());

	if (IdentityInterface.IsValid())
	{
		IdentityInterface->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, OnLoginCompleteDelegateHandle);

		if (bWasSuccessful)
		{
			StartOnlinePrivilegeTask();
		}
		else
		{
			UE_LOG(LogGauntlet, Error, TEXT("Failed!  Player failed to login!"));
			EndTest(-1);
		}
	}
	else
	{
		UE_LOG(LogGauntlet, Error, TEXT("Failed!  IdentityInterface is not valid in OnLoginTaskComplete!"));
		EndTest(-1);
	}


}

void UShooterTestControllerBase::StartOnlinePrivilegeTask()
{
	const ULocalPlayer* PlayerOwner            = GetFirstLocalPlayer();
	const IOnlineIdentityPtr IdentityInterface = Online::GetIdentityInterface(GetWorld());

	if (PlayerOwner && IdentityInterface.IsValid())
	{
		IdentityInterface->GetUserPrivilege(*PlayerOwner->GetCachedUniqueNetId().GetUniqueNetId(), EUserPrivileges::CanPlayOnline,
			IOnlineIdentity::FOnGetUserPrivilegeCompleteDelegate::CreateSP(this, &UShooterTestControllerBase::OnUserCanPlayOnline));
	}
	else
	{
		UE_LOG(LogGauntlet, Error, TEXT("Failed!  Could not find LocalPlayer or IdentityInterface is null in OnlinePrivilegeTask!"));
		EndTest(-1);
	}
}

void UShooterTestControllerBase::OnUserCanPlayOnline(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, uint32 PrivilegeResults)
{
	if (PrivilegeResults == (uint32)IOnlineIdentity::EPrivilegeResults::NoFailures)
	{
		bIsLoggingIn = false;
		bIsLoggedIn = true;

		if (UShooterGameInstance* GameInstance = GetGameInstance())
		{
			GameInstance->SetOnlineMode(EOnlineMode::Online);
		}
	}
	else
	{
		UE_LOG(LogGauntlet, Error, TEXT("Failed!  Player does not have appropiate privileges to play online!"));
		EndTest(-1);
	}
}

void UShooterTestControllerBase::HostGame()
{
	UShooterGameInstance* GameInstance = GetGameInstance();
	ULocalPlayer* PlayerOwner          = GameInstance ? GameInstance->GetFirstGamePlayer() : nullptr;

	if (PlayerOwner)
	{
		const FString GameType = TEXT("TDM");
		const FString StartURL = FString::Printf(TEXT("/Game/Maps/%s?game=%s%s"), TEXT("Sanctuary"), *GameType, TEXT("?listen"));

		GameInstance->HostGame(PlayerOwner, GameType, StartURL);
	}
	else
	{
		UE_LOG(LogGauntlet, Error, TEXT("Failed!  Could not find LocalPlayer or GameInstance is null!"));
		EndTest(-1);
	}
}

void UShooterTestControllerBase::StartQuickMatch()
{
	IOnlineSessionPtr Sessions = Online::GetSessionInterface(GetWorld());
	UShooterGameInstance* GameInstance = GetGameInstance();
	if (!Sessions.IsValid() || GameInstance == nullptr)
	{
		UE_LOG(LogGauntlet, Error, TEXT("Failed!  Could not find online session interface or GameInstance is null!"));
		EndTest(-1);
		return;
	}

	QuickMatchSearchSettings = MakeShareable(new FShooterOnlineSearchSettings(false, true));
	QuickMatchSearchSettings->QuerySettings.Set(SEARCH_XBOX_LIVE_HOPPER_NAME, FString("TeamDeathmatch"), EOnlineComparisonOp::Equals);
	QuickMatchSearchSettings->QuerySettings.Set(SEARCH_XBOX_LIVE_SESSION_TEMPLATE_NAME, FString("MatchSession"), EOnlineComparisonOp::Equals);
	QuickMatchSearchSettings->TimeoutInSeconds = 120.0f;

	FShooterOnlineSessionSettings SessionSettings(false, true, 8);
	SessionSettings.Set(SETTING_GAMEMODE, FString("TDM"), EOnlineDataAdvertisementType::ViaOnlineService);
	SessionSettings.Set(SETTING_MATCHING_HOPPER, FString("TeamDeathmatch"), EOnlineDataAdvertisementType::DontAdvertise);
	SessionSettings.Set(SETTING_MATCHING_TIMEOUT, 120.0f, EOnlineDataAdvertisementType::ViaOnlineService);
	SessionSettings.Set(SETTING_SESSION_TEMPLATE_NAME, FString("GameSession"), EOnlineDataAdvertisementType::DontAdvertise);

	TSharedRef<FOnlineSessionSearch> QuickMatchSearchSettingsRef = QuickMatchSearchSettings.ToSharedRef();

	Sessions->ClearOnMatchmakingCompleteDelegate_Handle(OnMatchmakingCompleteDelegateHandle);
	OnMatchmakingCompleteDelegateHandle = Sessions->AddOnMatchmakingCompleteDelegate_Handle(FOnMatchmakingCompleteDelegate::CreateSP(this, &UShooterTestControllerBase::OnMatchmakingComplete));

	// Perform matchmaking with all local players
	TArray<TSharedRef<const FUniqueNetId>> LocalPlayerIds;
	for (int32 i = 0; i < GameInstance->GetNumLocalPlayers(); ++i)
	{
		FUniqueNetIdRepl PlayerId = GameInstance->GetLocalPlayerByIndex(i)->GetPreferredUniqueNetId();
		if (PlayerId.IsValid())
		{
			LocalPlayerIds.Add((*PlayerId).AsShared());
		}
	}

	bInQuickMatchSearch = true;

	if (!Sessions->StartMatchmaking(LocalPlayerIds, NAME_GameSession, SessionSettings, QuickMatchSearchSettingsRef))
	{
		OnMatchmakingComplete(NAME_GameSession, false);
	}
}

void UShooterTestControllerBase::OnMatchmakingComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogGauntlet, Error, TEXT("Failed!  Could not find online session interface!"));
		EndTest(-1);
		return;
	}

	bInQuickMatchSearch = false;
	SessionInterface->ClearOnMatchmakingCompleteDelegate_Handle(OnMatchmakingCompleteDelegateHandle);

	if (!bWasSuccessful)
	{
		UE_LOG(LogGauntlet, Warning, TEXT("Matchmaking was unsuccessful."));
		return;
	}

	UE_LOG(LogGauntlet, Log, TEXT("Matchmaking successful! Session name is %s."), *SessionName.ToString());

	FNamedOnlineSession* MatchmadeSession = SessionInterface->GetNamedSession(SessionName);

	if (!MatchmadeSession)
	{
		UE_LOG(LogGauntlet, Warning, TEXT("OnMatchmakingComplete: No session."));
		return;
	}

	if (!MatchmadeSession->OwningUserId.IsValid())
	{
		UE_LOG(LogGauntlet, Warning, TEXT("OnMatchmakingComplete: No session owner/host."));
		return;
	}

	UE_LOG(LogGauntlet, Log, TEXT("OnMatchmakingComplete: Session host is %d."), *MatchmadeSession->OwningUserId->ToString());

	if (UShooterGameInstance* GameInstance = GetGameInstance())
	{
		IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());

		// We only care about hosted games 
		if (Subsystem && Subsystem->IsLocalPlayer(*MatchmadeSession->OwningUserId))
		{
			SessionInterface->EndSession(SessionName);
		}
		else
		{
			bFoundQuickMatchGame = true;
			GameInstance->TravelToSession(SessionName);
		}
	}
}

void UShooterTestControllerBase::StartSearchingForGame()
{
	if (UShooterGameInstance* GameInstance = GetGameInstance())
	{
		bIsSearchingForGame = true;
		GameInstance->FindSessions(GameInstance->GetFirstGamePlayer(), false, false);
	}
}

void UShooterTestControllerBase::UpdateSearchStatus()
{
	AShooterGameSession* ShooterSession = GetGameSession();

	if (ShooterSession)
	{
		int32 CurrentSearchIdx, NumSearchResults;
		EOnlineAsyncTaskState::Type SearchState = ShooterSession->GetSearchResultStatus(CurrentSearchIdx, NumSearchResults);

		UE_LOG(LogGauntlet, Log, TEXT("ShooterSession->GetSearchResultStatus: %s"), EOnlineAsyncTaskState::ToString(SearchState));

		switch (SearchState)
		{
		case EOnlineAsyncTaskState::InProgress:
		{
			break;
		}
		case EOnlineAsyncTaskState::Done:
		{
			const TArray<FOnlineSessionSearchResult> & SearchResults = ShooterSession->GetSearchResults();
			check(SearchResults.Num() == NumSearchResults);

			if (NumSearchResults > 0)
			{
				bFoundGame = true;

				UShooterGameInstance* GameInstance = GetGameInstance();
				ULocalPlayer* PlayerOwner          = GameInstance ? GameInstance->GetFirstGamePlayer() : nullptr;

				if (PlayerOwner)
				{
					// Join First Result
					GameInstance->JoinSession(PlayerOwner, 0);
				}
			}

			bIsSearchingForGame = false;

			break;
		}

		case EOnlineAsyncTaskState::Failed:
		case EOnlineAsyncTaskState::NotStarted:
		default:
		{
			bIsSearchingForGame = false;
			break;
		}
		}
	}
}

UShooterGameInstance* UShooterTestControllerBase::GetGameInstance() const 
{
	if (const UWorld* World = GetWorld())
	{
		return Cast<UShooterGameInstance>(GetWorld()->GetGameInstance());
	}

	return nullptr;
}

const FName UShooterTestControllerBase::GetGameInstanceState() const 
{
	if (const UShooterGameInstance* GameInstance = GetGameInstance())
	{
		return GameInstance->GetCurrentState();
	}

	return "";
}

AShooterGameSession* UShooterTestControllerBase::GetGameSession() const
{
	if (const UShooterGameInstance* GameInstance = GetGameInstance())
	{
		return GameInstance->GetGameSession();
	}

	return nullptr;
}

bool UShooterTestControllerBase::IsInGame() const
{
	return GetGameInstanceState() == ShooterGameInstanceState::Playing;
}

ULocalPlayer* UShooterTestControllerBase::GetFirstLocalPlayer() const
{
	if (const UShooterGameInstance* GameInstance = GetGameInstance())
	{
		return GameInstance->GetFirstGamePlayer();
	}

	return nullptr;
}