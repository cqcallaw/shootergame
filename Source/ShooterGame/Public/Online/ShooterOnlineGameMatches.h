// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Interfaces/OnlineGameMatchesInterface.h"
#include "Online/ShooterGameState.h"
#include "ShooterGameInstance.h"

class AShooterGameState;

class SHOOTERGAME_API FShooterOnlineGameMatches
{
public:
	FShooterOnlineGameMatches()
	{
	}

	~FShooterOnlineGameMatches()
	{
	}
	void Initialize(AShooterGameState* InGameState, UShooterGameInstance* InGameInstance);

	void CreateMatch(const FUniqueNetId& LocalOwnerId, const FString& ActivitId, const TArray<FGameMatchPlayer>& Players, const TArray<FGameMatchTeam>& Teams);
	void StartMatch(const FUniqueNetId& LocalUserId, const FString& MatchId);
	void EndMatch(const FUniqueNetId& LocalOwnerId, const FString& MatchId, const FFinalGameMatchReport& FinalReport);
	void LeaveGameMatchFeedback(const FUniqueNetId& LocalUserId, const FString& MatchId, const bool bRequestReview);

	void HandleMatchHasStarted(const FString& ActivityId, int32& NumTeams);
	void HandleMatchHasEnded(const bool bEnableGameFeedback, const int32 NumTeams, TArrayView<int32> TeamScores);

protected:
	TSharedPtr<const FUniqueNetId> GetMatchOwnerId();
	bool GetMatchOwnerAndId(TSharedPtr<const FUniqueNetId>& LocalOwnerId, FString& MatchId);
	bool GetMatchId(FString& MatchId);

	void BuildTeamGameMatchResults(const TArrayView<int32> TeamScores, const int32& NumTeams, TArray<FGameMatchTeamResult>& TeamResults);
	void BuildPlayerGameMatchResults(const TSharedRef<const FUniqueNetId> PlayerNetId, const int32 TeamId, FGameMatchPlayerResult PlayerResult);
	void BuildTeamPlayerGameMatchStats(const TSharedRef<const FUniqueNetId> PlayerNetId, const int32 TeamId, const FString& StatKey, const FString& StatValue);
	void BuildTeamGameMatchStats(TArray<FGameMatchTeamStats>& TeamStats);

protected:
	void OnCreateMatchComplete(const FUniqueNetId& LocalUserId, const FString& MatchId, const FOnlineError& Result);
	void OnEndMatchComplete(const FUniqueNetId& LocalUserId, const FOnlineError& Result, FString MatchId, bool bRequestReview);
	void OnMatchStatusUpdateComplete(const FUniqueNetId& LocalUserId, const EUpdateGameMatchStatus& Status, const FOnlineError& Result);
	void OnLeaveGameMatchFeedbackComplete(const FUniqueNetId& LocalUserId, const FOnlineError& Result);

	bool IsClientRepresentative(const TSharedPtr<const FUniqueNetId>& RepresentativeId);
	bool GetNamedSessionInfo(FNamedOnlineSession*& NamedSession);

protected:
	// TeamID, Players result.
	TMap<int32, TArray<FGameMatchPlayerResult>> TeamPlayerResultsMap;
	// Map for creating stats for teams
	TMap<int32, TArray<FGameMatchPlayerStats>> TeamToPlayerStatsMap;

	// Player to index mapping for PlayerNetIds
	TMap<FString, int32> PlayerIdToNetIdIndexMap;
	// Holds player's net ids
	TArray<TSharedRef<const FUniqueNetId>> PlayerNetIds;

	AShooterGameState* GameState = nullptr;
	UShooterGameInstance* GameInstance = nullptr;
};