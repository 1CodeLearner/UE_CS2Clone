// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CGameplayActor.h"
#include "CWeaponBase.generated.h"

//For UI update only 
DECLARE_DYNAMIC_DELEGATE(FWeaponUpdateDelegate);

UCLASS()
class CS2CLONE_API ACWeaponBase : public ACGameplayActor
{
	GENERATED_BODY()

public:
	ACWeaponBase();
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

private:
	UPROPERTY(EditAnywhere, Replicated, Category="Settings|Rounds")
	int ReserveTotalRounds;
	UPROPERTY(EditAnywhere, Category="Settings|Rounds")
	int InMagTotalRounds;
	UPROPERTY(EditAnywhere, Replicated, Category="Settings|Rounds")
	int InMagRemainingRounds;
};
