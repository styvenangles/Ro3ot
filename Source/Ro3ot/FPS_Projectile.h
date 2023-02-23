// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FPS_Projectile.generated.h"

USTRUCT(BlueprintType)
struct FProjectileSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		TArray<TSubclassOf <AActor>> FriendlyClass = { nullptr , nullptr, nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		TArray<TSubclassOf <AActor>> EnemyClass = { nullptr, nullptr, nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf <AActor> ParentClass = nullptr;
};

UCLASS()
class RO3OT_API AFPS_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPS_Projectile();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		FVector ProjectileDirection = FVector(1, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float ProjectileSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float LifeTime = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
		int Damage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		FProjectileSettings PSettings = FProjectileSettings();

	UFUNCTION()
		void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnStop(const FHitResult& Hit);

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		UProjectileMovementComponent* ProjectileComp;


private:
	float LifeTimer = 0.0f;

	void LifeTimeEnded();
};
