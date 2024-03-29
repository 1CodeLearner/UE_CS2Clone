// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Framework/MyGameState.h"
#include "Net/UnrealNetwork.h"
#include "Justin/Framework/MyPlayerState.h"

void AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyGameState, CT_Score);
	DOREPLIFETIME(AMyGameState, T_Score);
}

void AMyGameState::OnRep_ScoreUpdate()
{
	UE_LOG(LogTemp, Warning, TEXT("CT: %d, T: %d"), CT_Score, T_Score);
}

void AMyGameState::OnPlayerDead(AMyCharacter* character, bool& bIsMatchOver)
{
	bIsMatchOver = false;
	if (character)
	{
		auto PS = Cast<AMyPlayerState>(character->GetPlayerState());
		if (PS)
		{
			PS->SetDead(true);
		}

		if (Team_CounterTerrorist.Contains(PS))
		{
			if (IsTeamEliminated(Team_CounterTerrorist))
			{
				CT_Score++;
				bIsMatchOver = true;
			}
		}
		else
		{
			if (IsTeamEliminated(Team_Terrorist))
			{
				T_Score++;
				bIsMatchOver = true;
			}
		}
	}
}

bool AMyGameState::IsTeamEliminated(const TArray<AMyPlayerState*>& Team)
{
	for (auto i : Team) {
		if(!i->GetDead())
			return false;
	}
	return true;
}
