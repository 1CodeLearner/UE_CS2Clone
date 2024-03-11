// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

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

	UPROPERTY(VisibleInstanceOnly)
	TArray<class AActor*> allItem;
	
	UPROPERTY(EditAnywhere)
	class UInventoryComponent* invenComp;

	//camera
	UPROPERTY(EditAnywhere)
	class UCameraComponent* camera;
	

public:
	void EnhancedMove(const struct FInputActionValue& value);
	void EnhancedJump(const struct FInputActionValue& value);
	void EnhancedLook(const struct FInputActionValue& value);
	void EnhancedGetDrop(const struct FInputActionValue& value);
};
