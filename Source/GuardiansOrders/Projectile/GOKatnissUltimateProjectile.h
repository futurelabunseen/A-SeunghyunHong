// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/GOProjectile.h"
#include "GOKatnissUltimateProjectile.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API AGOKatnissUltimateProjectile : public AGOProjectile
{
	GENERATED_BODY()

public:
    AGOKatnissUltimateProjectile();

protected:
    virtual void OnSphereOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult) override;

    void ApplyDamageToActors();

private:
    TSet<AActor*> OverlappedActors;
};
