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
public:
	AMyPlayerController();
	void DisplayGameplay();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:

	//�� UI
	UPROPERTY(EditDefaultsOnly, Category = "Settings|UI")
	TSubclassOf<UGameplayDisplay> GameplayDisplayClass;
	UPROPERTY()
	TObjectPtr<UGameplayDisplay> GameplayDisplay;
	virtual void OnPossess(APawn* aPawn) override;

private:
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


	//ī��Ʈ�ٿ� ���� ó��
	UFUNCTION(BlueprintCallable)
	void StartTimer();
	float DestTime;
	float MarkedTime;
	bool bStart = false;
};
