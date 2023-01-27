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

	UFUNCTION(BlueprintCallable)
		void SubDamage(int dmg);

	void SpawnProjectile(FVector Direction);
	void HitScan(FVector Start, FVector Direction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		USceneComponent* ProjectileLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		FEnemyShootSettings Settings = FEnemyShootSettings();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		float InteractionMaxDistance = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		UArrowComponent* ProjectileArrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		bool CanShoot = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

private:
	int health;
	float FireTimer = 0.0f;
	AFPS_Projectile* projectile = nullptr;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};