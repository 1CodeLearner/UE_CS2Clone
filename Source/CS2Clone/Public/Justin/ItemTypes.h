// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.generated.h"

class ACWeaponBase;
class ACInteractableItem;
class UTexture2D;


UENUM()
enum class EInventorySlotType
{
	INV_PRIMARY UMETA(DisplayName="Primary"),
	INV_SECONDARY UMETA(DisplayName="Secondary"),
	INV_MELEE UMETA(DisplayName="Melee"),
	INV_UTILITY UMETA(DisplayName="Utility"),
	INV_BOMB UMETA(DisplayName="Bomb"),
};

USTRUCT()
struct FWeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText DisplayName;

	UPROPERTY(EditAnywhere)
	EInventorySlotType InventorySlotType;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> InventoryImage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> StoreImage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACInteractableItem> DroppedWeaponClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACWeaponBase> WeaponBaseClass;

	UPROPERTY(EditAnywhere, meta=(ClampMin="0.0"))
	int TotalRounds;

	UPROPERTY(EditAnywhere, meta=(ClampMin="0.0"))
	int InMagRounds;
};

