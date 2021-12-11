// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameModeBase.generated.h"

class APawnTank;
class APawnTurret;
class APlayerControllerBase;

UCLASS()
class TOONTANKS_API ATankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* DeadActor);

protected:
	void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool PlayerWon);

private:
	void HandleGameStart();
	void HandleGameOver(bool PlayerWon);

	int32 GetTargetTurretCount() const;

protected:
	//'BlueprintReadOnly' can't be used with a private specifier: either make protected or  meta = (AllowPrivateAccess = "true")
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Loop") 
	int32 StartDelay = 3;

private:
	
	APawnTank* PlayerTankRef = nullptr;

	int32 TargetTurrets = 0;

	APlayerControllerBase* PlayerControllerRef = nullptr;
};
