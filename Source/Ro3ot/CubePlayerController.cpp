// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePlayerController.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ACubePlayerController::ACubePlayerController()
{
	DefaultMouseCursor = EMouseCursor::Default;
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ACubePlayerController::BeginPlay()
{
	Super::BeginPlay();
	Cube = Cast<APlayerCube>(GetPawn());
}


void ACubePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Vertical", this, &ACubePlayerController::MoveVertical);
	InputComponent->BindAxis("Horizontal", this, &ACubePlayerController::MoveHorizontal);

	InputComponent->BindAction<FBooleanDelegate>("Shoot", EInputEvent::IE_Pressed, this, &ACubePlayerController::Shoot, true);
	InputComponent->BindAction<FBooleanDelegate>("Shoot", EInputEvent::IE_Released, this, &ACubePlayerController::Shoot, false);
}

void ACubePlayerController::MoveHorizontal(float value)
{
	/*if (Cube != nullptr) {
		USpringArmComponent* cam = Cube->CameraBoom;
		FVector Direction = cam->GetRightVector();
		Cube->AddMovementInput(Direction, value);
	}*/
}

void ACubePlayerController::MoveVertical(float value)
{
	/*if (Cube != nullptr)
	{
		USpringArmComponent* cam = Cube->CameraBoom;
		FVector Direction = cam->GetForwardVector();
		Cube->AddMovementInput(Direction, value);
	}*/
}

void ACubePlayerController::Shoot(bool TriggerIsPulled)
{
	Cube->Shoot(TriggerIsPulled);
}
