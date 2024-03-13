// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Framework/MyPlayerController.h"
#include "Justin/UI/GameplayDisplay.h"

AMyPlayerController::AMyPlayerController()
{
	GameplayDisplayClass = nullptr;
	GameplayDisplay = nullptr;
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
	DisplayGameplay();
}

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (HasAuthority())
	{
		const FString Test = GetWorld()->IsNetMode(NM_Client) ? TEXT("Client") : TEXT("Server");
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, FString::Printf(TEXT("HasAuthority. Net:%s. Pawn:%s"),
			*Test, *GetNameSafe(InPawn)));
	}
	else
	{
		const FString Test = GetWorld()->IsNetMode(NM_Client) ? TEXT("Client") : TEXT("Server");
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, FString::Printf(TEXT("NoAuthority. Net:%s. Pawn:%s"),
			*Test, *GetNameSafe(InPawn)));
	}
}
