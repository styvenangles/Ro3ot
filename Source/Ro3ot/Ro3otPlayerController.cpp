// Fill out your copyright notice in the Description page of Project Settings.


#include "Ro3otPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"

#pragma region OVERRIDES
ARo3otPlayerController::ARo3otPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ARo3otPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bInputPressed)
	{
		FollowTime += DeltaTime;

		// Look for the touch location
		FVector HitLocation = FVector::ZeroVector;
		FHitResult Hit;
		if (bIsTouch)
		{
			GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility, true, Hit);
		}
		else
		{
			GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		}
		HitLocation = Hit.Location;

		// Direct the Pawn towards that location
		APawn* const MyPawn = GetPawn();
		if (MyPawn)
		{
			FVector WorldDirection = (HitLocation - MyPawn->GetActorLocation()).GetSafeNormal();
			MyPawn->AddMovementInput(WorldDirection, 1.f, false);
		}
	}
	else
	{
		FollowTime = 0.f;
	}
}

void ARo3otPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ARo3otPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ARo3otPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ARo3otPlayerController::OnTouchPressed);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &ARo3otPlayerController::OnTouchReleased);

}

#pragma endregion OVERRIDES

#pragma region INPUT_SYSTEM

void ARo3otPlayerController::OnSetDestinationPressed()
{
	// We flag that the input is being pressed
	bInputPressed = true;
	// Just in case the character was moving because of a previous short press we stop it
	StopMovement();
}

void ARo3otPlayerController::OnSetDestinationReleased()
{
	// Player is no longer pressing the input
	bInputPressed = false;

	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		FVector HitLocation = FVector::ZeroVector;
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		HitLocation = Hit.Location;

		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitLocation);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, HitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);

		//GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Orange, FString::Printf(TEXT("Hit : %s"), *Hit.GetActor()->GetName()));

		selectedActor = Hit.GetActor();
	}
}

void ARo3otPlayerController::OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	bIsTouch = true;
	OnSetDestinationPressed();
}

void ARo3otPlayerController::OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

#pragma region INPUT_SYSTEM

#pragma region INTERFACE_SYSTEM
void ARo3otPlayerController::TargetEnemy_Implementation(AActor* Enemy, FVector Location, bool isPlayer)
{
}

void ARo3otPlayerController::InRangeEnemy_Implementation(AActor* Enemy, bool isPlayer)
{
	// Find character in world
	ACharacter* robotCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	robotPC = Cast<ARo3otCharacter>(robotCharacter);
	APawn* const MyPawn = GetPawn();
	if (Enemy != nullptr && Enemy == selectedActor)
	{
		robotPC->isInRangeToAttack = true;
		robotPC->targetActor = Enemy;
		StopMovement();
	}
	else if (Enemy != nullptr && Enemy->ActorHasTag("Enemy") && !bInputPressed)
	{
		robotPC->isInRangeToAttack = true;
		robotPC->targetActor = Enemy;
	}
	else
	{
		robotPC->isInRangeToAttack = false;
		robotPC->targetActor = nullptr;
	}
}


#pragma region INTERFACE_SYSTEM