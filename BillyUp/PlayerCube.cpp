// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCube.h"
#include "Components/PrimitiveComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "ParticleDefinitions.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "SaveRecord.h"

#define print(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

// Sets default values
APlayerCube::APlayerCube()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm Component"));

	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 900.f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	CameraComponent->SetupAttachment(CameraBoom);
	CameraComponent->bUsePawnControlRotation = false;


	RootComponent = GetRootComponent();
	
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);
}

// Called to bind functionality to input
void APlayerCube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCube::Jump);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &APlayerCube::MoveRight);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &APlayerCube::MoveLeft);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &APlayerCube::MoveDown);

}


// Called when the game starts or when spawned
void APlayerCube::BeginPlay()
{

	Super::BeginPlay();
	BeginLocationZ = FMath::RoundFromZero(Mesh->GetComponentLocation().Z);
	
	LoadRecord(0, "0");
}

// Called every frame
void APlayerCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Mesh)
	{
		Mesh->SetRelativeLocation(FVector(0, Mesh->GetRelativeLocation().Y, Mesh->GetRelativeLocation().Z));
		RootComponent->SetRelativeLocation(Mesh->GetRelativeLocation());

		if (Score == 0 && FMath::RoundFromZero(Mesh->GetComponentLocation().Z) == BeginLocationZ && TryJump == LimitTryJump)
		{
			TryJump = 0;
		}
	}
	
	Seconds = GetWorld()->GetTimeSeconds();
}


void APlayerCube::Jump()
{
	if (TryJump < LimitTryJump)
	{
		if (Mesh)
		{
#if UE_BUILD_DEVELOPMENT
			Mesh->AddForce(FVector(0, 0, (Speed)));
#endif
#if UE_BUILD_SHIPPING
		Mesh->AddForce(FVector(0,0 , (Speed * 2)));
#endif
		}
		++TryJump;

		if (JumpSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), JumpSound, GetActorLocation(), GetActorRotation(), 0.1f, FMath::RandRange(1.f, 1.3f));
		}


		if (UpParticle && !IsDead)
		{
			if (Mesh)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UpParticle, Mesh->GetComponentLocation(), FRotator(0, 0, 0));
			}
		}
	}
}

void APlayerCube::MoveRight()
{
	if (Mesh)
	{
#if UE_BUILD_DEVELOPMENT
		Mesh->AddForce(FVector(0, (-Speed) / 1.5, 0));
#endif
#if UE_BUILD_SHIPPING
		Mesh->AddForce(FVector(0, (-Speed * 2) / 1.5, 0));
#endif
	}
}

void APlayerCube::MoveLeft()
{

	if (Mesh)
	{
#if UE_BUILD_DEVELOPMENT
		Mesh->AddForce(FVector(0, (Speed) / 1.5, 0));
#endif
#if UE_BUILD_SHIPPING
		Mesh->AddForce(FVector(0, (Speed * 2) / 1.5, 0));
#endif
		
	}
}

void APlayerCube::MoveDown()
{
	if (Mesh)
	{
#if UE_BUILD_DEVELOPMENT
		Mesh->AddForce(FVector(0, 0, (-Speed)));
#endif
#if UE_BUILD_SHIPPING
		Mesh->AddForce(FVector(0, 0, (-Speed * 2)));
#endif
		
	}
}

int APlayerCube::GetScore()
{
	return Score;
}

float APlayerCube::GetSeconds()
{
	return Seconds;
}

int APlayerCube::GetTryJump()
{
	return LimitTryJump - TryJump;
}

int APlayerCube::GetScoreRecord()
{
	return ScoreRecord;
}

float APlayerCube::GetSecondRecord()
{
	return SecondRecord;
}

void APlayerCube::ReturnJump()
{
	TryJump = 0;
}

void APlayerCube::Dead()
{
	if (WBP_GameOver)
	{
		UWidget = CreateWidget<UUserWidget>(GetWorld(), WBP_GameOver);
		if (UWidget)
		{
			UWidget->AddToViewport();
		}
	}
}

void APlayerCube::SaveRecord(int ISlot, FString FSlot)
{
	USaveRecord* SaveRecord = Cast<USaveRecord>(UGameplayStatics::CreateSaveGameObject(USaveRecord::StaticClass()));

	if (SaveRecord)
	{
		if (Score > ScoreRecord)
		{
			print("dd");
			SaveRecord->Score = Score;
			SaveRecord->Seconds = Seconds;
		}
		else if (ScoreRecord == Score)
		{
			print("aa");
			SaveRecord->Score = Score;

			if (Seconds > ScoreRecord)
			{
				SaveRecord->Seconds = Seconds;
			}	
		}
		else
		{
			SaveRecord->Score = ScoreRecord;
			SaveRecord->Seconds = SecondRecord;
		}
		UGameplayStatics::SaveGameToSlot(SaveRecord, FSlot, ISlot);
	}
}

void APlayerCube::LoadRecord(int ISlot, FString FSlot)
{
	USaveRecord* SaveRecord = Cast<USaveRecord>(UGameplayStatics::CreateSaveGameObject(USaveRecord::StaticClass()));
	if (SaveRecord)
	{
		SaveRecord = Cast <USaveRecord>(UGameplayStatics::LoadGameFromSlot(FSlot, ISlot));

		if (SaveRecord)
		{
			ScoreRecord = SaveRecord->Score;
			SecondRecord = SaveRecord->Seconds;
		}
	}
}
