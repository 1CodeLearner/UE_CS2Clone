// Fill out your copyright notice in the Description page of Project Settings.


#include "RealGameMode.h"
#include "Justin/Framework/MyPlayerController.h"
#include "MyCharacter.h"
#include "kismet/GameplayStatics.h"
#include "Justin/Framework/MyPlayerState.h"
#include "Justin/Framework/MyGameState.h"

ARealGameMode::ARealGameMode()
{
	bDelayedStart = false;
}

void ARealGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ARealGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogTemp, Warning, TEXT("InitGame"));
}

void ARealGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	UE_LOG(LogTemp, Warning, TEXT("MatchStart"));

	auto GS = GetGameState<AMyGameState>();
	if (ensure(GS))
	{
		for (int i = 0; i < GS->PlayerArray.Num(); ++i)
		{
			auto PS = Cast<AMyPlayerState>(GS->PlayerArray[i]);
			if ( ensure(PS) && ensureAlways(PS->TeamType == ETeam::TEAM_MAX))
			{
				if (GS->Team_CounterTerrorist.Num() < GS->Team_Terrorist.Num())
				{
					PS->TeamType = ETeam::TEAM_CT;
					PS->SetTeamMesh();
					GS->Team_CounterTerrorist.Add(PS);
				}
				else
				{
					PS->TeamType = ETeam::TEAM_T;
					PS->SetTeamMesh();					
					GS->Team_Terrorist.Add(PS);
				}
			}
		}
	}
}
