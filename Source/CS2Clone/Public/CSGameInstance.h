// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Justin/ItemTypes.h"
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
		testing = 0;
	};
	FPlayerInfo(uint32 test)
	{
		testing = test;
	}

	UPROPERTY()
	uint32 testing; 
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
	TMap<FString,FPlayerInfo> playerInfoMap;

	void SetGameOnGoing(bool bStarted);
	bool IsGameOnGoing() const;
private:
	bool bGameOngoing;
};
