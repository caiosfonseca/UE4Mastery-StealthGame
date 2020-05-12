// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FPSCharacter.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	OverlapComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(Size));

	RootComponent = OverlapComp;

	OverlapComp->SetHiddenInGame(false);

	ArrowPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowPlane"));
	ArrowPlane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ArrowPlane->SetupAttachment(RootComponent);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::HandleOverlap);
	OverlapComp->OnComponentEndOverlap.AddDynamic(this, &AFPSLaunchPad::HandleEndOverlap);

}

void AFPSLaunchPad::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* Player = Cast<AFPSCharacter>(OtherActor);
	FVector LaunchVelocity =  (GetActorRotation() + LaunchRotation).Vector() * Magnitude;
	if(IsValid(Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player overlap"));
		Player->LaunchCharacter(LaunchVelocity, true, true);
		UGameplayStatics::PlaySound2D(this, LaunchSound);
		PlayEffects();
	}
	else
	{
		if( IsValid(OtherComp) && OtherComp->IsSimulatingPhysics() )
		{
			UE_LOG(LogTemp, Warning, TEXT("overlapped component"));
			OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
			UGameplayStatics::PlaySound2D(this, LaunchSound);
			PlayEffects();
		}
	}
}

void AFPSLaunchPad::HandleEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(IsValid(FxSystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("Deactivating FX System"));
		FxSystem->DeactivateSystem();
	}
}

void AFPSLaunchPad::PlayEffects()
{
	if(IsValid(FxSystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("Activating FX System"));
		FxSystem->ActivateSystem();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating FX System"));
		FxSystem = UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFX, GetActorLocation());
	}
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSLaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

