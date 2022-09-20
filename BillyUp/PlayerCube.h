// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCube.generated.h"

UCLASS()
class BILLYUP_API APlayerCube : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCube();

	UPROPERTY(EditAnywhere , Category = Component)
		class UStaticMeshComponent* Mesh;

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		class USpringArmComponent* CameraBoom;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:

	void Jump();
	void MoveRight();
	void MoveLeft();
	void MoveDown();

	UPROPERTY(EditAnywhere, Category = Property)
		float Speed;



	float BeginLocationZ = 0;
public:
	UPROPERTY(EditAnywhere , Category = Rules)
		int LimitTryJump = 3;

	UFUNCTION(BlueprintPure)
		int GetScore();

	UFUNCTION(BlueprintPure)
		float GetSeconds();

	UFUNCTION(BlueprintPure)
		int GetTryJump();

	UFUNCTION(BlueprintPure)
		int GetScoreRecord();

	UFUNCTION(BlueprintPure)
		float GetSecondRecord();
	
	UPROPERTY(EditAnywhere, Category = Property)
		int HowManyStucture = 3;

	UPROPERTY(EditAnywhere, Category = Widget)
		TSubclassOf<UUserWidget> WBP_GameOver;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
		UParticleSystem* UpParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
		UParticleSystem* BounceParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
		UParticleSystem* DeathParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		USoundBase* PointSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		USoundBase* JumpSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
		USoundBase* DeadSound;

	UUserWidget* UWidget;

	FVector FirstStuctureLoc = FVector(80 , 0 , 470); //(X=80.000000,Y=0.000000,Z=470.000000)

	int TryJump = 0;
	int CounterStucture = 0;
	int Score = 0;
	
	float Seconds = 0;

	void ReturnJump();

	bool BeginInGame = false;

	void Dead();

	bool IsDead = false;


	int ScoreRecord;
	int SecondRecord;

	void SaveRecord(int ISlot , FString FSlot);
	void LoadRecord(int ISlot, FString FSlot);
	

	
};
