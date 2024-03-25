// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Justin/ItemTypes.h"
#include "CSGameInstance.generated.h"

/**
 *
 */
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

	void SetGameStarted(bool bStarted);
	bool GetGameStarted() const;
private:
	bool bGameStarted;
};
