// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Framework/MyPlayerState.h"
#include "Justin/Framework/MyPlayerController.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

AMyPlayerState::AMyPlayerState()
{
	TeamType = ETeam::TEAM_MAX;
}

void AMyPlayerState::BeginPlay()
{
	Super::BeginPlay();

	auto PC = Cast<AMyPlayerController>(GetPlayerController());
	
	if (PC)
	{
		TestCharacter = PC->GetCharacter();
		SetTeamMesh();
	}

	UE_LOG(LogTemp, Warning, TEXT("Net: %s, Beginplay, Character: %s"), GetWorld()->GetNetMode() == NM_Client ? TEXT("Client") : TEXT("Server"), *GetNameSafe(TestCharacter));
}

void AMyPlayerState::OnRep_Try()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Try"));
	SetTeamMesh();
}

void AMyPlayerState::SetTeamMesh()
{
	switch (TeamType)
	{
	case ETeam::TEAM_CT:
		if (TestCharacter) {
			TestCharacter->GetMesh()->SetSkeletalMesh(FemaleMesh);
		}
		break;
	case ETeam::TEAM_T:
		if (TestCharacter) {
			TestCharacter->GetMesh()->SetSkeletalMesh(MaleMesh);
		}
		break;
	}
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerState, TeamType);
}
