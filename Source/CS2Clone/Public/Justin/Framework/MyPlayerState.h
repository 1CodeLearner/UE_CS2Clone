// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 *
 */

UENUM()
enum class ETeam : uint8
{
	TEAM_CT		UMETA(DisplayName = "CounterTerrorist"),
	TEAM_T		UMETA(DisplayName = "Terrorist"),
	TEAM_MAX	UMETA(DisplayName = "Max")
};

UCLASS()
class CS2CLONE_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	friend class ARealGameMode;

public:
	AMyPlayerState();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;




protected:
	UPROPERTY(ReplicatedUsing = OnRep_Try)
	ETeam TeamType;
	UFUNCTION()
	void OnRep_Try();
	UPROPERTY()
	ACharacter* TestCharacter; 

	UPROPERTY(EditAnywhere)
	USkeletalMesh* FemaleMesh;
	UPROPERTY(EditAnywhere)
	USkeletalMesh* MaleMesh;
private:
	void SetTeamMesh(); 
};
