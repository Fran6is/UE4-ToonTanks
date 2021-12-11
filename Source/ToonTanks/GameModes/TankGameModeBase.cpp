// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "ToonTanks/Controllers/PlayerControllerBase.h"

#include "Kismet/GameplayStatics.h"

void ATankGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
	
	if (DeadActor == PlayerTankRef)
	{
		PlayerTankRef->HandleDestruction();
		HandleGameOver(false);

		if ( IsValid(PlayerControllerRef) ) //Disable player controller
		{
			PlayerControllerRef->SetPlayerEnabledState(false);
		}
	}
	else if ( APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor) )
	{
		DestroyedTurret->HandleDestruction();

		if (--TargetTurrets == 0) //player won
			HandleGameOver(true);
	}

}

void ATankGameModeBase::HandleGameStart()
{
	TargetTurrets = GetTargetTurretCount();

	PlayerTankRef = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

	PlayerControllerRef = Cast<APlayerControllerBase>(
		UGameplayStatics::GetPlayerController(this, 0)
	);

	GameStart(); 

	if ( IsValid(PlayerControllerRef) ) 
	{
		PlayerControllerRef->SetPlayerEnabledState(false); // Disable tank input

		//Re-Enable Input in 'start delay' seconds time

		FTimerHandle PlayerEnableHandle;
		FTimerDelegate PlayerEnabledDelegate = FTimerDelegate::CreateUObject(
			PlayerControllerRef,
			&APlayerControllerBase::SetPlayerEnabledState,
			true //argument to pass to 'SetPlayerEnabledState'
		);
		
		GetWorld()->GetTimerManager().SetTimer(
			PlayerEnableHandle,
			PlayerEnabledDelegate,
			StartDelay + 1.5, //Why add 1.5? Check UI blueprints for countdown logic
			false
		);
	}

}

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
	GameOver(PlayerWon);
}

int32 ATankGameModeBase::GetTargetTurretCount() const
{
	TArray<AActor*> TurretActors;
	TurretActors.Reserve(3);

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		APawnTurret::StaticClass(),
		TurretActors
	);

	return TurretActors.Num();
}