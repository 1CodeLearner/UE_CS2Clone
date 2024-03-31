// Fill out your copyright notice in the Description page of Project Settings.


#include "RealGameMode.h"
#include "Justin/Framework/MyPlayerController.h"
#include "MyCharacter.h"
#include "kismet/GameplayStatics.h"
#include "Justin/Framework/MyPlayerState.h"
#include "Justin/Framework/MyGameState.h"
#include "CSGameInstance.h"
#include "Justin/Framework/MyGameState.h"
#include "EngineUtils.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

ARealGameMode::ARealGameMode()
{
	bDelayedStart = true;
	CountDownTime = 6.f;
	DestTime = 10.f;
	MarkedTime = 0.f;
	bStart = false;
}

void ARealGameMode::OnPlayerDead(AMyCharacter* character)
{
	if (GS)
	{
		GS->OnPlayerDead(character);
	}

	bool isMatchOver = false;

	if (GS->ETeamWon == ETeam::TEAM_CT) {
		UE_LOG(LogTemp, Warning, TEXT("CounterTerrorism wins by %d"), GS->CT_Score);
		GI->teamInfoMap[ETeam::TEAM_CT].IncrementScore();
		isMatchOver = true;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Terrorist wins by %d"), GS->T_Score);
		GI->teamInfoMap[ETeam::TEAM_T].IncrementScore();
		isMatchOver = true;
	}

	if (isMatchOver) {		
		//GetWorld()->ServerTravel("'/Game/Level_Gameplay.Level_Gameplay'",GetTravelType()); // doesn't load clients property
		//RestartTest();
		SetMatchState(MatchState::Cooldown);
		/*FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ARealGameMode::RestartTest, 5.f, false);*/
	}
	//instead of using this function, use WaitingPostMatch MatchState from AGameMode

}

void ARealGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bStart) //&& GameplayDisplay)
	{
		float remainingTime = DestTime - (MarkedTime + DeltaSeconds);
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("Time: %f"), remainingTime));
		if (remainingTime <= 0.f)
		{
			bStart = false;
			//GameplayDisplay->SetTime(0.f);

			if (HasAuthority())
			{
				StartMatch();
			}
		}
		else {
			//GameplayDisplay->SetTime(remainingTime);
			MarkedTime += DeltaSeconds;
		}
	}
}

void ARealGameMode::StartTimer()
{
	MarkedTime = GetWorld()->GetTimeSeconds();
	DestTime = MarkedTime + CountDownTime;
	bStart = true;
}

void ARealGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	//When new player joins, reset countdown to game start.
	StartTimer();
}

void ARealGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GI = GetGameInstance<UCSGameInstance>();

	/*
	* if game did not start - mark game as started.
	* if game did start - skip delay start and start game immediately
	*/
}

void ARealGameMode::BeginPlay()
{
	Super::BeginPlay();

	GS = Cast<AMyGameState>(GameState);
}

void ARealGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	if(MatchState == MatchState::Cooldown){
		for(auto i : TActorRange<AMyPlayerController>(GetWorld())){
			if(i){
				i->SetMatchState(MatchState);
			}
		}
	}
}

void ARealGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();

	StartTimer();
}

void ARealGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	if (GI && GI->IsGameOnGoing())
	{
		UpdateTeam();
	}
	else
	{
		GI->SetGameOnGoing(true);
		AssignTeam();
	}
}

void ARealGameMode::HandleLeavingMap()
{
	Super::HandleLeavingMap();
}

void ARealGameMode::RestartTest()
{
	RestartGame();
}

void ARealGameMode::AssignTeam()
{
	if (ensure(GS))
	{
		for (int i = 0; i < GS->PlayerArray.Num(); ++i)
		{
			auto PS = Cast<AMyPlayerState>(GS->PlayerArray[i]);

			//assign team to player
			if (ensure(PS) && ensureAlways(PS->TeamType == ETeam::TEAM_MAX))
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

			FString PlayerId = PS->GetUniqueId().ToString();
			UE_LOG(LogTemp, Warning, TEXT("[%s]: team: %s"),
				*PlayerId, *UEnum::GetValueAsString(PS->TeamType));

			//add Player info on Game instance
			uint32 rand = FMath::RandRange(0, 10);
			if (ensure(GI)) {
				GI->playerInfoMap.Add(PlayerId, FPlayerInfo(PS->TeamType));

				GI->teamInfoMap.Add(ETeam::TEAM_CT, FTeamInfo());
				GI->teamInfoMap.Add(ETeam::TEAM_T, FTeamInfo());
			}
		}
	}
}

void ARealGameMode::UpdateTeam()
{
	if (ensure(GS))
	{
		for (int i = 0; i < GS->PlayerArray.Num(); ++i)
		{
			auto PS = Cast<AMyPlayerState>(GS->PlayerArray[i]);
			FString PlayerId = PS->GetUniqueId().ToString();

			ETeam team = GI->playerInfoMap[PlayerId].TeamType;
			PS->TeamType = team;
			PS->SetTeamMesh();

			if (PS->TeamType == ETeam::TEAM_CT)
				GS->Team_CounterTerrorist.Add(PS);
			else
				GS->Team_Terrorist.Add(PS);

			UE_LOG(LogTemp, Warning, TEXT("[%s]: team: %s"),
				*PlayerId, *UEnum::GetValueAsString(PS->TeamType));
		}

		GS->CT_Score = GI->teamInfoMap[ETeam::TEAM_CT].GetScore();
		GS->T_Score = GI->teamInfoMap[ETeam::TEAM_T].GetScore();
		UE_LOG(LogTemp, Warning, TEXT("UpdateTeam() CT: %d, T: %d"), GS->CT_Score, GS->T_Score);

	}
}

void ARealGameMode::StartCooldown()
{
	SetMatchState(MatchState::Cooldown);
}
