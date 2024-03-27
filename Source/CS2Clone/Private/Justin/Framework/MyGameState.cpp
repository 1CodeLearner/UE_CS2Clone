// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Framework/MyGameState.h"
#include "Net/UnrealNetwork.h"

void AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyGameState, CT_Score);
	DOREPLIFETIME(AMyGameState, T_Score);
}

void AMyGameState::OnRep_ScoreUpdate()
{
	UE_LOG(LogTemp, Warning, TEXT("CT: %d, T: %d"), CT_Score,T_Score);
}
