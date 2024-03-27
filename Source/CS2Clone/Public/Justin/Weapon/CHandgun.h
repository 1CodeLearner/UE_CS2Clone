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
	//UI ������Ʈ�� ���� delegate
	FWeaponUpdateDelegate OnWeaponUpdate;

	//�÷��̾� �ɸ��Ͱ� ����� �� �ִ� ���
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

	//�½��ÿ�: ����
	/*float timerTotal = 5.f;
	float currTime = 0.f;*/

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;

	//�� �ʱ�ȭ �ϱ� ���� ����
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Init")
	FGameplayTag WeaponTag;
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Init")
	TObjectPtr<UDataTable> DT_Weapon;

	//�ִϸ��̼�
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Animation")
	TObjectPtr<UAnimSequence> FireAnimSeq;
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Animation")
	TObjectPtr<UAnimSequence> ReloadAnimSeq;

	//������
	UFUNCTION(Server, Reliable)
	void Server_Reload();
	UFUNCTION(NetMulticast, Reliable)
	void Multi_Reload();
	UFUNCTION()
	void CompleteReload(); //���������� �ҷ�����
	UFUNCTION(Client, Reliable)
	void Client_CompleteReload();


	//�߻�
	UFUNCTION(Server, Reliable)
	void Server_Fire(FHitResult Hit);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_Fire();

private:
	//�� ����
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Settings|Rounds")
	int ReserveTotalRounds;
	UPROPERTY(VisibleAnywhere, Category = "Settings|Rounds")
	int InMagTotalRounds;
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Settings|Rounds")
	int InMagRemainingRounds;

	//Ŭ���̾�Ʈ�� ������ �ϰ� �ִ��� Ȯ��
	bool bClientReloading;
	//���
	void LogGunState();
};
