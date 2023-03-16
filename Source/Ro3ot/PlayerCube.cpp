// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCube.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetStringLibrary.h"

// Sets default values
APlayerCube::APlayerCube()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCube::OnActorBeginOverlap);
	health = 6;
	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f; // The camera follows at this distance behind the character
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraBoom->bUsePawnControlRotation = false; // Don't want arm to rotate when character does

	// Create a follow camera	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ProjectileArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileArrow"));
	ProjectileArrow->SetupAttachment(RootComponent);

	ProjectileLocation = CreateDefaultSubobject<USceneComponent>("ProjectileLocation");
	ProjectileLocation->SetupAttachment(ProjectileArrow);
}

// Called when the game starts or when spawned
void APlayerCube::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCube::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FireTimer += DeltaSeconds;
	if (FireTimer >= (1 / Settings.FireRate))
	{
		CanShoot = true;
	}
}


void APlayerCube::SpawnProjectile(FVector Start, FVector Direction)
{
	FActorSpawnParameters Params;
	FVector Location = ProjectileLocation->GetComponentLocation();
	FRotator Rotation = Direction.Rotation();

	GetWorld()->SpawnActor<AFPS_Projectile>(Settings.ProjectileClass, Location, Rotation, Params);
}

void APlayerCube::Shoot(bool TriggerIsPulled)
{
	FString stringDebug = UKismetStringLibrary::Conv_BoolToString(TriggerIsPulled);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *stringDebug);
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Blue, UKismetStringLibrary::Conv_BoolToString(TriggerIsPulled));
	FVector Direction = this->GetActorForwardVector();
	FVector Start = this->GetActorLocation() + (Direction * 100.0f);
	FVector End = Start + Direction * InteractionMaxDistance;

	if (!TriggerIsPulled)
	{
		TriggerHasBeenReleased = true;
		return;
	}

	if (Settings.ShotType != Auto && !TriggerHasBeenReleased)
	{
		return;
	}

	TriggerHasBeenReleased = false;

	if (CanShoot)
	{
		if (Settings.ProjectileClass != nullptr)
		{
			SpawnProjectile(Start, Direction);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("HitScan"));
			HitScan(Start, Direction);
		}

		FireTimer = 0.0f;
		CanShoot = false;
	}
}

void APlayerCube::HitScan(FVector Start, FVector Direction)
{
	FHitResult Hit = FHitResult();

	FVector End = Start + (Direction * Settings.MaxDistance);

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);

	FVector HitLocation = Hit.bBlockingHit ? Hit.Location : End;

	DrawDebugSphere(GetWorld(), HitLocation, 10.f, 8, FColor::Red, false, 1.f);

}


void APlayerCube::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPS_Projectile* projectileInstance = Cast<AFPS_Projectile>(OtherActor);
	if (projectileInstance != nullptr)
	{
		for (int i = 0; i < projectileInstance->PSettings.EnemyClass.Num(); i++)
		{
			if (projectileInstance->PSettings.EnemyClass[i] == this->GetClass())
			{
				SubDamage(projectileInstance->Damage);
				return;
			}
		}
	}
}

void APlayerCube::SubDamage(int dmg)
{
	if (!invulnerabiltyFrame)
	{
		health -= dmg;
		if (health <= 0)
		{
			health = 0;
		}
	}
}
