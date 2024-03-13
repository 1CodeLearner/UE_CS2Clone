// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGameplayItem.generated.h"

class USkeletalMeshComponent;

UCLASS()
class CS2CLONE_API ACGameplayItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ACGameplayItem();
	UPROPERTY(EditDefaultsOnly, Category="Settings")
	TObjectPtr<USkeletalMeshComponent> SKMComponent;
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
