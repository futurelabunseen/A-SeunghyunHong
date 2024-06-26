// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile/GOProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Physics/GOCollision.h"
#include "Share/EditorNames.h"

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
	ProjectileMovement->InitialSpeed = GOProjectile::PROJECTILE_INITIAL_SPEED;
	ProjectileMovement->MaxSpeed = GOProjectile::PROJECTILE_MAX_SPEED;
	ProjectileMovement->ProjectileGravityScale = GOProjectile::PROJECTILE_GRAVITY_SCALE;

	Damage = GOProjectile::PROJECTILE_BASE_DAMAGE; // Default damage value
}

//void AGOProjectile::OnSpawnFromPool_Implementation()
//{   
//	// Initialize or reset any required properties
//	SetActorHiddenInGame(false);
//	SetActorEnableCollision(true);
//	UE_LOG(LogTemp, Warning, TEXT("[Pool] OnSpawnFromPool_Implementation"));
//}
//
//void AGOProjectile::OnReturnToPool_Implementation()
//{
//	SetActorHiddenInGame(true);
//	SetActorEnableCollision(false);
//	UE_LOG(LogTemp, Warning, TEXT("[Pool] OnReturnToPool_Implementation"));
//}

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
		return;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	if (SweepResult.GetActor() != GetOwner())
	{
		UGameplayStatics::ApplyDamage(SweepResult.GetActor(), Damage, GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
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
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	}
	UE_LOG(LogTemp, Warning, TEXT("[Projectile]  AGOProjectile::Destroyed"));

	Super::Destroyed();
	//if (UObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
	//{
	//	PoolSubsystem->ReturnProjectile(this);
	//}
}

void AGOProjectile::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer(
		DestroyTimer,
		this,
		&AGOProjectile::DestroyTimerFinished,
		GOProjectile::PROJECTILE_DESTROY_DELAY_TIME
	);
}

void AGOProjectile::DestroyTimerFinished()
{
	Destroy();
	//if (UObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>())
	//{
	//	PoolSubsystem->ReturnProjectile(this);
	//}
	//else
	//{
	//	Destroy();
	//}
}