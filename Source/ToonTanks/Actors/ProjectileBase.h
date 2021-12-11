// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;

UCLASS()
class TOONTANKS_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

	void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& HitResult );

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;

	//Particle efffect: as component!
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleTrail = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MovementSpeed = 1500.f;

	//PARTICLE EFFECT!
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* HitParticle = nullptr;

	//SOUND EFFECT!
	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* HitSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* LaunchSound = nullptr;

	//CAMERA SHAKE class
	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UCameraShake> HitShake;
};
