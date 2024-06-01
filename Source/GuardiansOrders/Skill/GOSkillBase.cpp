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

}


void UGOSkillBase::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOSkillBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UGOSkillBase::SetSkillOwner(AActor* NewOwner)
{
	SkillOwnerCharacter = NewOwner;
}

void UGOSkillBase::InitializeSkill(FName InSkillName)
{
	//UE_LOG(LogTemp, Warning, TEXT("UGOSkillBase::InitializeSkill InSkillName: %s"), InSkillName);

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;

	// Retrieve the subsystem from the game instance.
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();
	if (GOGameInstance)
	{
		FGOSkillData* SkillDataRow = GOGameInstance->GetSkillData(InSkillName);
		SkillData = *SkillDataRow; 
		// 이게 테이블을 직접 참조하는 것이다..! 내가 쓸 구조체로 만들어서 복사해서 써야 한다.
		// 이 클래스 인스턴스에서 전용으로 쓸 구조체를 만들어서 써야 한다!!
		// 안그러면 테이블 값이 바뀔 수 있다. 
		FGOSkillStat* SkillStatRow = GOGameInstance->GetSkillStatData(SkillDataRow->SkillStatName);

		if (SkillStatRow)
		{
			SkillStat = *SkillStatRow; // 이게 테이블을 직접 참조하는 것이다..!
			UE_LOG(LogTemp, Warning, TEXT("SkillStat = *SkillStatRow; is called."));
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
	if (IsCastable() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill is not castable due to cooldown or other conditions."));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[SkillSystem] UGOSkillBase::StartCast() is Called."));

	bIsCasting = true;
	SetIsOnCoolTime(true);

	SetCoolDownTimer();
	// 델리게이트로 알려주장
	UGOSkillBaseFIsOnCooldown.Broadcast(GetIsOnCoolTime());  // 쿨다운 시작 시 즉시 UI 업데이트

	//SpawnParticleEffect(SkillData.SkillCastType, SkillData.ParticleSpawnLocation);
	UE_LOG(LogTemp, Warning, TEXT("[ UGOSkillBase::StartCast] Broadcast"));

	//if (GetWorld()->GetTimerManager().IsTimerActive(CoolDownTickTimerHandle))
	//{
	//	GetWorld()->GetTimerManager().ClearTimer(CoolDownTickTimerHandle);
	//}
	//GetWorld()->GetTimerManager().SetTimer(CoolDownTickTimerHandle, this, &UGOSkillBase::CheckCooldownTick, 0.1f, true);
}

void UGOSkillBase::UpdateCast(float DeltaTime)
{
	//if (CoolDownTimer > 0.0f)
	//{
	//	CoolDownTimer -= DeltaTime;
	//	if (CoolDownTimer <= 0.0f)
	//	{
	//		CoolDownTimer = 0.0f;
	//		bIsCastable = true;
	//	}
	//	OnCooldownUpdated.Broadcast(CoolDownTimer);
	//	UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI UGOSkillBase::UpdateCast] is called. %d"), CoolDownTimer);
	//}
}

void UGOSkillBase::ActivateSkill() // UGOSkillCastComponent::OnUpdateCast 에서 불림
{
	// 스킬 효과 발동 로직, 예: 대미지 처리, 상태 효과 적용 등
	ExecuteSkill(GetSkillCollisionType());
	UE_LOG(LogTemp, Log, TEXT("[UGOSkillBase::Activate()] Skill %s activated."), *SkillData.SkillName);
}

void UGOSkillBase::FinishCast()
{
	// bIsCasting = false;
	// bIsCastable = false; // 쿨다운이 진행되므로 다시 캐스트할 수 없음
	SetCoolDownTimer();  // 쿨다운 타이머 재설정 ㅠㅠ....
	UE_LOG(LogTemp, Log, TEXT("[SkillBarUI UGOSkillBase::FinishCast()] is called "));
	SetIsOnCoolTime(false);

}

void UGOSkillBase::InterruptedCast()
{
	// bIsCasting = false;
	// 필요한 경우 캐스팅 중단 처리, 예: 애니메이션 중단, 효과 제거 등
	UE_LOG(LogTemp, Log, TEXT("Skill casting interrupted."));
}

void UGOSkillBase::ActivateEffect()
{

}

void UGOSkillBase::HandleSkillTrigger()
{
	switch (GetSkillTriggerType())
	{
	case ESkillTriggerType::Target:
		UE_LOG(LogTemp, Log, TEXT("Skill Trigger Type: Target"));
		break;
	case ESkillTriggerType::NonTargetDirection:
		UE_LOG(LogTemp, Log, TEXT("Skill Trigger Type: NonTargetDirection"));
		break;
	case ESkillTriggerType::NonTargetRange:
		UE_LOG(LogTemp, Log, TEXT("Skill Trigger Type: NonTargetRange"));
		break;
	case ESkillTriggerType::AutoTargetRadius:
	{
		UE_LOG(LogTemp, Log, TEXT("Skill Trigger Type: AutoTargetRadius"));
		float Radius = GetAutoDetectionRadius();
		AGOCharacterBase* ClosestTarget = DetectClosestTarget(Radius);
		SetTarget(ClosestTarget);
		break;
	}
	case ESkillTriggerType::AutoTargetRadiusDegree:
	{
		UE_LOG(LogTemp, Log, TEXT("Skill Trigger Type: AutoTargetRadiusDegree"));
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
		UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI UGOSkillBase::CheckCooldownTick()] CoolDownTimer : %f "), CoolDownTimer);
		//OnCooldownUpdated.Broadcast(CoolDownTimer);  // UI 업데이트를 위해 델리게이트 호출
		//if (CoolDownTimer <= 0.0f)
		//{
		//	EndCooldown();
		//}
	}
}

void UGOSkillBase::EndCooldown()
{
	GetWorld()->GetTimerManager().ClearTimer(CoolDownTickTimerHandle);
	CoolDownTimer = 0.0f;
	// bIsCastable = true;
	//OnCooldownUpdated.Broadcast(CoolDownTimer);  // 최종적으로 쿨다운 완료 알림
}

void UGOSkillBase::SetTarget(AGOCharacterBase* NewTarget)
{
	TargetGOCharacter = NewTarget;  // 대상 캐릭터 설정
	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::SetTarget] TargetCharacter : %s "), *TargetGOCharacter.GetName());

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
	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] ESkillCollisionType !!! %d : "), SkillCollisionType);
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
	if (HitDetected)
	{
		AActor* HitActor = OutHitResult.GetActor();
		if (HitActor)
		{
			// 피해 적용 로직 (ex: HitActor->TakeDamage())
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformLineTraceSingle Called !"));
	FColor TraceColor = HitDetected ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), Start, End, TraceColor, false, 5.0f, 0, 1.f);
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
	if (HitDetected)
	{
		for (const FHitResult& Hit : OutHitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				// 피해 적용 로직
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformLineTraceMulti Called !"));
	FColor TraceColor = HitDetected ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), Start, End, TraceColor, false, 5.0f, 0, 1.f);
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
	
	if (HitDetected)
	{
		AActor* HitActor = OutHitResult.GetActor();
		if (HitActor)
		{
			// 로그에 피격 액터의 이름을 출력
			UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] HITCHECK SweepSingle Hit Actor: %s"), *HitActor->GetName());
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformSweepSingle Called !"));
	FColor TraceColor = HitDetected ? FColor::Green : FColor::Red;
	DrawDebugCapsule(GetWorld(), Start + ((End - Start) * 0.5f), (End - Start).Size() / 2, Stats.DamageRadius, FQuat::Identity, TraceColor, false, 5.0f);
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
	if (HitDetected)
	{
		for (const FHitResult& Hit : OutHitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				// 피해 적용 로직
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformSweepMulti Called !"));
	FColor TraceColor = HitDetected ? FColor::Green : FColor::Red;
	DrawDebugCapsule(GetWorld(), Start + ((End - Start) * 0.5f), (End - Start).Size() / 2, Stats.DamageRadius, FQuat::Identity, TraceColor, false, 5.0f);
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

	// TArray<FOverlapResult> Overlaps;
	HitDetected = OwnerActor->GetWorld()->OverlapMultiByChannel(OutOverlaps, Location, FQuat::Identity, CCHANNEL_GOACTION, CollisionShape, Params);
	if (HitDetected)
	{
		for (const FOverlapResult& Overlap : OutOverlaps)
		{
			AActor* OverlappedActor = Overlap.GetActor();
			if (OverlappedActor && OverlappedActor != OwnerActor)
			{
				// 피해 적용 로직
				UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformOverlapMulti Called ! OverlappedActor : %s"), *OverlappedActor->GetName());
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformOverlapMulti Called !"));
	FColor TraceColor = OutOverlaps.Num() > 0 ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), Location, Stats.DamageRadius, 32, TraceColor, false, 5.0f);
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
	DrawDebugSphere(GetWorld(), Location, Radius, 3, FColor::Yellow, false, 10.0f);

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
	DrawDebugCone(GetWorld(), Location, FVector(Dir, 0.0f), Radius, FMath::DegreesToRadians(Degree), FMath::DegreesToRadians(Degree), 5, FColor::Yellow, false, 10.0f);

	return ClosestCharacter;
}