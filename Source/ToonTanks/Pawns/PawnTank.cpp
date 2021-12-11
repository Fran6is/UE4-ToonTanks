// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


APawnTank::APawnTank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->bInheritRoll = false;
	SpringArm->TargetArmLength = 600.f;
	SpringArm->SocketOffset = FVector(0, 0, 120.f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void APawnTank::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = GetController<APlayerController>();

}

void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("APawnTank::%s is Ticking\t Time: %.2fs"), *GetName(), GetWorld()->RealTimeSeconds)

	Rotate();
	Move();

	if (IsValid(PlayerControllerRef))
	{
		FHitResult HitResult;
		PlayerControllerRef->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility,
			false, 
			HitResult
		);

		//Necessary because a non-hit has an 'impact point' of <0,0,0>
		//which will make the turret rotate towards the world origin if no hit
		if (HitResult.bBlockingHit) 
			RotateTurretTowards(HitResult.ImpactPoint);

		//UE_LOG(LogTemp, Warning, TEXT("APawnTank::%s cursor hit location = %s \t Time: %.2fs"), *GetName(), *HitResult.ImpactPoint.ToString(), GetWorld()->RealTimeSeconds)
	}
}

void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &APawnTank::CalculateMoveInput);
	PlayerInputComponent->BindAxis(FName("Turn"),		 this, &APawnTank::CalculateRotateInput);
	PlayerInputComponent->BindAction(FName("Fire"), EInputEvent::IE_Pressed, this, &APawnTank::Fire);
}

void APawnTank::CalculateMoveInput(float Throw)
{
	MoveDirection.X = TranslationalSpeed * Throw * GetWorld()->DeltaTimeSeconds;

	//UE_LOG(LogTemp, Warning, TEXT("APawnTank::%s Move throw = %.2f \t Time: %.2fs"), *GetName(), Throw, GetWorld()->RealTimeSeconds)

}

void APawnTank::CalculateRotateInput(float Throw)
{
	float Angle = FMath::DegreesToRadians(RotationalSpeed) * Throw * GetWorld()->DeltaTimeSeconds;
	const FVector& AxisOfRotation = FVector::UpVector;

	RotationDirection.W = FMath::Cos(Angle);
	RotationDirection.X = FMath::Sin(Angle) * AxisOfRotation.X;
	RotationDirection.Y = FMath::Sin(Angle) * AxisOfRotation.Y;
	RotationDirection.Z = FMath::Sin(Angle) * AxisOfRotation.Z;

	//UE_LOG(LogTemp, Warning, TEXT("APawnTank::%s Move throw = %.2f \t Time: %.2fs"), *GetName(), Throw, GetWorld()->RealTimeSeconds)
}

void APawnTank::Move()
{
	AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate()
{
	AddActorLocalRotation(RotationDirection, true);
}

bool APawnTank::GetIsPlayerAlive() const
{
	return bIsPlayerAlive;
}

void APawnTank::HandleDestruction()
{
	Super::HandleDestruction(); //Play death animation / particle in parent class

	//By destroying pawn, we automatically / drastically 
	//jump to the scene camera which is undesireable.
	//Oh! and if you do choose this option, go to "AGamemodeBase::ActorDied()"
	//If a pawn is destroyed, so will it's player controller. So trying to 
	//access it afterwards, will crash the game. Even the 'IsValid()' check can't detect this
	//Destroy();  

	//Instead:
	//Hide Tank, tag as dead, and turn-off tick calls to improve performance
	bIsPlayerAlive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}
