// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile/GOProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Physics/GOCollision.h"
#include "Interface/GOApplySkillInterface.h"

AGOProjectile::AGOProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(CCHANNEL_GOProjectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 500.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AGOProjectile::SetProjectileMaxSpeed(float Speed)
{
	ProjectileMovement->MaxSpeed = Speed;
}

void AGOProjectile::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("[Projectile]  AGOProjectile::BeginPlay : %s"), *GetName());

	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AGOProjectile::OnSphereOverlap);
}

void AGOProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || OtherActor == GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Projectile]  AGOProjectile::OnSphereOverlap return"));

		// Ignore collision if OtherActor is nullptr or if it is the owner of the projectile
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	//LoopingSoundComponent->Stop();

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	if (SweepResult.GetActor() != GetOwner())
	{
		if (IGOApplySkillInterface* GOApplySkillInterface = Cast<IGOApplySkillInterface>(SweepResult.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("[Projectile] OnSphereOverlap GetOwner() : %s"), *GetOwner()->GetName());

			GOApplySkillInterface->ApplySkillEffect(SweepResult.GetActor(), 10, GetOwner()); // 
		}
	}

	if (HasAuthority())
	{
		StartDestroyTimer();
	}
	else
	{
		bHit = true;
	}
}

void AGOProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		//LoopingSoundComponent->Stop();
	}
	UE_LOG(LogTemp, Warning, TEXT("[Projectile]  AGOProjectile::Destroyed"));

	Super::Destroyed();
}

void AGOProjectile::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer(
		DestroyTimer,
		this,
		&AGOProjectile::DestroyTimerFinished,
		DestroyTime
	);
}

void AGOProjectile::DestroyTimerFinished()
{
	Destroy();
}