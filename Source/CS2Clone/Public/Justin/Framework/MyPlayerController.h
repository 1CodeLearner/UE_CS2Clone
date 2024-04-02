// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 *
 */

class UGameplayDisplay;
enum class ETeam : uint8;

USTRUCT()
struct FMatchState
{
	GENERATED_BODY()

	UPROPERTY()
	ETeam Winner;
	UPROPERTY()
	FName CurrentState;
	UPROPERTY()
	int32 CT_Score;
	UPROPERTY()
	int32 T_Score; 
};

UCLASS()
class CS2CLONE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	friend class ARealGameMode;

public:
	AMyPlayerController();

	UPROPERTY(ReplicatedUsing = "OnRep_OnMatchStateChanged")
	FMatchState MatchState;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:

	//�� UI
	UPROPERTY(EditDefaultsOnly, Category = "Settings|UI")
	TSubclassOf<UGameplayDisplay> GameplayDisplayClass;
	UPROPERTY()
	TObjectPtr<UGameplayDisplay> GameplayDisplay;
	virtual void OnPossess(APawn* aPawn) override;

	virtual void SetMatchState(FName CurrMatchState);

private:
	UFUNCTION()
	void OnRep_OnMatchStateChanged();
	void DisplayResult();

	UFUNCTION(Server, Reliable)
	void Server_RequestWinner();
	UFUNCTION(Client, Reliable)
	void Client_RespondWinner(const FString& teamWon);


	//���� �ð� ����ȭ
	float GetServerTime() const;
	//������ Ŭ���̾�Ʈ �ð� ����
	float DeltaTime;
	UFUNCTION(Server, Reliable)
	void Server_RequestServerTime(float SentClientTime);
	UFUNCTION(Client, Reliable)
	void Client_RespondServerTime(float SentClientTime, float CurrentServerTime);
	UFUNCTION()
	void SendServerTimeRequest();

};
