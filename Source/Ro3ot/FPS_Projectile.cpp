// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_Projectile.h"

// Sets default values
AFPS_Projectile::AFPS_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");

	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AFPS_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->OnComponentHit.AddDynamic(this, &AFPS_Projectile::OnHit);

	FVector BaseVelocity = ProjectileDirection * ProjectileSpeed;
	ProjectileComp->SetVelocityInLocalSpace(BaseVelocity);
}

// Called every frame
void AFPS_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LifeTimer += DeltaTime;
	if (LifeTimer >= LifeTime)
	{
		LifeTimeEnded();
	}

}

void AFPS_Projectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void AFPS_Projectile::OnStop(const FHitResult& Hit)
{
	Destroy();
}

void AFPS_Projectile::LifeTimeEnded()
{
	Destroy();
}