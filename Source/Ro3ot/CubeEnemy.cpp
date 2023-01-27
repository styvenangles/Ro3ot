// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeEnemy.h"

// Sets default values
ACubeEnemy::ACubeEnemy()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACubeEnemy::OnHit);
	health = 3;

	USkeletalMeshComponent* mesh;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshContainer(TEXT("/Script/Engine.SkeletalMesh'/Engine/EngineMeshes/SkeletalCube.SkeletalCube'"));
	if (MeshContainer.Object)
	{
		mesh = GetMesh();
		mesh->SetSkeletalMesh(MeshContainer.Object);
		mesh->SetWorldScale3D(FVector(2.f, 2.f, 6.f));
		mesh->SetWorldLocation(FVector(0.f, 0.f, -70.f));
	}


	ProjectileArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileArrow"));
	ProjectileArrow->SetupAttachment(RootComponent);

	ProjectileLocation = CreateDefaultSubobject<USceneComponent>("ProjectileLocation");
	ProjectileLocation->SetupAttachment(ProjectileArrow);
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
			SpawnProjectile(Direction);
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

void ACubeEnemy::SpawnProjectile(FVector Direction)
{
	FActorSpawnParameters Params;

	FVector Location = ProjectileLocation->GetComponentLocation();
	FRotator Rotation = Direction.Rotation();

	GetWorld()->SpawnActor<AFPS_Projectile>(Settings.ProjectileClass, Location, Rotation, Params);
}

void ACubeEnemy::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->GetClass() == Settings.ProjectileClass)
	{
		AFPS_Projectile* projectileInstance = Cast<AFPS_Projectile>(OtherActor);
		SubDamage(projectileInstance->Damage);
	}
}

void ACubeEnemy::SubDamage(int dmg)
{
	health -= dmg;
	if (health <= 0)
	{
		Destroy();
	}
}
