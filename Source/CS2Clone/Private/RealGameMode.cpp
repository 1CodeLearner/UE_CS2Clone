// Fill out your copyright notice in the Description page of Project Settings.


#include "RealGameMode.h"
#include "Justin/Framework/MyPlayerController.h"
#include "MyCharacter.h"
#include "kismet/GameplayStatics.h"
#include "Justin/Framework/MyPlayerState.h"
#include "Justin/Framework/MyGameState.h"

ARealGameMode::ARealGameMode()
{
	
}

void ARealGameMode::PostLogin(APlayerController* NewPlayer)
{	
	Super::PostLogin(NewPlayer);

	auto PS = NewPlayer->GetPlayerState<AMyPlayerState>();
	auto GS = GetGameState<AMyGameState>();

	if (ensure(PS) && ensure(GS) && PS->TeamType == ETeam::TEAM_MAX) 
	{
		if (GS->Team_CounterTerrorist.Num() < GS->Team_Terrorist.Num()) 
		{
			PS->TeamType = ETeam::TEAM_CT;
			GS->Team_CounterTerrorist.Add(PS);
		}
		else 
		{
			PS->TeamType = ETeam::TEAM_T;
			GS->Team_Terrorist.Add(PS);
		}
	}
}

void ARealGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}
