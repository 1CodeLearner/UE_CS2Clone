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

	//springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	//springArm->SetupAttachment(RootComponent);
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(RootComponent);
	camera->SetWorldLocation(FVector(30, 0, 80));

	//invencomp
	invenComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));

	GunComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Gun"));
	GunComponent->SetupAttachment(GetMesh(), FName(TEXT("GunPosition")));
	GunComponent->SetRelativeLocation(FVector(-7.144f, 3.68f, 4.136f));
	GunComponent->SetRelativeRotation(FRotator(3.4f, 75.699f, 6.6424f));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//get APlayerController
	APlayerController* playerContoller = Cast<APlayerController>(GetController());
	//get subSystem
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerContoller->GetLocalPlayer());
	subSystem->AddMappingContext(imc_Default, 0);

	hasPistol = false;
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
	if(enhancedInputComponent != nullptr)
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
	
	UE_LOG(LogTemp, Warning, TEXT("Drop Item"));
	
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
	if(HaveItemSlot == EInventorySlotType::INV_MAX) return;
	
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

// old version
void AMyCharacter::GetItem(EInventorySlotType slotType)
{
	//FName slotName = UEnum::GetValueAsName<EInventorySlotType>(slotType);
	//UE_LOG(LogTemp, Warning, TEXT("slotName : %s"), *slotName.ToString());
	// �������� ��Ʈ������ ����ȯ
	int32 invenSlotType = (int32)slotType;

	
	// 1. ������ ���� �ߺ� �Ǹ� �ȵǰ� �κ� �迭�� 5�̻� �Ǹ� �������ʵ���
	
	// ���� �ν��Ͻ� ��������
	UCSGameInstance* gameInstance = Cast<UCSGameInstance>(GetWorld()->GetGameInstance());
	
	//invenComp->myItems.Add(gameInstance->defineItem[invenSlotType]);
	invenComp->myItems[invenSlotType] = gameInstance->defineItem[invenSlotType];
	// �������ִ°��߿� 0���� ũ�� return //or nullptr; 

	//���� �ν��Ͻ� ��������
	//UCSGameInstance* gameInstance = Cast<UCSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//�κ� ������Ʈ��
	//invenComp->myItems.Add(gameInstance->defineItem[slotType]);


}

//new version
bool AMyCharacter::GetItem(FItemType itemInfo)
{
	// �κ��丮���� ���° �ڸ��� �����ϴ���?
	// ����Ÿ�� ���� ��Ƴ��� ����ȯ
	int32 invenSlotType = (int32)(itemInfo.InventorySlotType);

	//���� �κ��� �ִ� ������ �迭���� ������ Ÿ���� max�� �ƴϸ� false ���� 
	//���� ����Ÿ�Կ� �������� ���������� �ȵ� 
	if(invenComp->myItems[invenSlotType].InventorySlotType != EInventorySlotType::INV_MAX) return false;

	//�� �κ� ���� Ÿ�Կ� �浹ü ���������� �߰�
	invenComp->myItems[invenSlotType] = itemInfo; 
	return true;
}


void AMyCharacter::AttachGun()
{
	// 2�� ���� ������ ��������� ����
	if(invenComp->myItems[1].InventorySlotType == EInventorySlotType::INV_MAX) return;
	
	//�ڵ�� ������ ���� ������
	if (!hasPistol) // true�� ������ 
	{
		// �ڵ�� ����
		handGun = GetWorld()->SpawnActor<ACHandgun>(invenComp->myItems[1].GameplayItemClass, GetActorLocation(), GetActorRotation());
		if(handGun != nullptr)
		// �ڵ���� �� ������Ʈ�� ���̱�
		handGun->AttachToComponent(GunComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

		//�ڵ�� �������ִ�
		hasPistol = true;
	}

}

// �� ������� ���� �� ����
void AMyCharacter::DetachGun()
{
	if (invenComp->myItems[1].InventorySlotType == EInventorySlotType::INV_MAX) return;

	// �� ������ ������
	if (hasPistol)
	{
		//�� �տ��� ����
		handGun->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		//�ʵ忡 �� ����
		GetWorld()->SpawnActor<ACInteractableItem>(invenComp->myItems[1].DroppedItemClass, GetActorLocation(), GetActorRotation());
		//����ִ� �������� �ٲ�ġ��
		invenComp->myItems[1] = invenComp->myItems[0];

		//�� �Ȱ������ִ�
		hasPistol = false;
	}
}

void AMyCharacter::PlayerFIre()
{
	//if(handGun == nullptr) return;
	//hasPistol�� true �� �������� false ����
	if(!hasPistol) return; 
	
	handGun->Fire();
}

void AMyCharacter::PlayerReload()
{
	//if (hasPistol && invenComp->myItems[1].InventorySlotType == EInventorySlotType::INV_MAX) return;
	if(!hasPistol) return; 
	handGun->Reload();
}

