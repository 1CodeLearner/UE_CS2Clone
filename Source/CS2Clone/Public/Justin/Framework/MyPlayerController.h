// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 *
 */

class UGameplayDisplay;

UCLASS()
class CS2CLONE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	friend class ARealGameMode; 

public:
	AMyPlayerController();
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
	UPROPERTY(ReplicatedUsing = "OnRep_OnMatchStateChanged")
	FName MatchState;
	UFUNCTION()
	void OnRep_OnMatchStateChanged();
	void DisplayResult();

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
