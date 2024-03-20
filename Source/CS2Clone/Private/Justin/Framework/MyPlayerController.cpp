// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Framework/MyPlayerController.h"
#include "Justin/UI/GameplayDisplay.h"

AMyPlayerController::AMyPlayerController()
{
	GameplayDisplayClass = nullptr;
	GameplayDisplay = nullptr;
	DeltaTime = 0.f;
	DestTime = 10.f;
	MarkedTime = 0.f;
}

void AMyPlayerController::DisplayGameplay()
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
	if (IsLocalController()) 
	{
		DisplayGameplay();
	}
	if (!HasAuthority())
	{
		SendServerTimeRequest();
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &AMyPlayerController::SendServerTimeRequest, 5.f, true);
	}
}

void AMyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bStart && GameplayDisplay)
	{
		float remainingTime = DestTime - (MarkedTime + DeltaSeconds);		
		if (remainingTime <= 0.f)
		{
			bStart = false;
			GameplayDisplay->SetTime(0.f);
		}
		else {
			GameplayDisplay->SetTime(remainingTime);
			MarkedTime += DeltaSeconds;
		}
	}
}

void AMyPlayerController::SendServerTimeRequest()
{
	Server_RequestServerTime(GetWorld()->GetTimeSeconds());
}

void AMyPlayerController::StartTimer()
{
	MarkedTime = GetServerTime();
	DestTime = MarkedTime + 2.f;
	bStart = true;
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
	UE_LOG(LogTemp, Warning, TEXT("Nice"));
	Client_RespondServerTime(SentClientTime, GetWorld()->GetTimeSeconds());
}

void AMyPlayerController::Client_RespondServerTime_Implementation(float SentClientTime, float CurrentServerTime)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - SentClientTime;
	float EstimatedServerTime = CurrentServerTime + RoundTripTime / 2.f;
	
	DeltaTime = EstimatedServerTime - GetWorld()->GetTimeSeconds();
}

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	UE_LOG(LogTemp, Warning, TEXT("Net: %s"), GetWorld()->GetNetMode() == NM_Client ? TEXT("Client") : TEXT("Server"));

}
