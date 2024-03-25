// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RealGameMode.generated.h"

class UCSGameInstance; 
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
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void HandleMatchIsWaitingToStart() override; 
	virtual void HandleMatchHasStarted() override;
	virtual void HandleLeavingMap() override;

	UFUNCTION()
	void RestartTest();
private:
	/*UPROPERTY()
	TObjectPtr<UCSGameInstance> GameInstance;*/
	void AssignTeam();

	//카운트다운 정보 처리
	UFUNCTION()
	void StartTimer();

	float DestTime;
	float MarkedTime;
	float CountDownTime;
	bool bStart = false;
};
