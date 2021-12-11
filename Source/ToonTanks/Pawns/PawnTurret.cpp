// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"

void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle,
		this, &APawnTurret::CheckFireRate, FireRate, true);

	PlayerPawnRef = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if( IsValid(PlayerPawnRef) && (ReturnDistanceToPlayer() <= FireRange) )
	{
		RotateTurretTowards(PlayerPawnRef->GetActorLocation());
	}
	//UE_LOG(LogTemp, Warning, TEXT("APawnTurret::%s is Ticking\t Time: %.2fs"), *GetName(), GetWorld()->RealTimeSeconds)
}

void APawnTurret::CheckFireRate()
{
	if( IsValid(PlayerPawnRef) && (ReturnDistanceToPlayer() <= FireRange) )
	{
		if( PlayerPawnRef->GetIsPlayerAlive() ) 
			Fire();

		//UE_LOG(LogTemp, Warning, TEXT("APawnTurret: 'Player in range'\t Time: %.2fs"), GetWorld()->RealTimeSeconds)
	}
}

float APawnTurret::ReturnDistanceToPlayer() const
{
	return IsValid(PlayerPawnRef) ?
		FVector::Distance(GetActorLocation(), PlayerPawnRef->GetActorLocation())
		: 0.f;
}

void APawnTurret::HandleDestruction()
{
	Super::HandleDestruction();

	Destroy();
}


