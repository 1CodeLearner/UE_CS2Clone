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

	//주 UI
	UPROPERTY(EditDefaultsOnly, Category = "Settings|UI")
	TSubclassOf<UGameplayDisplay> GameplayDisplayClass;
	UPROPERTY()
	TObjectPtr<UGameplayDisplay> GameplayDisplay;
	virtual void OnPossess(APawn* aPawn) override;

private:
	//서버 시간 동기화
	float GetServerTime() const;
	//서버와 클라이언트 시간 차이
	float DeltaTime;
	UFUNCTION(Server, Reliable)
	void Server_RequestServerTime(float SentClientTime);
	UFUNCTION(Client, Reliable)
	void Client_RespondServerTime(float SentClientTime, float CurrentServerTime);
	UFUNCTION()
	void SendServerTimeRequest();


	//카운트다운 정보 처리
	UFUNCTION(BlueprintCallable)
	void StartTimer();
	float DestTime;
	float MarkedTime;
	bool bStart = false;
};
