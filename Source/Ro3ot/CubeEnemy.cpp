// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeEnemy.h"
#include "PlayerCube.h"
#include "Kismet/GameplayStatics.h"

#include "Kismet/KismetStringLibrary.h"

// Sets default values
ACubeEnemy::ACubeEnemy()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACubeEnemy::OnActorBeginOverlap);

	USkeletalMeshComponent* mesh;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshContainer(TEXT("/Script/Engine.SkeletalMesh'/Game/Blueprint/Enemies/Rig/Ro3ot_Mechant_Rig.Ro3ot_Mechant_Rig'"));
	if (MeshContainer.Object)
	{
		mesh = GetMesh();
		mesh->SetSkeletalMesh(MeshContainer.Object);
		mesh->SetWorldScale3D(FVector(1.25f, 1.25f, 1.25f));
		mesh->SetWorldLocation(FVector(0.f, 0.f, -70.f));
		mesh->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
	}


	ProjectileArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileArrow"));
	ProjectileArrow->SetupAttachment(RootComponent);

	ProjectileLocationOrigin = CreateDefaultSubobject<USceneComponent>("ProjectileLocationOrigin");
	ProjectileLocationOrigin->SetupAttachment(ProjectileArrow);

	ProjectileLocation = CreateDefaultSubobject<USceneComponent>("ProjectileLocation");
	ProjectileLocation->SetupAttachment(ProjectileArrow);

	ProjectileLocation2 = CreateDefaultSubobject<USceneComponent>("ProjectileLocation2");
	ProjectileLocation2->SetupAttachment(ProjectileArrow);

	ProjectileLocation3 = CreateDefaultSubobject<USceneComponent>("ProjectileLocation3");
	ProjectileLocation3->SetupAttachment(ProjectileArrow);

}

// Called when the game starts or when spawned
void ACubeEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FireTimer += DeltaTime;
	if (FireTimer >= (1 / Settings.FireRate))
	{
		CanShoot = true;
	}
}

void ACubeEnemy::Shoot()
{
	FVector Direction = this->GetActorForwardVector();
	FVector Start = this->GetActorLocation() + (Direction * 100.0f);

	if (CanShoot)
	{
		if (Settings.ProjectileClass != nullptr)
		{
			SpawnProjectile();
		}
		else
		{
			HitScan(Start, Direction);
		}

		FireTimer = 0.0f;
		CanShoot = false;
	}
}

void ACubeEnemy::HitScan(FVector Start, FVector Direction)
{
	FHitResult Hit = FHitResult();

	FVector End = Start + (Direction * Settings.MaxDistance);

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);

	FVector HitLocation = Hit.bBlockingHit ? Hit.Location : End;

	DrawDebugSphere(GetWorld(), HitLocation, 10.f, 8, FColor::Red, false, 1.f);

}

void ACubeEnemy::SpawnProjectile()
{
	FActorSpawnParameters Params;

	FVector LocationOrigin = ProjectileLocationOrigin->GetComponentLocation();
	FVector Location = ProjectileLocation->GetComponentLocation();
	FVector Location2 = ProjectileLocation2->GetComponentLocation();
	FVector Location3 = ProjectileLocation3->GetComponentLocation();

	FVector Direction = this->GetActorForwardVector();
	FVector Direction2 = (Location2 - LocationOrigin) + this->GetActorForwardVector();
	FVector Direction3 = (Location3 - LocationOrigin) + this->GetActorForwardVector();

	Direction.Normalize();
	Direction2.Normalize();
	Direction3.Normalize();

	FRotator Rotation = Direction.Rotation();
	FRotator Rotation2 = Direction2.Rotation();
	FRotator Rotation3 = Direction3.Rotation();

	if (Settings.FireProjectileNbr == 1)
	{
		GetWorld()->SpawnActor<AFPS_Projectile>(Settings.ProjectileClass, Location, Rotation, Params);
	}
	else if (Settings.FireProjectileNbr == 2)
	{
		multipleProjectileSpawn.Add(Location);
		multipleProjectileSpawn.Add(Location2);

		multipleProjectileRotation.Add(Rotation);
		multipleProjectileRotation.Add(Rotation2);

		for (int i = 0; i < Settings.FireProjectileNbr; i++)
		{
			GetWorld()->SpawnActor<AFPS_Projectile>(Settings.ProjectileClass, multipleProjectileSpawn[i], multipleProjectileRotation[i], Params);
		}
		multipleProjectileSpawn.Empty();
		multipleProjectileRotation.Empty();
	}
	else
	{
		multipleProjectileSpawn.Add(Location);
		multipleProjectileSpawn.Add(Location2);
		multipleProjectileSpawn.Add(Location3);

		multipleProjectileRotation.Add(Rotation);
		multipleProjectileRotation.Add(Rotation2);
		multipleProjectileRotation.Add(Rotation3);

		for (int i = 0; i < Settings.FireProjectileNbr; i++)
		{
			GetWorld()->SpawnActor<AFPS_Projectile>(Settings.ProjectileClass, multipleProjectileSpawn[i], multipleProjectileRotation[i], Params);
		}
		multipleProjectileSpawn.Empty();
		multipleProjectileRotation.Empty();
	}

}

void ACubeEnemy::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void ACubeEnemy::SubDamage(int dmg)
{
	health -= dmg;
	if (health <= 0)
	{
		ACharacter* charA = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		APlayerCube* playerCube = Cast<APlayerCube>(charA);

		playerCube->SetScrap(scrapMetals);
		Destroy();
	}
}
