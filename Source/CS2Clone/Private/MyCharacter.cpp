#include "MyCharacter.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/SkeletalMesh.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include "InventoryComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//skeleta mesh 셋팅
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}
	
	// 메쉬 위치 셋팅
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	//springArm->SetupAttachment(RootComponent);
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(RootComponent);

	//invencomp
	invenComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
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
		enhancedInputComponent->BindAction(ia_GetDrop, ETriggerEvent::Started, this, &AMyCharacter::EnhancedGetDrop);
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

void AMyCharacter::EnhancedGetDrop(const struct FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("f"));
}

