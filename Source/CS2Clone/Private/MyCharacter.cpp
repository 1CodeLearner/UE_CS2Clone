#include "MyCharacter.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/SkeletalMesh.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include "InventoryComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Justin/Base/CInteractableItem.h"
#include "CSGameInstance.h"
#include "Justin/Weapon/CHandgun.h"
#include "PlayerAnimInstance.h"
#include "MyUserWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>

#include "Net/UnrealNetwork.h"
#include "Justin/Framework/MyPlayerController.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameModeBase.h>
#include "GameFramework/SpectatorPawn.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//skeleta mesh ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	// �޽� ��ġ ����
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(GetMesh(), FName(TEXT("headSocket")));
	camera->SetRelativeLocation(FVector(9.8f, 13.2f, 0));
	camera->SetRelativeRotation(FRotator(0, 80, 270));
	//camera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);

	//invencomp
	invenComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));

	GunComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Gun"));
	GunComponent->SetupAttachment(GetMesh(), FName(TEXT("GunPosition")));
	GunComponent->SetRelativeLocation(FVector(-7.144f, 3.68f, 0));
	GunComponent->SetRelativeRotation(FRotator(3.4f, 75.699f, 6.6424f));


}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	////get APlayerController
	//APlayerController* playerContoller = Cast<APlayerController>(Controller);
	////get subSystem
	//UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerContoller-//>GetLocalPlayer());
	//subSystem->AddMappingContext(imc_Default, 0);
	anim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	hasPistol = false;
	MyUserWidget = Cast<UMyUserWidget>(CreateWidget(GetWorld(), MyUserWidgetFactory));
	MyUserWidget->AddToViewport();


	if (IsLocallyControlled())
	{
		MappingContext();
	}

	CurrHp = MaxHP;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(ia_Move, ETriggerEvent::Triggered, this, &AMyCharacter::EnhancedMove);
		enhancedInputComponent->BindAction(ia_Jump, ETriggerEvent::Triggered, this, &AMyCharacter::EnhancedJump);
		enhancedInputComponent->BindAction(ia_Look, ETriggerEvent::Triggered, this, &AMyCharacter::EnhancedLook);
		enhancedInputComponent->BindAction(ia_GetDrop, ETriggerEvent::Started, this, &AMyCharacter::EnhancedDrop);
		enhancedInputComponent->BindAction(ia_InputItemSlot, ETriggerEvent::Started, this, &AMyCharacter::SelectItem);
		enhancedInputComponent->BindAction(ia_Fire, ETriggerEvent::Started, this, &AMyCharacter::EnhancedFire);
		enhancedInputComponent->BindAction(ia_Reload, ETriggerEvent::Started, this, &AMyCharacter::EnhancedReload);
	}
}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyCharacter, handGun);
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	MappingContext();
}

void AMyCharacter::EnhancedMove(const struct FInputActionValue& value)
{
	FVector2D dir = value.Get<FVector2D>();
	FVector originVec = FVector(dir.Y, dir.X, 0);
	FVector newVec = GetTransform().TransformVector(originVec);

	AddMovementInput(newVec);
}

void AMyCharacter::EnhancedJump(const struct FInputActionValue& value)
{
	Jump();
}

void AMyCharacter::EnhancedLook(const struct FInputActionValue& value)
{
	FVector2D dir = value.Get<FVector2D>();

	AddControllerYawInput(dir.X);
	AddControllerPitchInput(dir.Y);
	
}

void AMyCharacter::EnhancedDrop(const struct FInputActionValue& value)
{
	DetachGun();
}

void AMyCharacter::SelectItem(const struct FInputActionValue& value)
{
	//input �Է°� ������
	int32 slotIdx = value.Get<float>();
	slotIdx--;

	//���� �ε����� �� ������ ���� / �κ��丮��������
	EInventorySlotType HaveItemSlot = invenComp->myItems[slotIdx].InventorySlotType;

	// �� ������ ���Կ� �������� ������ ����
	if (HaveItemSlot == EInventorySlotType::INV_MAX) return;

	UE_LOG(LogTemp, Warning, TEXT("item : %d"), HaveItemSlot);

	//������ �������� ����� ����� �� �ִ� �Լ��� ȣ��
	AttachGun();
}

void AMyCharacter::EnhancedFire(const struct FInputActionValue& value)
{
	//if (hasPistol && invenComp->myItems[1].InventorySlotType == EInventorySlotType::INV_MAX) return;
	PlayerFIre();
}

void AMyCharacter::EnhancedReload(const struct FInputActionValue& value)
{
	PlayerReload();
}

void AMyCharacter::MappingContext()
{
	//get APlayerController
	APlayerController* playerContoller = Cast<APlayerController>(GetController());
	//get subSystem
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerContoller->GetLocalPlayer());
	if (HasAuthority())
	{
		const FString Test = GetWorld()->IsNetMode(NM_Client) ? TEXT("Client") : TEXT("Server");
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, FString::Printf(TEXT("HasAuthority. subSystem:%s. Net:%s"),
			subSystem ? TEXT("YES") : TEXT("NO"), *Test));
	}
	else
	{
		const FString Test = GetWorld()->IsNetMode(NM_Client) ? TEXT("Client") : TEXT("Server");
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, FString::Printf(TEXT("NoAuthority. subSystem:%s. Net:%s"),
			subSystem ? TEXT("YES") : TEXT("NO"), *Test));
	}
	if (subSystem)
	{
		subSystem->AddMappingContext(imc_Default, 0);
	}

}


//new version
bool AMyCharacter::GetItem(FItemType itemInfo)
{
	// �κ��丮���� ���° �ڸ��� �����ϴ���?
	// ����Ÿ�� ���� ��Ƴ��� ����ȯ
	int32 invenSlotType = (int32)(itemInfo.InventorySlotType);

	//���� �κ��� �ִ� ������ �迭���� ������ Ÿ���� max�� �ƴϸ� false ���� 
	//���� ����Ÿ�Կ� �������� ���������� �ȵ� 
	if (invenComp->myItems[invenSlotType].InventorySlotType != EInventorySlotType::INV_MAX) return false;

	//�� �κ� ���� Ÿ�Կ� �浹ü ���������� �߰�
	invenComp->myItems[invenSlotType] = itemInfo;
	return true;
}


void AMyCharacter::AttachGun()
{
	// 2�� ���� ������ ��������� ����
	if (invenComp->myItems[1].InventorySlotType == EInventorySlotType::INV_MAX) return;

	ServerAttachGun();
}

void AMyCharacter::ServerAttachGun_Implementation()
{
	MultiAttachGun();
}

void AMyCharacter::MultiAttachGun_Implementation()
{
	//�ڵ�� ������ ���� ������
	if (!hasPistol) // true�� ������ 
	{
		// �ڵ�� ����
		if (HasAuthority()) 
		{
			handGun = GetWorld()->SpawnActor<ACHandgun>(invenComp->myItems[1].GameplayItemClass, GetActorLocation(), GetActorRotation());
			if (handGun != nullptr)
				// �ڵ���� �� ������Ʈ�� ���̱�
				handGun->AttachToComponent(GunComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
		//�ڵ�� �������ִ�
		hasPistol = true;
		if(HasAuthority())
			handGun->SetOwner(this);
		anim->hasPistol = true;
	}

}

// �� ������� ���� �� ����
void AMyCharacter::DetachGun()
{
	if (invenComp->myItems[1].InventorySlotType == EInventorySlotType::INV_MAX) return;
	ServerDetachGun();
}

void AMyCharacter::ServerDetachGun_Implementation()
{
	MultiDetachGun();

}

void AMyCharacter::MultiDetachGun_Implementation()
{
	// �� ������ ������
	if (hasPistol)
	{
		
		FVector GunLoc = GunComponent->GetComponentLocation();
		//FVector DropLoc = GunComponent;
		UE_LOG(LogTemp, Warning, TEXT("Drop Item"));
		//�� �տ��� ����
		handGun->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		//�ʵ忡 �� ����
		GetWorld()->SpawnActor<ACInteractableItem>(invenComp->myItems[1].DroppedItemClass, GunLoc, GetActorRotation());
		//����ִ� �������� �ٲ�ġ��
		invenComp->myItems[1] = invenComp->myItems[0];
		//�� �Ȱ������ִ�
		hasPistol = false;
		handGun->Destroy();
	}
		anim->hasPistol = false;

}

void AMyCharacter::PlayerFIre()
{
	ServerPlayerFire();
}

void AMyCharacter::ServerPlayerFire_Implementation()
{
	MultiPlayerFire();	
	
}

void AMyCharacter::MultiPlayerFire_Implementation()
{
	//if(handGun == nullptr) return;
	//hasPistol�� true �� �������� false ����
	if (!hasPistol) return;
	if (handGun->CanFire())
	{
		PlayAnimMontage(pistolMontage, 1.0f, FName(TEXT("Fire")));
		if (IsLocallyControlled())
		{
			handGun->Fire();
		}
	}
}

void AMyCharacter::PlayerReload()
{
	ServerReload();
}

void AMyCharacter::ServerReload_Implementation()
{
	MultiReload();
}

void AMyCharacter::MultiReload_Implementation()
{
	if (!hasPistol) return;
	if (handGun->CanReload())
	{
		PlayAnimMontage(pistolMontage, 1.0f, FName(TEXT("Reload")));
		handGun->Reload();
	}
}

void AMyCharacter::PlayerDead()
{
	ServerDead();
}

void AMyCharacter::ServerDead_Implementation()
{
	MultiDead();
	
}

void AMyCharacter::MultiDead_Implementation()
{
	if (CurrHp <= 0)
	{
		anim->isDeath = true;
		if (HasAuthority())
		{

		UE_LOG(LogTemp,Warning, TEXT("dead"));
		// �浹 �ȵǰ�
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// Movement ������Ʈ ��Ȱ��
		GetCharacterMovement()->DisableMovement();
		MultiDetachGun();
		AMyPlayerController* pc = Cast<AMyPlayerController>(GetController()); 
		auto spectatorTest = GetWorld()->SpawnActor<ASpectatorPawn>(GetWorld()->GetAuthGameMode()->SpectatorClass, GetActorLocation(), GetActorRotation());
		
		pc->Possess(spectatorTest);
		Destroy();
		
		}
	}
}

