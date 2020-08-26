// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ShooterGame.h"
#include "ShooterRecentlyMet.h"
#include "ShooterTypes.h"
#include "ShooterStyle.h"
#include "ShooterOptionsWidgetStyle.h"
#include "ShooterGameUserSettings.h"
#include "ShooterPersistentUser.h"
#include "Player/ShooterLocalPlayer.h"
#include "OnlineSubsystemUtils.h"

#define LOCTEXT_NAMESPACE "ShooterGame.HUD.Menu"

void FShooterRecentlyMet::Construct(ULocalPlayer* _PlayerOwner, int32 LocalUserNum_)
{
	RecentlyMetStyle = &FShooterStyle::Get().GetWidgetStyle<FShooterOptionsStyle>("DefaultShooterOptionsStyle");

	PlayerOwner = _PlayerOwner;
	LocalUserNum = LocalUserNum_;
	CurrRecentlyMetIndex = 0;
	MinRecentlyMetIndex = 0;
	MaxRecentlyMetIndex = 0; //initialized after the recently met list is (read in/set)

	/** Recently Met menu items */
	RecentlyMetRoot = FShooterMenuItem::CreateRoot();
	RecentlyMetItem = MenuHelper::AddMenuItem(RecentlyMetRoot, LOCTEXT("Recently Met", "RECENTLY MET"));

	/** Init online items */
	if (PlayerOwner)
	{
		OnlineSub = Online::GetSubsystem(PlayerOwner->GetWorld());
	}

	UserSettings = CastChecked<UShooterGameUserSettings>(GEngine->GetGameUserSettings());	
}

void FShooterRecentlyMet::OnApplySettings()
{
	ApplySettings();
}

void FShooterRecentlyMet::ApplySettings()
{
	UShooterPersistentUser* PersistentUser = GetPersistentUser();
	if(PersistentUser)
	{
		PersistentUser->TellInputAboutKeybindings();

		PersistentUser->SaveIfDirty();
	}

	UserSettings->ApplySettings(false);

	OnApplyChanges.ExecuteIfBound();
}

void FShooterRecentlyMet::TellInputAboutKeybindings()
{
	UShooterPersistentUser* PersistentUser = GetPersistentUser();
	if(PersistentUser)
	{
		PersistentUser->TellInputAboutKeybindings();
	}
}

UShooterPersistentUser* FShooterRecentlyMet::GetPersistentUser() const
{
	UShooterLocalPlayer* const SLP = Cast<UShooterLocalPlayer>(PlayerOwner);
	return SLP ? SLP->GetPersistentUser() : nullptr;
}

void FShooterRecentlyMet::UpdateRecentlyMet(int32 NewOwnerIndex)
{
	LocalUserNum = NewOwnerIndex;
	
	if (OnlineSub)
	{
		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
		if (Identity.IsValid())
		{
			LocalUsername = Identity->GetPlayerNickname(LocalUserNum);
		}
	}
	
	MenuHelper::ClearSubMenu(RecentlyMetItem);
	MaxRecentlyMetIndex = 0;

	AShooterGameState* const MyGameState = PlayerOwner->GetWorld()->GetGameState<AShooterGameState>();
	if (MyGameState != nullptr)
	{
		MetPlayerArray = MyGameState->PlayerArray;

		for (int32 i = 0; i < MetPlayerArray.Num(); ++i)
		{
			const APlayerState* PlayerState = MetPlayerArray[i];
			FString Username = PlayerState->GetHumanReadableName();
			if (Username != LocalUsername && PlayerState->bIsABot == false)
			{
				TSharedPtr<FShooterMenuItem> UserItem = MenuHelper::AddMenuItem(RecentlyMetItem, FText::FromString(Username));
				UserItem->OnControllerDownInputPressed.BindRaw(this, &FShooterRecentlyMet::IncrementRecentlyMetCounter);
				UserItem->OnControllerUpInputPressed.BindRaw(this, &FShooterRecentlyMet::DecrementRecentlyMetCounter);
				UserItem->OnControllerFacebuttonDownPressed.BindRaw(this, &FShooterRecentlyMet::ViewSelectedUsersProfile);
			}
			else
			{
				MetPlayerArray.RemoveAt(i);
				--i; //we just deleted an item, so we need to go make sure i doesn't increment again, otherwise it would skip the player that was supposed to be looked at next
			}
		}

		MaxRecentlyMetIndex = MetPlayerArray.Num() - 1;
	}

	MenuHelper::AddMenuItemSP(RecentlyMetItem, LOCTEXT("Close", "CLOSE"), this, &FShooterRecentlyMet::OnApplySettings);
}

void FShooterRecentlyMet::IncrementRecentlyMetCounter()
{
	if (CurrRecentlyMetIndex + 1 <= MaxRecentlyMetIndex)
	{
		++CurrRecentlyMetIndex;
	}
}
void FShooterRecentlyMet::DecrementRecentlyMetCounter()
{
	if (CurrRecentlyMetIndex - 1 >= MinRecentlyMetIndex)
	{
		--CurrRecentlyMetIndex;
	}
}
void FShooterRecentlyMet::ViewSelectedUsersProfile()
{
	if (OnlineSub)
	{
		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
		if (Identity.IsValid() && MetPlayerArray.IsValidIndex(CurrRecentlyMetIndex))
		{
			const APlayerState* PlayerState = MetPlayerArray[CurrRecentlyMetIndex];
		
			TSharedPtr<const FUniqueNetId> Requestor = Identity->GetUniquePlayerId(LocalUserNum);
			TSharedPtr<const FUniqueNetId> Requestee = PlayerState->UniqueId.GetUniqueNetId();
			
			IOnlineExternalUIPtr ExternalUI = OnlineSub->GetExternalUIInterface();
			if (ExternalUI.IsValid() && Requestor.IsValid() && Requestee.IsValid())
			{
				ExternalUI->ShowProfileUI(*Requestor, *Requestee, FOnProfileUIClosedDelegate());
			}
		}
	}
}


#undef LOCTEXT_NAMESPACE
