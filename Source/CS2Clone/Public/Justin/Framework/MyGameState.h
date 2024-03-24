// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

/**
 *
 */
 class AMyPlayerState;

UCLASS()
class CS2CLONE_API AMyGameState : public AGameState
{
	GENERATED_BODY()

	friend class ARealGameMode;
protected:
	TArray<AMyPlayerState*> Team_CounterTerrorist; 
	TArray<AMyPlayerState*> Team_Terrorist; 
};
