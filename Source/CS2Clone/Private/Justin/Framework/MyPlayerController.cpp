// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Framework/MyPlayerController.h"
#include "Justin/UI/GameplayDisplay.h"
#include "RealGameMode.h"
#include "Justin/Framework/MyPlayerState.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/WidgetLayoutLibrary.h"

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
	MatchState = CurrMatchState;
	if (IsLocalController())
		OnRep_OnMatchStateChanged();
}

void AMyPlayerController::OnRep_OnMatchStateChanged()
{
	if (MatchState == MatchState::Cooldown) {
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		UE_LOG(LogTemp, Warning, TEXT("[%s]: MatchState: %s"), *GetNameSafe(this), *MatchState::Cooldown.ToString());
	}
}
