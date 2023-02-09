// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPS_Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "CubeEnemy.generated.h"

USTRUCT(BlueprintType)
struct FEnemyShootSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float FireRate = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		int FireProjectileNbr = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float MaxDistance = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf <AFPS_Projectile> ProjectileClass = nullptr;
};

UCLASS()
class RO3OT_API ACubeEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACubeEnemy();

	UFUNCTION(BlueprintCallable)
		void Shoot();

	UFUNCTION(BlueprintCallable, CallInEditor)
		float SetScrap(float scrapIn) { return scrapMetals += scrapIn; };

	UFUNCTION(BlueprintCallable, CallInEditor)
		float GetScrap() { return scrapMetals; };

	UFUNCTION(BlueprintCallable, CallInEditor)
		int GetHp() { return health; };

	UFUNCTION(BlueprintCallable)
		void SubDamage(int dmg);

	void SpawnProjectile();
	void HitScan(FVector Start, FVector Direction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		USceneComponent* ProjectileLocationOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		USceneComponent* ProjectileLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		USceneComponent* ProjectileLocation2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		USceneComponent* ProjectileLocation3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		FEnemyShootSettings Settings = FEnemyShootSettings();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		float InteractionMaxDistance = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		UArrowComponent* ProjectileArrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		TArray<FVector> multipleProjectileSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		TArray<FRotator> multipleProjectileRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		bool CanShoot = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	int health;
	

private:
	float FireTimer = 0.0f;
	AFPS_Projectile* projectile = nullptr;

	float scrapMetals = 0.f;

	UFUNCTION()
		void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
