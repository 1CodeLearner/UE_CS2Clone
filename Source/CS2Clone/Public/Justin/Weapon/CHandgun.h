// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Base/CGameplayItem.h"
#include "GameplayTagContainer.h"
#include "CHandgun.generated.h"

class UDataTable;

//For UI update only 
DECLARE_DYNAMIC_DELEGATE(FWeaponUpdateDelegate);

UCLASS()
class CS2CLONE_API ACHandgun : public ACGameplayItem
{
	GENERATED_BODY()

public:
	ACHandgun();
	//For UI update only 
	FWeaponUpdateDelegate OnWeaponUpdate;
	//
	UFUNCTION(BlueprintCallable, Category = "Settings|Test")
	bool CanReload() const;
	UFUNCTION(BlueprintCallable, Category = "Settings|Test")
	void Reload();

	UFUNCTION(Server, Reliable)
	void ServerReload();
	void ServerReload_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Settings|Test")
	bool CanFire() const;
	UFUNCTION(BlueprintCallable, Category = "Settings|Test")
	void Fire();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, Category="Settings|Init")
	FGameplayTag WeaponTag;
	UPROPERTY(EditDefaultsOnly, Category="Settings|Init")
	TObjectPtr<UDataTable> DT_Weapon;

private:
	UPROPERTY(VisibleAnywhere, Replicated, Category="Settings|Rounds")
	int ReserveTotalRounds;
	UPROPERTY(VisibleAnywhere, Category="Settings|Rounds")
	int InMagTotalRounds;
	UPROPERTY(VisibleAnywhere, Replicated, Category="Settings|Rounds")
	int InMagRemainingRounds;
};
