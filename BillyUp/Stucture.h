// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCube.h"
#include "Stucture.generated.h"

UCLASS()
class BILLYUP_API AStucture : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStucture();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* WallRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* WallLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* Floor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* LineDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* BgWallRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* BgWallLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Box)
		class UBoxComponent* FloorBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Box)
		class UBoxComponent* BelowFloorBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Box)
		class UBoxComponent* AboveFloorBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Box)
		class UBoxComponent* ForWallBoxComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, Category = Blueprint)
		TSubclassOf<AStucture> BP_Stucture;
	
	void Spawn();
	void DestroyBall();

	bool IsUsedFloor = false;
	bool IsUsedBelow = false;
	bool IsUsedAbove = false;

public:
	APlayerCube* PlayerCube;

	UPROPERTY(EditAnywhere, Category = Property)
		float Speed = 250;

	UPROPERTY(EditAnywhere, Category = Property)
		int NumStucture = 0;
};
