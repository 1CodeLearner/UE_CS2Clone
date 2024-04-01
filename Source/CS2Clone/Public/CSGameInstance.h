// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Justin/ItemTypes.h"
#include "Justin/Framework/MyPlayerState.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CSGameInstance.generated.h"

/**
 *
 */

USTRUCT()
struct FPlayerInfo
{
	GENERATED_BODY()
public:
	FPlayerInfo()
	{
		TeamType = ETeam::TEAM_MAX;
	};
	FPlayerInfo(ETeam team)
	{
		TeamType = team;
	}

	UPROPERTY()
	ETeam TeamType;
};

USTRUCT()
struct FTeamInfo {

	GENERATED_BODY()
public:
	void IncrementScore()
	{
		Score++;
	}
	int32 GetScore() const
	{
		return Score;
	}
private:
	UPROPERTY()
	int32 Score = 0;
};

DECLARE_DELEGATE_TwoParams(FSearchComplete, int32, FString);

UCLASS()
class CS2CLONE_API UCSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UCSGameInstance();
public:
	// 세션 만드는 함수
	UFUNCTION(BlueprintCallable)
	void CreateMySession(FString roomName, int32 maxPlayer);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	// 세션 파괴하는 함수
	UFUNCTION(BlueprintCallable)
	void DestroyMySession();
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	// 세션을 검색 함수
	UFUNCTION(BlueprintCallable)
	void FindOtherSession();
	void OnFindSessionComplete(bool bWasSuccessful);

	// 세션 참여 함수
	UFUNCTION(BlueprintCallable)
	void JoinOtherSession(int32 idx);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result);


	FString StringBase64Encode(FString str);
	FString StringBase64Decode(FString str);
public:
	// 이 변수통해 (세션 만들고, 세션 검색, 세션 참여)
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> sessionInterface;

	// 세션 검색에 쓰이는 클래스
	TSharedPtr<class FOnlineSessionSearch> sessionSearch;

	// 세션 이름
	FString mySessionName = TEXT("lokimve7");

	// 세션 검색이 완료되면 호출해야 하는 Delegate
	FSearchComplete onSearchComplete;
protected:
	virtual void Init() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<struct FItemType> defineItem;

	UPROPERTY()
	TMap<FString, FPlayerInfo> playerInfoMap;

	UPROPERTY()
	TMap<ETeam, FTeamInfo> teamInfoMap;

	void SetGameOnGoing(bool bStarted);
	bool IsGameOnGoing() const;
private:
	UPROPERTY(Replicated)
	bool bGameOngoing;
};
