// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "FPS_Projectile.h"
#include "PlayerCube.generated.h"

UENUM(BlueprintType)
enum EShotType
{
	Semi = 0,
	Auto
};

USTRUCT(BlueprintType)
struct FShootSettings
{
	GENERATED_USTRUCT_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TEnumAsByte<EShotType> ShotType = Auto;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float MaxDistance = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf <AFPS_Projectile> ProjectileClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float FireRate = 4.0f;
};

UCLASS()
class RO3OT_API APlayerCube : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCube();
	void Shoot(bool TriggerIsPulled);
	void SpawnProjectile(FVector Start, FVector Direction);
	void HitScan(FVector Start, FVector Direction);

	UFUNCTION(BlueprintCallable, CallInEditor)
		float SetScrap(float scrapMetals) { return scrapAmount += scrapMetals; };

	UFUNCTION(BlueprintCallable, CallInEditor)
		float GetScrap() { return scrapAmount; };

	UFUNCTION(BlueprintCallable, CallInEditor)
		int GetHp() { return health; };

	UFUNCTION(BlueprintImplementableEvent)
		void ManageBerserk();

	void SubDamage(int dmg);
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FShootSettings Settings = FShootSettings();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		bool invulnerabiltyFrame = false;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		USceneComponent* ProjectileLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		float InteractionMaxDistance = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		UArrowComponent* ProjectileArrow;

	int health;

private:

	bool CanShoot = false, TriggerHasBeenReleased = true;

	float scrapAmount = 0.f, FireTimer = 0.f;

	UFUNCTION()
		void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
