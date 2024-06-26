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

    if (!OverlappedActors.Contains(OtherActor))
    {
        OverlappedActors.Add(OtherActor);  // 액터를 추가하여 중복 오버랩을 방지

        UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());

        ApplyDamageToActors();
    }
}

void AGOKatnissUltimateProjectile::ApplyDamageToActors()
{
    TArray<AActor*> OverlappingActors;
    Sphere->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor != GetOwner())
        {
            UGameplayStatics::ApplyDamage(Actor, Damage, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
        }
    }
}
