// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

class UBoxComponent;

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
        UBoxComponent* OverlapComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
        UStaticMeshComponent* ArrowPlane;

	UPROPERTY(VisibleAnywhere, Category = "Components")
        UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Control Variables")
	FRotator LaunchRotation = FRotator(60.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Control Variables")
		float Magnitude = 200.0f;
	
	UPROPERTY(EditAnywhere, Category = "Control Variables")
        float Size = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
        USoundBase* LaunchSound;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
        UParticleSystem* LaunchFX;

	class UParticleSystemComponent* FxSystem;

	UFUNCTION()
        void HandleOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent,	AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void PlayEffects();
};
