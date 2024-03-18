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

	//총 초기화
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
	

	//총 정보 출력
	LogGunState();


}

void ACHandgun::LogGunState()
{
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

	//소지품에 남은 총알 
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Reserve: %d"), ReserveTotalRounds));
	//탄창 총 양
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Total: %d"), InMagTotalRounds));
	//탄창안에 남은 종알
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black,
		FString::Printf(TEXT("Remaining: %d"), InMagRemainingRounds));
}

bool ACHandgun::CanReload() const
{
	//탄창이 채워지지 않았을 경우 + 예비 탄약이 있을 경우
	return InMagRemainingRounds != InMagTotalRounds && ReserveTotalRounds != 0;
}

//이 함수로 장전
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

//이 함수로 쏜다
void ACHandgun::Fire()
{
	FHitResult Hit;
	AActor* camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	if (CanFire() && ensure(FireAnimSeq) && camera)
	{
		//총 쏘는 애니매이션
		SKMComponent->PlayAnimation(FireAnimSeq, false);
		
		FVector Start = camera->GetActorLocation();
		FVector End = Start + camera->GetActorForwardVector() * 50000.f;
		FCollisionQueryParams params;
		params.AddIgnoredActor(GetOwner() ? GetOwner() : this);
		
		//AMyCharacter를 찾는 Line trace 
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, params);
		AMyCharacter* character = nullptr;
		if (bHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString::Printf(TEXT("HitActor: %s"), *GetNameSafe(Hit.GetActor())));
			//if hit actor was player, send a server RPC
			character = Cast<AMyCharacter>(Hit.GetActor());
		}
		
		//발사 되어서 서버에 총 정보 처리하기
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
