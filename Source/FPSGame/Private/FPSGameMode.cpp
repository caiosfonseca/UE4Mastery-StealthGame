// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "FPSGameState.h"
#include "FPSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	GameStateClass = AFPSGameState::StaticClass();
	PlayerControllerClass = AFPSPlayerController::StaticClass();
	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
	if(IsValid(InstigatorPawn))
	{

		if(SpectatingViewpointClass)
		{
			AActor* NewViewTarget = UGameplayStatics::GetActorOfClass(this, SpectatingViewpointClass);


			for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
			{
				APlayerController* PC = Cast<APlayerController>(It->Get());
				if(IsValid(PC))
				{
					PC->SetViewTargetWithBlend(NewViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}	
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spectating Viewpoint Class is null, please update gamemode class with a valid class. Cannot change spectating view target on mission complete "));
		}
	}

	AFPSGameState* GS = GetGameState<AFPSGameState>();
	if(GS)
	{
		GS->MulticastOnMissionComplete(InstigatorPawn, bMissionSuccess);
	}
	
	OnMissionCompleted(InstigatorPawn, bMissionSuccess);
}
