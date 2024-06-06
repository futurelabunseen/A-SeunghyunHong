// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GOProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class GUARDIANSORDERS_API AGOProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AGOProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	void SetProjectileMaxSpeed(float Speed);

	// Add a property to specify the damage value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage;

protected:
	virtual void BeginPlay() override;
	void StartDestroyTimer();
	void DestroyTimerFinished();

	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

	bool bHit = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

	/*UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;*/

	FTimerHandle DestroyTimer;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 0.5f;
};
