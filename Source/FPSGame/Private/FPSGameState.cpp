// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InsigatorPawn, bool bMissionComplete)
{
    for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
    {
        APawn* Pawn = It->Get();
        if(IsValid(Pawn) && Pawn->IsLocallyControlled())
        {
            Pawn->DisableInput(nullptr);
        }
    }
}
