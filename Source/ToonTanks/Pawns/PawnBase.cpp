// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "ToonTanks/Actors/ProjectileBase.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void APawnBase::RotateTurretTowards(const FVector& Target)
{
	TurretMesh->SetWorldRotation( 
		FRotator(0.f, (Target - TurretMesh->GetComponentLocation()).Rotation().Yaw, 0),
		true
	);
}

void APawnBase::Fire()
{
	//why this check? wouldn't the spawn function check for this
	//Maybe. Which will also mean that if we don't provide a class for it to spawn, the
	//return will be a nullptr. which will crash our program since
	//we're immediately setting it's owner and not doing a null check

	if (ProjectileClass) 
	{ 
		GetWorld()->SpawnActor<AProjectileBase>(
			ProjectileClass,
			ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation()
		)->SetOwner(this);
	}
}

void APawnBase::HandleDestruction()
{

	UGameplayStatics::SpawnEmitterAtLocation(
		this,
		DeathParticle,
		GetActorLocation()
	);

	//Different ways to play sound
		//https://answers.unrealengine.com/questions/428907/different-ways-to-play-a-sound.html
	UGameplayStatics::SpawnSoundAtLocation(
		this,
		DeathSound,
		GetActorLocation()
	);

	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(
		DeathShake
	);
}

