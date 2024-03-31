// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RealGameMode.generated.h"

class UCSGameInstance; 
class AMyCharacter; 
class AMyGameState;

namespace MatchState
{
	extern CS2CLONE_API const FName Cooldown;
	extern CS2CLONE_API const FName GameFinished;
}

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

	virtual void OnMatchStateSet() override;

	virtual void RestartPlayer(AController* NewPlayer) override;
	void SpawnTeamLocation(AActor* StartSpot, AController* NewPlayer);

	virtual void HandleMatchIsWaitingToStart() override; 
	virtual void HandleMatchHasStarted() override;
	virtual void HandleLeavingMap() override;

	//instead of using this function, use WaitingPostMatch MatchState from AGameMode
	UFUNCTION()
	void RestartTest();

private:
	UPROPERTY()
	TObjectPtr<UCSGameInstance> GI;
	UPROPERTY()
	TObjectPtr<AMyGameState> GS;
	void Initialize();
	void PreparePlayers();

	void UpdateTeam();

	//카운트다운 정보 처리
	UFUNCTION()
	void StartTimer();

	void StartCooldown();

	float DestTime;
	float MarkedTime;
	float CountDownTime;
	bool bStart = false;
};
