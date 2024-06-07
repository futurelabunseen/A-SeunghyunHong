// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/GOKatnissUltimateProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Interface/GOApplySkillInterface.h"
#include "Components/SphereComponent.h"

AGOKatnissUltimateProjectile::AGOKatnissUltimateProjectile()
{
}

void AGOKatnissUltimateProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor == nullptr || OtherActor == GetOwner())
    {
        return;
    }

    UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());

    ApplyDamageToActors();

    //if (HasAuthority())
    //{
    //    StartDestroyTimer();
    //}
    //else
    //{
    //    bHit = true;
    //}
}

void AGOKatnissUltimateProjectile::ApplyDamageToActors()
{
    TArray<AActor*> OverlappingActors;
    Sphere->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor != GetOwner())
        {
            if (IGOApplySkillInterface* GOApplySkillInterface = Cast<IGOApplySkillInterface>(Actor))
            {
                GOApplySkillInterface->ApplySkillEffect(Actor, Damage, GetOwner());
            }
        }
    }
}