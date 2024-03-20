// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Weapon/Anim/CWeaponAnim.h"
#include "Justin/Weapon/CHandgun.h"

void UCWeaponAnim::AnimNotify_Reload()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("22222222")));
}

void UCWeaponAnim::AnimNotify_Fire()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Firedman")));
}
