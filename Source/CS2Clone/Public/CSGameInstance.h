// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Justin/ItemTypes.h"
#include "Justin/Framework/MyPlayerState.h"
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

UCLASS()
class CS2CLONE_API UCSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UCSGameInstance();

protected:
	virtual void Init() override;

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
	bool bGameOngoing;
};
