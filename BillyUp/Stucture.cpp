// Fill out your copyright notice in the Description page of Project Settings.


#include "Stucture.h"
#include "PlayerCube.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


#define print(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

// Sets default values
AStucture::AStucture()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = GetRootComponent();

	WallRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall Right"));
	WallRight->SetupAttachment(RootComponent);
	WallRight->SetRelativeLocation(FVector(0.000000,500.000000,0.000000));
	WallRight->SetRelativeScale3D(FVector(8.5,0.500000,7.250000));

	WallLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall Left"));
	WallLeft->SetupAttachment(RootComponent);
	WallLeft->SetRelativeLocation(FVector(0.000000, -500.000000, 0.000000));
	WallLeft->SetRelativeScale3D(FVector(8.5, 0.500000, 7.250000));

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->SetupAttachment(RootComponent);

	BgWallRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BG Wall Right"));
	BgWallRight->SetupAttachment(RootComponent);
	BgWallRight->SetRelativeLocation(FVector(0.020616, 1391.906616,-0.008633));
	BgWallRight->SetRelativeScale3D(FVector(17.750000, 10.000000,1.000000));
	BgWallRight->SetRelativeRotation(FRotator(0.000608,270.000092,449.999969));

	BgWallLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BG Wall Left"));
	BgWallLeft->SetupAttachment(RootComponent);
	BgWallLeft->SetRelativeLocation(FVector(0.019993, -1386.358765, 0.020939));
	BgWallLeft->SetRelativeScale3D(FVector(17.750000,10.250000, 1.000000));
	BgWallLeft->SetRelativeRotation(FRotator(0.000608,270.000092,449.999969));

	ForWallBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("For Wall Box Component"));
	ForWallBoxComponent->SetupAttachment(Floor);
	ForWallBoxComponent->SetRelativeScale3D(FVector(0.375000,1.600000, 4.000000));
	ForWallBoxComponent->SetGenerateOverlapEvents(true);
	
	FloorBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Floor Box Component"));
	FloorBoxComponent->SetupAttachment(Floor);
	FloorBoxComponent->SetBoxExtent(FVector(48, 48, 32));
	FloorBoxComponent->SetGenerateOverlapEvents(true);

	BelowFloorBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Below Floor Box Component"));
	BelowFloorBoxComponent->SetupAttachment(Floor);
	BelowFloorBoxComponent->SetBoxExtent(FVector(150, 500, 100));
	BelowFloorBoxComponent->SetGenerateOverlapEvents(true);

	AboveFloorBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Above Floor Box Component"));
	AboveFloorBoxComponent->SetupAttachment(Floor);
	AboveFloorBoxComponent->SetBoxExtent(FVector(150, 500, 100));
	AboveFloorBoxComponent->SetGenerateOverlapEvents(true);

	LineDeath = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Line Death"));
	LineDeath->SetupAttachment(BelowFloorBoxComponent);
	LineDeath->SetRelativeScale3D(FVector(0.5 , 10.9 , 0.25));
	LineDeath->SetRelativeLocation(FVector(-114.401245, 0.000000, 101.007568));
	LineDeath->SetVisibility(false);
	
	FloorBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AStucture::OnBeginOverlap);
	BelowFloorBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AStucture::OnBeginOverlap);
	AboveFloorBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AStucture::OnBeginOverlap);
	ForWallBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AStucture::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AStucture::BeginPlay()
{
	Super::BeginPlay();

	FVector FloorLoc = Floor->GetComponentLocation();
	float RandomX = FloorLoc.Y + FMath::RandRange(-345, 345);

	Floor->SetWorldLocation(FVector(FloorLoc.X, RandomX ,FloorLoc.Z));

	Speed = FMath::RandRange(250, 450);

	TArray<AActor*> ReturnedActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerCube::StaticClass(), ReturnedActors);
	PlayerCube = nullptr;
	if (ReturnedActors.Num() > 0) 
	{
		PlayerCube = Cast<APlayerCube>(ReturnedActors[0]);
	}

	if (FMath::RandRange(0,1) == 1)
	{
		Speed *= -1;
	}

	if (PlayerCube)
	{
		if (NumStucture == 1 || NumStucture == 2)
		{
			Spawn();
		}
	}


}

// Called every frame
void AStucture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	float FloorLocY = Floor->GetRelativeLocation().Y  + (Speed * DeltaTime);

	FVector FloorLoc = FVector(Floor->GetRelativeLocation().X , FloorLocY , Floor->GetRelativeLocation().Z);

	Floor->SetRelativeLocation(FloorLoc);


}

void AStucture::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent == FloorBoxComponent)
	{
		APlayerCube* Player = Cast<APlayerCube>(OtherActor);
		if (Player)
		{
			if (!IsUsedFloor || IsUsedFloor && Player->TryJump != 0)
			{
				if (Player)
				{
					PlayerCube->Score = NumStucture;
					if (!IsUsedFloor)
					{
						Spawn();
						IsUsedFloor = true;

						if (PlayerCube->PointSound)
						{
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerCube->PointSound,PlayerCube->GetActorLocation(),PlayerCube->GetActorRotation(), 0.5f, FMath::RandRange(1.f, 1.3f));
						}
					}

					if (PlayerCube->BounceParticle && !PlayerCube->IsDead)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PlayerCube->BounceParticle, PlayerCube->GetActorLocation(), FRotator(0, 0, 0));
					}
					Player->TryJump = 0;
					LineDeath->SetVisibility(true);
				}
			}
		}
	}
	else if (OverlappedComponent == BelowFloorBoxComponent)
	{
		APlayerCube* Player = Cast<APlayerCube>(OtherActor);
		if (IsUsedFloor)
		{
			if (Player)
			{
				FTimerHandle DestroyTimer;
				GetWorldTimerManager().SetTimer(DestroyTimer, this, &AStucture::DestroyBall, .1f, true);			
			}
		}
	}
	else if (OverlappedComponent == AboveFloorBoxComponent)
	{
		APlayerCube* Player = Cast<APlayerCube>(OtherActor);
		if (Player)
		{
			Player->BeginInGame = true;
			Destroy();
		}
	}
	else if (OverlappedComponent == ForWallBoxComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current values are: vector %s"), *GetActorLocation().ToString());
		if (OtherComp == WallLeft || OtherComp == WallRight)
		{
			Speed *= -1;
		}
	}
}

void AStucture::Spawn()
{
	if (PlayerCube)
	{
		for (int i = 0; i < 2; i++)
		{
			FVector Location = PlayerCube->FirstStuctureLoc;
			Location.Z += 725.0f * PlayerCube->HowManyStucture;
			PlayerCube->HowManyStucture++;
			AStucture* Stucture = GetWorld()->SpawnActor<AStucture>(BP_Stucture, FTransform(FRotator(0,0,0), Location, FVector(1, 1, 1)));
			Stucture->NumStucture = PlayerCube->HowManyStucture;
		}
	}
}

void AStucture::DestroyBall()
{
	if (PlayerCube->Mesh)
	{
		if (!PlayerCube->IsDead)
		{

			PlayerCube->Mesh->DestroyComponent();

			PlayerCube->Dead();
			if (PlayerCube->DeathParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PlayerCube->DeathParticle, PlayerCube->GetActorLocation(), FRotator(0, 0, 0));
			}
			if (PlayerCube->DeadSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerCube->DeadSound, PlayerCube->GetActorLocation(), PlayerCube->GetActorRotation(), 0.5f, FMath::RandRange(0.5f, .7f));
			}
			PlayerCube->SaveRecord(0,"0");
			PlayerCube->LoadRecord(0, "0");
		}
		PlayerCube->IsDead = true;
	}
}


