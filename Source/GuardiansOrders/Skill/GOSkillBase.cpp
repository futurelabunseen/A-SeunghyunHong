// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillBase.h"
#include "Net/UnrealNetwork.h"
#include "GameData/GOGameSubsystem.h"
#include "Subsystems/SubsystemCollection.h"
#include "Kismet/GameplayStatics.h"
#include "GameData/GOCharacterData.h"
#include "GameData/GOCharacterStat.h"
#include "Character/GOCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Physics/GOCollision.h"
#include "Share/EGOSkill.h"

UGOSkillBase::UGOSkillBase()
{
	bTickable = true;
	bTickableWhenPaused = false;
}

void UGOSkillBase::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOSkillBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UGOSkillBase::Tick(float DeltaTime)
{
	if (bIsCasting == true)
	{
		CoolDownTimer -= DeltaTime;
		if (CoolDownTimer <= 0.0f)
		{
			FinishCast();
		}
	}
}

bool UGOSkillBase::IsTickable() const
{
	return bTickable;
}

bool UGOSkillBase::IsTickableInEditor() const
{
	return bTickable;
}

bool UGOSkillBase::IsTickableWhenPaused() const
{
	return bTickableWhenPaused;
}

TStatId UGOSkillBase::GetStatId() const
{
	return TStatId();
}

UWorld* UGOSkillBase::GetWorld() const
{
	return GetOuter()->GetWorld();
}

void UGOSkillBase::SetSkillOwner(AActor* NewOwner)
{
	SkillOwnerCharacter = NewOwner;
}

void UGOSkillBase::InitializeSkill(FName InSkillName)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();
	if (GOGameInstance)
	{
		FGOSkillData* SkillDataRow = GOGameInstance->GetSkillData(InSkillName);
		SkillData = *SkillDataRow;
		FGOSkillStat* SkillStatRow = GOGameInstance->GetSkillStatData(SkillDataRow->SkillStatName);
		if (SkillStatRow)
		{
			SkillStat = *SkillStatRow;
		}
	}
}

void UGOSkillBase::SpawnParticleAtLocation(FVector Location)
{
	if (SkillData.SkillEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SkillData.SkillEffect, Location);
	}
}

void UGOSkillBase::SpawnParticleAtActor(AActor* Actor)
{
	if (Actor && SkillData.SkillEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(SkillData.SkillEffect, Actor->GetRootComponent(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
	}
}

void UGOSkillBase::SpawnParticleAroundActor(AActor* Actor, float Radius)
{
	if (Actor && SkillData.SkillEffect)
	{
		FVector Location = Actor->GetActorLocation();
		FBoxSphereBounds SphereBounds(Location, FVector(Radius), Radius);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SkillData.SkillEffect, Location, FRotator::ZeroRotator, FVector(Radius));
	}
}

void UGOSkillBase::StartCast()
{
	bIsCasting = true;
	SetIsOnCoolTime(true);

	SetCoolDownTimer();
	CoolDownTime = CoolDownTimer;

	if (bIsCasting == true)
		UGOSkillBaseFIsOnCooldown.Broadcast(GetIsOnCoolTime());  // 쿨다운 시작 시 즉시 UI 업데이트
}

void UGOSkillBase::UpdateCast(float DeltaTime)
{
	if (CoolDownTime >= CoolDownTimer)
	{
		CoolDownTimer -= DeltaTime;
	}
	if (CoolDownTimer < 0)
	{
		FinishCast();
	}
}

void UGOSkillBase::ActivateSkill() // UGOSkillCastComponent::OnUpdateCast 에서 불림
{
	// 스킬 효과 발동 로직, 예: 대미지 처리, 상태 효과 적용 등
	ExecuteSkill(GetSkillCollisionType());
}

void UGOSkillBase::FinishCast()
{
	bIsCasting = false;
	bIsCastable = false;
	SetCoolDownTimer();
	SetIsOnCoolTime(false);
}

void UGOSkillBase::InterruptedCast()
{

}

void UGOSkillBase::ActivateEffect()
{

}

void UGOSkillBase::HandleSkillTrigger()
{
	switch (GetSkillTriggerType())
	{
	case ESkillTriggerType::Target:
		break;
	case ESkillTriggerType::NonTargetDirection:
		break;
	case ESkillTriggerType::NonTargetRange:
		break;
	case ESkillTriggerType::AutoTargetRadius:
	{
		float Radius = GetAutoDetectionRadius();
		AGOCharacterBase* ClosestTarget = DetectClosestTarget(Radius);
		SetTarget(ClosestTarget);
		break;
	}
	case ESkillTriggerType::AutoTargetRadiusDegree:
	{
		float Radius = GetAutoDetectionRadius();
		float Degree = GetAutoDetectionDegree();
		FVector ForwardVector = GetSkillOwner()->GetActorForwardVector();
		FVector2D Direction(ForwardVector.X, ForwardVector.Y);
		AGOCharacterBase* ClosestTarget = DetectClosestTargetRadiusDegreeBase(Direction, Radius, Degree);
		SetTarget(ClosestTarget);
		break;
	}
	default:
		break;
	}
}

void UGOSkillBase::HandleSkillAffect()
{
	switch (GetSkillAffectType())
	{
	case ESkillAffectType::Damage:
		// Damage 효과 처리 로직
		UE_LOG(LogTemp, Log, TEXT("Skill Affect Type: Damage"));
		if (AGOCharacterBase* Target = GetTarget())
		{
			// Target에게 Damage 처리 로직
		}
		break;
	case ESkillAffectType::Defense:
		// Defense 효과 처리 로직
		UE_LOG(LogTemp, Log, TEXT("Skill Affect Type: Defense"));
		break;
	case ESkillAffectType::Heal:
		// Heal 효과 처리 로직
		UE_LOG(LogTemp, Log, TEXT("Skill Affect Type: Heal"));
		break;
	case ESkillAffectType::Buff:
		// Buff 효과 처리 로직
		UE_LOG(LogTemp, Log, TEXT("Skill Affect Type: Buff"));
		break;
	case ESkillAffectType::Debuff:
		// Debuff 효과 처리 로직
		UE_LOG(LogTemp, Log, TEXT("Skill Affect Type: Debuff"));
		break;
	default:
		break;
	}
}

void UGOSkillBase::SpawnParticleEffect(ESkillCastType CastType, EParticleSpawnLocation SpawnLocation)
{
	switch (CastType)
	{
	case ESkillCastType::Instant:
		// Immediate effect, spawn particles directly
		HandleSpawnParticle(SpawnLocation);
		break;
	case ESkillCastType::Projectile:
		// Projectile effect, typically handled elsewhere, but for now, use the same logic as Instant
		HandleSpawnParticle(SpawnLocation);
		break;
	case ESkillCastType::Delayed:
		// Delayed spawning logic, using a timer to spawn after a delay
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, SpawnLocation]()
			{
				HandleSpawnParticle(SpawnLocation);
			});
		break;
	case ESkillCastType::AreaOfEffect:
		// Area effect logic, spawn particles at the specified location
		HandleSpawnParticle(SpawnLocation);
		break;
	case ESkillCastType::Charge:
		// Charged skill logic, using a timer to simulate charging time
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, SpawnLocation]()
			{
				HandleSpawnParticle(SpawnLocation);
			});
		break;
	default:
		break;
	}
}

void UGOSkillBase::HandleSpawnParticle(EParticleSpawnLocation SpawnLocation)
{
	switch (SpawnLocation)
	{
	case EParticleSpawnLocation::Caster:
		if (AActor* Owner = GetSkillOwner())
		{
			SpawnParticleAtActor(Owner);
		}
		break;
	case EParticleSpawnLocation::Target:
		if (AGOCharacterBase* Target = GetTarget())
		{
			SpawnParticleAtActor(Target);
		}
		break;
	case EParticleSpawnLocation::Projectile:
		// Handle projectile particle spawning if applicable
		break;
	case EParticleSpawnLocation::Ground:
		if (AActor* Owner = GetSkillOwner())
		{
			FVector Location = Owner->GetActorLocation();
			SpawnParticleAtLocation(Location);
		}
		break;
	case EParticleSpawnLocation::AroundCaster:
		if (AActor* Owner = GetSkillOwner())
		{
			SpawnParticleAroundActor(Owner, 1.f); // Example radius
		}
		break;
	case EParticleSpawnLocation::AroundTarget:
		if (AGOCharacterBase* Target = GetTarget())
		{
			SpawnParticleAroundActor(Target, 1.f); // Example radius
		}
		break;
	case EParticleSpawnLocation::CustomLocation:
		// Handle custom location particle spawning
		break;
	default:
		break;
	}
}

void UGOSkillBase::CheckCooldownTick()
{
	if (CoolDownTimer > 0.0f)
	{
		CoolDownTimer -= 0.1f;
	}
}

void UGOSkillBase::EndCooldown()
{
	GetWorld()->GetTimerManager().ClearTimer(CoolDownTickTimerHandle);
	CoolDownTimer = 0.0f;
}

void UGOSkillBase::SetTarget(AGOCharacterBase* NewTarget)
{
	TargetGOCharacter = NewTarget;
}

TObjectPtr<AGOCharacterBase> UGOSkillBase::GetTarget()
{
	return TargetGOCharacter;
}


void UGOSkillBase::ExecuteSkill(ESkillCollisionType SkillCollisionType)
{
	switch (SkillCollisionType)
	{
	case ESkillCollisionType::LineTraceSingle:
		PerformLineTraceSingle(GetTotalSkillStat(), OutHitCollisionStruct.OutHitResult);
		break;
	case ESkillCollisionType::LineTraceMulti:
		PerformLineTraceMulti(GetTotalSkillStat(), OutHitCollisionStruct.OutHitResults);
		break;
	case ESkillCollisionType::SweepSingle:
		PerformSweepSingle(GetTotalSkillStat(), OutHitCollisionStruct.OutHitResult);
		break;
	case ESkillCollisionType::SweepMulti:
		PerformSweepMulti(GetTotalSkillStat(), OutHitCollisionStruct.OutHitResults);
		break;
	case ESkillCollisionType::OverlapMulti:
		PerformOverlapMulti(GetTotalSkillStat(), OutHitCollisionStruct.OutOverlaps);
		break;
	}
}

void UGOSkillBase::PerformLineTraceSingle(const FGOSkillStat& Stats, FHitResult& OutHitResult)
{
	if (!SkillOwnerCharacter) return;
	AGOCharacterBase* OwnerActor = Cast<AGOCharacterBase>(SkillOwnerCharacter);
	FVector Forward = OwnerActor->GetActorForwardVector();
	FVector Start = OwnerActor->GetActorLocation() + Forward * OwnerActor->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector End = Start + Forward * Stats.DamageRange;

	//FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwnerActor); // 스킬 사용자는 무시

	HitDetected = OwnerActor->GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, CCHANNEL_GOACTION, CollisionParams);
}

void UGOSkillBase::PerformLineTraceMulti(const FGOSkillStat& Stats, TArray<FHitResult>& OutHitResults)
{
	if (!SkillOwnerCharacter) return;
	AGOCharacterBase* OwnerActor = Cast<AGOCharacterBase>(SkillOwnerCharacter);
	FVector Forward = OwnerActor->GetActorForwardVector();
	FVector Start = OwnerActor->GetActorLocation() + Forward * OwnerActor->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector End = Start + Forward * Stats.DamageRange;

	//TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwnerActor);

	HitDetected = OwnerActor->GetWorld()->LineTraceMultiByChannel(OutHitResults, Start, End, CCHANNEL_GOACTION, CollisionParams);
}

void UGOSkillBase::PerformSweepSingle(const FGOSkillStat& Stats, FHitResult& OutHitResult)
{
	if (!SkillOwnerCharacter) return;

	AGOCharacterBase* OwnerActor = Cast<AGOCharacterBase>(SkillOwnerCharacter);
	FVector Forward = OwnerActor->GetActorForwardVector();
	FVector Start = OwnerActor->GetActorLocation() + Forward * (OwnerActor->GetCapsuleComponent()->GetScaledCapsuleRadius() + 50.0f);
	//FVector End = Start + FVector(1, 0, 0);  // 무의미한 벡터, 스위프의 시작점만 중요
	FVector End = Start + Forward * Stats.DamageRange;  // 무의미한 벡터, 스위프의 시작점만 중요

	//FHitResult HitResult;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Stats.DamageRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerActor);

	HitDetected = OwnerActor->GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_GOACTION, CollisionShape, Params);
}

void UGOSkillBase::PerformSweepMulti(const FGOSkillStat& Stats, TArray<FHitResult>& OutHitResults)
{
	if (!SkillOwnerCharacter) return;
	AGOCharacterBase* OwnerActor = Cast<AGOCharacterBase>(SkillOwnerCharacter);
	FVector Forward = OwnerActor->GetActorForwardVector();
	FVector Start = OwnerActor->GetActorLocation() + Forward * (OwnerActor->GetCapsuleComponent()->GetScaledCapsuleRadius() + 50.0f);
	FVector End = Start + FVector(1, 0, 0);  // 무의미한 벡터, 스위프의 시작점만 중요

	//TArray<FHitResult> HitResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Stats.DamageRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerActor);

	HitDetected = OwnerActor->GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, CCHANNEL_GOACTION, CollisionShape, Params);
}

void UGOSkillBase::PerformOverlapMulti(const FGOSkillStat& Stats, TArray<FOverlapResult>& OutOverlaps)
{
	if (!SkillOwnerCharacter) return;

	AGOCharacterBase* OwnerActor = Cast<AGOCharacterBase>(SkillOwnerCharacter);
	FVector Forward = OwnerActor->GetActorForwardVector();
	FVector Location = OwnerActor->GetActorLocation() + Forward * (OwnerActor->GetCapsuleComponent()->GetScaledCapsuleRadius() + 50.0f);
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Stats.DamageRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerActor);

	HitDetected = OwnerActor->GetWorld()->OverlapMultiByChannel(OutOverlaps, Location, FQuat::Identity, CCHANNEL_GOACTION, CollisionShape, Params);
}

TObjectPtr<AGOCharacterBase> UGOSkillBase::DetectClosestTarget(float Radius)
{
	TArray<FOverlapResult> OutResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetSkillOwner());  // 자기 자신은 무시

	FVector Location = GetSkillOwner()->GetActorLocation();
	GetWorld()->OverlapMultiByChannel(OutResults, Location, FQuat::Identity, CCHANNEL_GOACTION, FCollisionShape::MakeSphere(Radius), CollisionParams);

	AGOCharacterBase* ClosestCharacter = nullptr;
	float MinDistance = FLT_MAX;

	for (auto& Result : OutResults)
	{
		AActor* HitActor = Result.GetActor();
		AGOCharacterBase* GOCharacter = Cast<AGOCharacterBase>(HitActor);  // GOCharacterBase 타입으로 캐스트
		if (GOCharacter && GOCharacter != GetSkillOwner())
		{
			float Distance = (GOCharacter->GetActorLocation() - Location).Size();
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestCharacter = GOCharacter;
			}
		}
	}
	return ClosestCharacter;
}

TObjectPtr<AGOCharacterBase> UGOSkillBase::DetectClosestTargetRadiusDegreeBase(const FVector2D& Dir, float Radius, float Degree)
{
	TArray<FOverlapResult> OutResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetSkillOwner());  // 자기 자신은 무시

	FVector Location = GetSkillOwner()->GetActorLocation();
	GetWorld()->OverlapMultiByChannel(OutResults, Location, FQuat::Identity, CCHANNEL_GOACTION, FCollisionShape::MakeSphere(Radius), CollisionParams);

	AGOCharacterBase* ClosestCharacter = nullptr;
	float MinDistance = FLT_MAX;

	for (auto& Result : OutResults)
	{
		AActor* HitActor = Result.GetActor();
		AGOCharacterBase* GOCharacter = Cast<AGOCharacterBase>(HitActor);
		if (GOCharacter && GOCharacter != GetSkillOwner())
		{
			FVector2D HitDir = FVector2D(GOCharacter->GetActorLocation() - Location);
			HitDir.Normalize();
			float CosTheta = FMath::Cos(FMath::DegreesToRadians(Degree));
			if (FVector2D::DotProduct(Dir, HitDir) >= CosTheta)
			{
				float Distance = (GOCharacter->GetActorLocation() - Location).Size();
				if (Distance < MinDistance)
				{
					MinDistance = Distance;
					ClosestCharacter = GOCharacter;
				}
			}
		}
	}
	return ClosestCharacter;
}