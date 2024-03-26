// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

//인벤토리 슬롯 타입 전방선언
enum class EInventorySlotType : uint8;

UCLASS()
class CS2CLONE_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
public:
	//player Mapping Context
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imc_Default;

	//player input
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_Move;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_Look;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_Jump;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_GetDrop;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_InputItemSlot;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_Fire;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_Reload;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMyUserWidget> MyUserWidgetFactory;

	UPROPERTY()
	class UMyUserWidget* MyUserWidget;

public:
	UPROPERTY()
	class UPlayerAnimInstance* anim;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* pistolMontage;

public:
	UPROPERTY(EditAnywhere)
	class UInventoryComponent* invenComp;

	UPROPERTY(EditAnywhere)
	class USceneComponent* GunComponent;

	
	UPROPERTY(EditAnywhere)
	class ACHandgun* handGun;

	//camera
	UPROPERTY(EditAnywhere)
	class UCameraComponent* camera;

public:
	bool hasPistol = false;

	float MaxHP = 100;
	float CurrHp;


public:
	void EnhancedMove(const struct FInputActionValue& value);
	void EnhancedJump(const struct FInputActionValue& value);
	void EnhancedLook(const struct FInputActionValue& value);
	void EnhancedDrop(const struct FInputActionValue& value);
	void SelectItem(const struct FInputActionValue& value);
	void EnhancedFire(const struct FInputActionValue& value);
	void EnhancedReload(const struct FInputActionValue& value);

	void MappingContext();

	bool GetItem(struct FItemType itemInfo);

	void AttachGun();
	UFUNCTION(Server, Reliable)
	void ServerAttachGun();
	UFUNCTION(NetMulticast, Reliable)
	void MultiAttachGun();

	void DetachGun();
	UFUNCTION(Server, Reliable)
	void ServerDetachGun();
	UFUNCTION(NetMulticast, Reliable)
	void MultiDetachGun();


	void PlayerFIre();
	UFUNCTION(Server, Reliable)
	void ServerPlayerFire();
	UFUNCTION(NetMulticast, Reliable)
	void MultiPlayerFire();

	void PlayerReload();
	UFUNCTION(Server, Reliable)
	void ServerReload();
	UFUNCTION(NetMulticast, Reliable)
	void MultiReload();

	void PlayerDead();

};
