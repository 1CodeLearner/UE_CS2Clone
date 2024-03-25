// Fill out your copyright notice in the Description page of Project Settings.


#include "RealGameMode.h"
#include "Justin/Framework/MyPlayerController.h"
#include "MyCharacter.h"
#include "kismet/GameplayStatics.h"
#include "Justin/Framework/MyPlayerState.h"
#include "Justin/Framework/MyGameState.h"
#include "CSGameInstance.h"

ARealGameMode::ARealGameMode()
{
	bDelayedStart = true;
	CountDownTime = 15.f;
	DestTime = 10.f;
	MarkedTime = 0.f;
	bStart = false;
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
				UE_LOG(LogTemp, Warning, TEXT("Timer ended"));
				StartMatch();
			}
		}
		else {
			//GameplayDisplay->SetTime(remainingTime);
			MarkedTime += DeltaSeconds;
		}
	}
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
	UE_LOG(LogTemp, Warning, TEXT("InitGame"));

	/*
	* if game did not start - mark game as started.
	* if game did start - skip delay start and start game immediately
	*/
	GameInstance = GetGameInstance<UCSGameInstance>();
	if (GameInstance && !GameInstance->GetGameStarted())
	{
		GameInstance->SetGameStarted(true);
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

	UE_LOG(LogTemp, Warning, TEXT("MatchStart"));

	AssignTeam();

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ARealGameMode::RestartTest, 5.f, false);
}

void ARealGameMode::HandleLeavingMap()
{
	Super::HandleLeavingMap();

	UE_LOG(LogTemp, Warning, TEXT("Leaving map....`"));
}

void ARealGameMode::RestartTest()
{
	RestartGame();
}

void ARealGameMode::AssignTeam()
{
	auto GS = GetGameState<AMyGameState>();
	if (ensure(GS))
	{
		for (int i = 0; i < GS->PlayerArray.Num(); ++i)
		{
			auto PS = Cast<AMyPlayerState>(GS->PlayerArray[i]);
			UE_LOG(LogTemp, Warning, TEXT("[%d]: PlayerId: %s"), i, *PS->GetUniqueId().ToString());
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
		}
	}
}

void ARealGameMode::StartTimer()
{
	MarkedTime = GetWorld()->GetTimeSeconds();
	DestTime = MarkedTime + CountDownTime;
	bStart = true;
}
