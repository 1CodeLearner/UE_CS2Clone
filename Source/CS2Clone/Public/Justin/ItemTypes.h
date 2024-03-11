// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemTypes.generated.h"

class ACWeaponBase;
class ACInteractableItem;
class UTexture2D;


UENUM(BlueprintType)
enum class EInventorySlotType
{
	INV_PRIMARY UMETA(DisplayName="Primary"),
	INV_SECONDARY UMETA(DisplayName="Secondary"),
	INV_MELEE UMETA(DisplayName="Melee"),
	INV_UTILITY UMETA(DisplayName="Utility"),
	INV_BOMB UMETA(DisplayName="Bomb"),
	INV_MAX UMETA(DisplayName="Max")
};

USTRUCT(BlueprintAble)
struct FWeaponInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	FWeaponInfo() 
	{
		DisplayName = FText::FromString(FString::Printf(TEXT("")));
		InventorySlotType = EInventorySlotType::INV_MAX;
		InventoryImage = nullptr;
		StoreImage = nullptr;
		DroppedWeaponClass = nullptr;
		WeaponBaseClass = nullptr;
		ReserveTotalRounds = 0;
		InMagTotalRounds = 0;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EInventorySlotType InventorySlotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UTexture2D> InventoryImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UTexture2D> StoreImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ACInteractableItem> DroppedWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ACWeaponBase> WeaponBaseClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0"), Category = "Weapon")
	int ReserveTotalRounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0"), Category = "Weapon")
	int InMagTotalRounds;
};

