// Fill out your copyright notice in the Description page of Project Settings.


#include "Justin/Weapon/CHandgun.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Justin/ItemTypes.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"

ACHandgun::ACHandgun()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	ReserveTotalRounds = 0;
	InMagTotalRounds = 0;
	InMagRemainingRounds = 0;
}


void ACHandgun::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(DT_Weapon) && ensure(WeaponTag.IsValid()))
	{
		FWeapon* weaponInfo = DT_Weapon->FindRow<FWeapon>(WeaponTag.GetTagName(), FString::Printf(TEXT("")));
		if (weaponInfo)
		{
			ReserveTotalRounds = weaponInfo->ReserveTotalRounds;
			InMagTotalRounds = weaponInfo->InMagTotalRounds;
			InMagRemainingRounds = InMagTotalRounds;
		}
	}
}

void ACHandgun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACHandgun, ReserveTotalRounds, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ACHandgun, InMagRemainingRounds, COND_OwnerOnly);
}

void ACHandgun::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black,
		FString::Printf(TEXT("Owner: %s"), *GetNameSafe(GetOwner())));

	if (GetWorld()->IsNetMode(NM_Client))
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Client")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Server")));
	}
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Reserve: %d"), ReserveTotalRounds));
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Total: %d"), InMagTotalRounds));
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black,
		FString::Printf(TEXT("Remaining: %d"), InMagRemainingRounds));
}

bool ACHandgun::CanReload() const
{
	return InMagRemainingRounds != InMagTotalRounds && ReserveTotalRounds != 0;
}

void ACHandgun::Reload()
{
	if (CanReload())
	{
		SKMComponent->PlayAnimation(ReloadAnimSeq, false);
		//Temporary timer
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ACHandgun::Server_Reload, 2.f, false);
	}
}

void ACHandgun::Server_Reload_Implementation()
{
	ensure(CanReload());
	int refillAmt = InMagTotalRounds - InMagRemainingRounds;

	int ReserveTemp = ReserveTotalRounds - refillAmt;
	if (ReserveTemp < 0)
	{
		refillAmt += ReserveTemp;
		ReserveTotalRounds = 0;
	}
	else
		ReserveTotalRounds = ReserveTemp;

	InMagRemainingRounds += refillAmt;
}

bool ACHandgun::CanFire() const
{
	return InMagRemainingRounds > 0;
}

void ACHandgun::Fire()
{
	//get actor that was hit
	FHitResult Hit;
	AActor* camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	if (CanFire() && ensure(FireAnimSeq) && camera)
	{
		SKMComponent->PlayAnimation(FireAnimSeq, false);
		FVector Start = camera->GetActorLocation();
		FVector End = Start + camera->GetActorForwardVector() * 50000.f;
		FCollisionQueryParams params;
		params.AddIgnoredActor(GetOwner() ? GetOwner() : this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, params);
		AMyCharacter* character = nullptr;
		if (bHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString::Printf(TEXT("HitActor: %s"), *GetNameSafe(Hit.GetActor())));
			//if hit actor was player, send a server RPC
			character = Cast<AMyCharacter>(Hit.GetActor());
		}

		Server_Fire(character, Hit);
		DrawDebugLine(GetWorld(), Start, End, FColor::Black, false, 2.f);
	}
}

void ACHandgun::Server_Fire_Implementation(AActor* ActorHit, FHitResult Hit)
{
	ensure(CanFire());
	InMagRemainingRounds--;

	if (InMagRemainingRounds < 0)
		InMagRemainingRounds = 0;
}
