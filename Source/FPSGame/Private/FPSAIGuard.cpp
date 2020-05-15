// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "FPSGameMode.h"
#include "DrawDebugHelpers.h"
#include "UnrealNetwork.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();

	if(bPatrol)
	{
		MoveToNextPatrolPoint();
	}
	
}

void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if(GuardState == EAIState::Alerted)
	{
		return;
	}
	DrawDebugSphere(GetWorld(), Location, 32.f, 12, FColor::Green, false, 10.f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.f;
	NewLookAt.Roll = 0.f;
	
	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);	
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.f);
	
	SetGuardState(EAIState::Suspicious);

	AController* AiController = GetController();
	if(IsValid(AiController))
	{
		AiController->StopMovement();
	}
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if(!IsValid(SeenPawn))
	{
		return;
	}
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if(IsValid(GM))
	{
		GM->CompleteMission(SeenPawn, false);
	}

	SetGuardState(EAIState::Alerted);
	
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Red, false, 10.f);

	AController* AiController = GetController();
	if(IsValid(AiController))
	{
		AiController->StopMovement();
	}

}

void AFPSAIGuard::ResetOrientation()
{
	if(GuardState == EAIState::Alerted)
	{
		return;
	}
	
	SetActorRotation(OriginalRotation);

	SetGuardState(EAIState::Idle);

	if(bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if(GuardState == NewState)
	{
		return;
	}


	GuardState = NewState;
	OnRep_GuardState();
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
	if(!IsValid(CurrentPatrolPoint))
	{
		CurrentPatrolPoint = PatrolPoints[0];
	}
	else
	{
		int32 Index = PatrolPoints.Find(CurrentPatrolPoint);
		Index = (Index + 1) % (PatrolPoints.Num());
		CurrentPatrolPoint = PatrolPoints[Index];
	}

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsValid(CurrentPatrolPoint))
	{
		FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		float DistanceToGoal = Delta.Size();

		if(DistanceToGoal < 75)
		{
			MoveToNextPatrolPoint();
		}
	}

}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty> &  OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}
