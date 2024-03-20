// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "MyCharacter.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Player 가져오자
	player = Cast<AMyCharacter>(TryGetPawnOwner());

}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (player)
	{
		// 앞 뒤 (dirV)
		dirV = FVector::DotProduct(player->GetActorForwardVector(), player->GetVelocity());

		// 좌 우 (dirH)
		dirH = FVector::DotProduct(player->GetActorRightVector(), player->GetVelocity());

		//0 ~ 360 --> -180 ~ 180
		pitchAngle = -player->GetBaseAimRotation().Pitch;

		pitchAngle = -player->GetBaseAimRotation().GetNormalized().Pitch;
	}
}
