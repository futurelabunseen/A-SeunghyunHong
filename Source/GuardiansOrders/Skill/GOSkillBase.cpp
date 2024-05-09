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
	UE_LOG(LogTemp, Warning, TEXT("UGOSkillBase::InitializeSkill InSkillName: %s"), InSkillName);

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

void UGOSkillBase::ActivateSkill()
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
		PerformLineTraceSingle(GetTotalSkillStat());
		break;
	case ESkillCollisionType::LineTraceMulti:
		PerformLineTraceMulti(GetTotalSkillStat());
		break;
	case ESkillCollisionType::SweepSingle:
		PerformSweepSingle(GetTotalSkillStat());
		break;
	case ESkillCollisionType::SweepMulti:
		PerformSweepMulti(GetTotalSkillStat());
		break;
	case ESkillCollisionType::OverlapMulti:
		PerformOverlapMulti(GetTotalSkillStat());
		break;
	}
	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] ESkillCollisionType !!! %d : "), SkillCollisionType);

}

void UGOSkillBase::PerformLineTraceSingle(const FGOSkillStat& Stats)
{
	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformLineTraceSingle Called 0000!"));
	if (!SkillOwnerCharacter) return;
	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformLineTraceSingle Called 1111!"));
	AGOCharacterBase* OwnerActor = Cast<AGOCharacterBase>(SkillOwnerCharacter);
	FVector Forward = OwnerActor->GetActorForwardVector();
	FVector Start = OwnerActor->GetActorLocation() + Forward * OwnerActor->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector End = Start + Forward * Stats.DamageRange;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwnerActor);  // 스킬 사용자는 무시

	HitDetected = OwnerActor->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, CCHANNEL_GOACTION, CollisionParams);
	if (HitDetected)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			// 피해 적용 로직 (예: HitActor->TakeDamage())

		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformLineTraceSingle Called !"));

}


void UGOSkillBase::PerformLineTraceMulti(const FGOSkillStat& Stats)
{
	if (!SkillOwnerCharacter) return;
	AGOCharacterBase* OwnerActor = Cast<AGOCharacterBase>(SkillOwnerCharacter);
	FVector Forward = OwnerActor->GetActorForwardVector();
	FVector Start = OwnerActor->GetActorLocation() + Forward * OwnerActor->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector End = Start + Forward * Stats.DamageRange;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwnerActor);

	HitDetected = OwnerActor->GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, CCHANNEL_GOACTION, CollisionParams);
	if (HitDetected)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				// 피해 적용 로직
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformLineTraceMulti Called !"));

}

void UGOSkillBase::PerformSweepSingle(const FGOSkillStat& Stats)
{
	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformSweepSingle Called 000 !"));

	if (!SkillOwnerCharacter) return;
	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformSweepSingle Called 111 !"));

	AGOCharacterBase* OwnerActor = Cast<AGOCharacterBase>(SkillOwnerCharacter);
	FVector Forward = OwnerActor->GetActorForwardVector();
	FVector Start = OwnerActor->GetActorLocation() + Forward * OwnerActor->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector End = Start + FVector(1, 0, 0);  // 무의미한 벡터, 스위프의 시작점만 중요

	FHitResult HitResult;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Stats.DamageRadius);

	HitDetected = OwnerActor->GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, CCHANNEL_GOACTION, CollisionShape);
	if (HitDetected)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			// 피해 적용 로직
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformSweepSingle Called !"));

}

void UGOSkillBase::PerformSweepMulti(const FGOSkillStat& Stats)
{
	if (!SkillOwnerCharacter) return;
	AGOCharacterBase* OwnerActor = Cast<AGOCharacterBase>(SkillOwnerCharacter);
	FVector Forward = OwnerActor->GetActorForwardVector();
	FVector Start = OwnerActor->GetActorLocation() + Forward * OwnerActor->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector End = Start + FVector(1, 0, 0);  // 무의미한 벡터, 스위프의 시작점만 중요

	TArray<FHitResult> HitResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Stats.DamageRadius);

	HitDetected = OwnerActor->GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, CCHANNEL_GOACTION, CollisionShape);
	if (HitDetected)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				// 피해 적용 로직
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformSweepMulti Called !"));

}

void UGOSkillBase::PerformOverlapMulti(const FGOSkillStat& Stats)
{
	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformOverlapMulti 000 !"));

	if (!SkillOwnerCharacter) return;

	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillBase::ExecuteSkill] PerformOverlapMulti 111 !"));

	AGOCharacterBase* OwnerActor = Cast<AGOCharacterBase>(SkillOwnerCharacter);
	FVector Location = OwnerActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * OwnerActor->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Stats.DamageRadius);
	TArray<FOverlapResult> Overlaps;
	if (OwnerActor->GetWorld()->OverlapMultiByChannel(Overlaps, Location, FQuat::Identity, CCHANNEL_GOACTION, CollisionShape))
	{
		for (const FOverlapResult& Overlap : Overlaps)
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

}