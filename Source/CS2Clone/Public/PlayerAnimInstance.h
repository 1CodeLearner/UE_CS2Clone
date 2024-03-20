// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CS2CLONE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//UFUNCTION()
	//void AnimNotify_ReloadComplete();

	//UFUNCTION()
	//void AnimNotify_DieEnd();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool hasPistol;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float dirH;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float dirV;

	UPROPERTY()
	class AMyCharacter* player;

	UPROPERTY(BlueprintReadOnly)
	float pitchAngle;

	UPROPERTY(BlueprintReadOnly)
	bool isDeath = false;
};
