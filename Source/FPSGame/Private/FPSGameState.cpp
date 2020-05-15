// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "FPSPlayerController.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InsigatorPawn, bool bMissionComplete)
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
    {
        AFPSPlayerController* PC = Cast<AFPSPlayerController>(It->Get());

        if(IsValid(PC) && PC->IsLocalController())
        {
            PC->OnMissionCompleted(InsigatorPawn, bMissionComplete);

            APawn* MyPawn = PC->GetPawn();
            if(IsValid(MyPawn))
            {
                MyPawn->DisableInput(nullptr);        
            }
        }
    }
}
