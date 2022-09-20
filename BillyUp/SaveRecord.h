// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveRecord.generated.h"

/**
 * 
 */
UCLASS()
class BILLYUP_API USaveRecord : public USaveGame
{
	GENERATED_BODY()
	

public:

	USaveRecord();

	UPROPERTY(EditAnywhere)
		int Score;

	UPROPERTY(EditAnywhere)
		float Seconds;
};
