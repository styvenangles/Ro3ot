// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePlayerController.h"

ACubePlayerController::ACubePlayerController()
{

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

	InputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &ACubePlayerController::Shoot);
}

void ACubePlayerController::MoveHorizontal(float value)
{
	if (Cube != nullptr) {
		FVector Direction = Cube->GetActorRightVector();
		Cube->AddMovementInput(Direction, value);
	}
}

void ACubePlayerController::MoveVertical(float value)
{
	if (Cube != nullptr)
	{
		FVector Direction = Cube->GetActorForwardVector();
		Cube->AddMovementInput(Direction, value);
	}
}

void ACubePlayerController::Shoot()
{
	Cube->Shoot();
}
