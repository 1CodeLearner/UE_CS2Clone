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
	//UE_LOG(LogTemp, Warning, TEXT("Net: %s,PlayerState Beginplay"), GetWorld()->GetNetMode() == NM_Client ? TEXT("Client") : TEXT("Server"));

	//UE_LOG(LogTemp, Warning, TEXT("PlayerID: %s"), PlayerId);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("PlayerID: %s"), ));
}

void AMyPlayerState::OnRep_TeamAssigned()
{
	//UE_LOG(LogTemp, Warning, TEXT("OnRep_Try"));
	TestCharacter = TestCharacter ? TestCharacter : Cast<ACharacter>(GetPawn());
	SetTeamMesh();
}

void AMyPlayerState::SetTeamMesh()
{
	switch (TeamType)
	{
	case ETeam::TEAM_CT:
		if (TestCharacter)
			TestCharacter->GetMesh()->SetSkeletalMesh(FemaleMesh);
		break;
	case ETeam::TEAM_T:
		if (TestCharacter)
			TestCharacter->GetMesh()->SetSkeletalMesh(MaleMesh); 
		break;
	}
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerState, TeamType);
}

void AMyPlayerState::SetCharacter(ACharacter* Controlled)
{
	if (Controlled)
		TestCharacter = Controlled;
}
