// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Framework/MyPlayerController.h"
#include "Justin/UI/GameplayDisplay.h"
#include "RealGameMode.h"
#include "Justin/Framework/MyPlayerState.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Justin/Framework/MyGameState.h"

AMyPlayerController::AMyPlayerController()
{
	GameplayDisplayClass = nullptr;
	GameplayDisplay = nullptr;
	DeltaTime = 0.f;
}

void AMyPlayerController::DisplayResult()
{
	if (!GameplayDisplay && ensure(GameplayDisplayClass))
	{
		GameplayDisplay = CreateWidget<UGameplayDisplay>(GetWorld(), GameplayDisplayClass);
	}
	if (GameplayDisplay)
	{
		GameplayDisplay->AddToViewport();
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//if (IsLocalController())
	//{
	//	DisplayGameplay();
	//}
	if (!HasAuthority())
	{
		SendServerTimeRequest();
		FTimerHandle Handle;
		//every 5 seconds send server time request to sync with server time
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &AMyPlayerController::SendServerTimeRequest, 5.f, true);
	}
}

void AMyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerController, MatchState);
}

void AMyPlayerController::SendServerTimeRequest()
{
	Server_RequestServerTime(GetWorld()->GetTimeSeconds());
}

float AMyPlayerController::GetServerTime() const
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	else
	{
		return DeltaTime + GetWorld()->GetTimeSeconds();
	}
}

void AMyPlayerController::Server_RequestServerTime_Implementation(float SentClientTime)
{
	Client_RespondServerTime(SentClientTime, GetWorld()->GetTimeSeconds());
}

void AMyPlayerController::Client_RespondServerTime_Implementation(float SentClientTime, float CurrentServerTime)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - SentClientTime;
	float EstimatedServerTime = CurrentServerTime + RoundTripTime / 2.f;

	DeltaTime = EstimatedServerTime - GetWorld()->GetTimeSeconds();
	UE_LOG(LogTemp, Warning, TEXT("Current Server time: %f"), GetServerTime());
}

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	auto PS = Cast<AMyPlayerState>(PlayerState);
	if (ensure(PS))
	{
		PS->SetCharacter(Cast<ACharacter>(aPawn));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No PlayerState?"));
	}
}

void AMyPlayerController::SetMatchState(FName CurrMatchState)
{
	MatchState = FMatchState(MatchState.Winner, CurrMatchState);

	if (IsLocalController())
		OnRep_OnMatchStateChanged();
}

void AMyPlayerController::OnRep_OnMatchStateChanged()
{
	if (MatchState.CurrentState == MatchState::Cooldown) {
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		UE_LOG(LogTemp, Warning, TEXT("[%s]: MatchState: %s"), *GetNameSafe(this), *MatchState::Cooldown.ToString());
	}
	else if (MatchState.CurrentState == MatchState::GameFinished) {
		UE_LOG(LogTemp, Warning, TEXT("[%s]: MatchState: %s"), *GetNameSafe(this), *MatchState::GameFinished.ToString());

		if (HasAuthority()) {
			auto GS = GetWorld()->GetGameState<AMyGameState>();
			FString string;
			if (GS) {
				switch (GS->EWinner) {
				case ETeam::TEAM_CT:
					string = FString::Printf(TEXT("CounterTerrorists Wins"));
					break;
				case ETeam::TEAM_T:
					string = FString::Printf(TEXT("Terrorists Wins"));
					break;
				}
			}
			DisplayResult();

			FText text = FText::FromString(string);
			GameplayDisplay->SetWinner(text);
		}
		else{
			Server_RequestWinner();
		}
	}
	else
		GameplayDisplay->RemoveFromParent();



}

void AMyPlayerController::Server_RequestWinner_Implementation()
{
	auto GS = GetWorld()->GetGameState<AMyGameState>();

	FString string;
	if (GS) {
		switch (GS->EWinner) {
		case ETeam::TEAM_CT:
			string = FString::Printf(TEXT("CounterTerrorists Wins"));
			break;
		case ETeam::TEAM_T:
			string = FString::Printf(TEXT("Terrorists Wins"));
			break;
		}
	}

	Client_RespondWinner(string);
}

void AMyPlayerController::Client_RespondWinner_Implementation(const FString& teamWon)
{
	DisplayResult();

	FText text = FText::FromString(teamWon);
	GameplayDisplay->SetWinner(text);

}