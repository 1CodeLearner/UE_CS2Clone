// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RealGameMode.generated.h"

class UCSGameInstance; 
class AMyCharacter; 
class AMyGameState;
/**
 *
 */
UCLASS()
class CS2CLONE_API ARealGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ARealGameMode();

	void OnPlayerDead(AMyCharacter* character);

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void BeginPlay() override;

	virtual void HandleMatchIsWaitingToStart() override; 
	virtual void HandleMatchHasStarted() override;
	virtual void HandleLeavingMap() override;

	//instead of using this function, use WaitingPostMatch MatchState from AGameMode
	UFUNCTION()
	void RestartTest();
private:
	UPROPERTY()
	TObjectPtr<AMyGameState> GS;
	void AssignTeam();
	void UpdateTeam();

	//ī��Ʈ�ٿ� ���� ó��
	UFUNCTION()
	void StartTimer();

	float DestTime;
	float MarkedTime;
	float CountDownTime;
	bool bStart = false;
};
