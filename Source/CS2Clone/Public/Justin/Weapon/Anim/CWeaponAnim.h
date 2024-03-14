// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CWeaponAnim.generated.h"

/**
 *
 */
UCLASS()
class CS2CLONE_API UCWeaponAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void AnimNotify_Reload();
	UFUNCTION()
	void AnimNotify_Fire();
};
