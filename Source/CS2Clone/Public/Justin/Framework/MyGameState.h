// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyCharacter.h"
#include "MyGameState.generated.h"

/**
 *
 */
class AMyPlayerState;
class ARealGameMode;
enum class ETeam : uint8;

UCLASS()
class CS2CLONE_API AMyGameState : public AGameState
{
	GENERATED_BODY()

	friend class ARealGameMode;
public:
	AMyGameState();

	UPROPERTY(Replicated)
	ETeam EWinner;
protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > &OutLifetimeProps) const override;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_ScoreUpdate)
	int32 CT_Score;
	UPROPERTY(ReplicatedUsing = OnRep_ScoreUpdate)
	int32 T_Score;

	UFUNCTION()
	void OnRep_ScoreUpdate();

	TArray<AMyPlayerState*> Team_CounterTerrorist;
	TArray<AMyPlayerState*> Team_Terrorist;
			
	UPROPERTY(Replicated)
	ETeam ETeamWon;

private:
	bool IsTeamEliminated(const TArray<AMyPlayerState*>& Team);
	void OnPlayerDead(AMyCharacter* character);
};
