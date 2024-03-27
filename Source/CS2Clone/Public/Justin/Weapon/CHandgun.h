// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Justin/Base/CGameplayItem.h"
#include "GameplayTagContainer.h"
#include "CHandgun.generated.h"

class UDataTable;
class UAnimSequence;

//For UI update only 
DECLARE_DYNAMIC_DELEGATE(FWeaponUpdateDelegate);

UCLASS()
class CS2CLONE_API ACHandgun : public ACGameplayItem
{
	GENERATED_BODY()

public:
	ACHandgun();
	//UI 업대이트를 위한 delegate
	FWeaponUpdateDelegate OnWeaponUpdate;

	//플레이어 케릭터가 사용할 수 있는 기능
	UFUNCTION(BlueprintCallable, Category = "Settings|Test")
	bool CanReload() const;
	UFUNCTION(BlueprintCallable, Category = "Settings|Test")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Settings|Test")
	bool CanFire() const;
	UFUNCTION(BlueprintCallable, Category = "Settings|Test")
	void Fire();
	//

	virtual void OnRep_Owner() override;

	//태스팅용: 무시
	/*float timerTotal = 5.f;
	float currTime = 0.f;*/

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;

	//총 초기화 하기 위한 정보
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Init")
	FGameplayTag WeaponTag;
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Init")
	TObjectPtr<UDataTable> DT_Weapon;

	//애니메이션
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Animation")
	TObjectPtr<UAnimSequence> FireAnimSeq;
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Animation")
	TObjectPtr<UAnimSequence> ReloadAnimSeq;

	//재장전
	UFUNCTION(Server, Reliable)
	void Server_Reload();
	UFUNCTION(NetMulticast, Reliable)
	void Multi_Reload();
	UFUNCTION()
	void CompleteReload(); //서버에서만 불러진다
	UFUNCTION(Client, Reliable)
	void Client_CompleteReload();


	//발사
	UFUNCTION(Server, Reliable)
	void Server_Fire(FHitResult Hit);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_Fire();

private:
	//총 정보
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Settings|Rounds")
	int ReserveTotalRounds;
	UPROPERTY(VisibleAnywhere, Category = "Settings|Rounds")
	int InMagTotalRounds;
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Settings|Rounds")
	int InMagRemainingRounds;

	//클라이언트가 재장전 하고 있는지 확인
	bool bClientReloading;
	//출력
	void LogGunState();
};
