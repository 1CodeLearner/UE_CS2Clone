// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RealGameMode.generated.h"

/**
 *
 */
UCLASS()
class CS2CLONE_API ARealGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ARealGameMode();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void HandleMatchHasStarted() override;
	virtual void HandleLeavingMap() override;

	UFUNCTION()
	void RestartTest();
};
