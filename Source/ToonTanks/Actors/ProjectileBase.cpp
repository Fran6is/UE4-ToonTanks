// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Particles/ParticleSystemComponent.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed     = MovementSpeed;

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	ParticleTrail->SetupAttachment(RootComponent);

	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("Projectile base ticking"))
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& HitResult)
{
	AActor* MyOwner = GetOwner();

	if (!MyOwner) return;

	if (OtherActor && (OtherActor != this) && (OtherActor != MyOwner) )
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			Damage,
			MyOwner->GetInstigatorController(),
			this,
			DamageType
		);

		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			HitParticle,
			GetActorLocation()
		);

		//Different ways to play sound
		//https://answers.unrealengine.com/questions/428907/different-ways-to-play-a-sound.html
		UGameplayStatics::PlaySoundAtLocation(
			this, 
			HitSound, 
			GetActorLocation()
		);

		GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(
			HitShake
		);


		//UE_LOG(LogTemp, Warning, TEXT("Projectile hits %s"), *OtherActor->GetName())

		Destroy();
	}

}

