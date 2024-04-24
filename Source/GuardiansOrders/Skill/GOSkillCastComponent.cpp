﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillCastComponent.h"

UGOSkillCastComponent::UGOSkillCastComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}


void UGOSkillCastComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGOSkillCastComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	 
	if (CurrentSkill != nullptr)
	{
		OnUpdateCast(DeltaTime);
	}
}

void UGOSkillCastComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UGOSkillCastComponent, CoolDown);
	//DOREPLIFETIME(UGOSkillCastComponent, CoolDownTimer);
}

void UGOSkillCastComponent::OnStartCast(UGOSkillBase* InSkill)
{
	if (InSkill == nullptr)
	{
		return;
	}

	ESkillTriggerType TriggerType = InSkill->GetSkillTriggerType();
	ESkillAffectType AffectType = InSkill->GetSkillAffectType();

	// 스킬 타입에 따른 로직 분기
	switch (TriggerType)
	{
	case ESkillTriggerType::Target:
		HandleTargetSkill(InSkill);
		break;

	case ESkillTriggerType::AutoTarget:
		HandleAutoTargetSkill(InSkill);
		break;

	case ESkillTriggerType::NonTargetDirection:
		HandleDirectionalSkill(InSkill);
		break;

	case ESkillTriggerType::NonTargetRange:
		HandleAreaOfEffectSkill(InSkill);
		break;

	default:
		break;
	}

	bIsOnCasting = true;
	// OwnerCharacter->SetPlayerActionState(EGOPlayerActionState::Cast, true);  // Cast 상태 활성화
	CurrentSkill = InSkill;
	CurrentSkill->StartCast();  // 스킬 특정 초기화 로직 호출
}

void UGOSkillCastComponent::OnUpdateCast(float DeltaTime)
{
	if (CurrentSkill == nullptr || !bIsOnCasting)
	{
		return;
	}

	CurrentSkill->UpdateCast(DeltaTime);
	
	//if (DeltaTime > CurrentSkill->GetCastingTime())
	//{
	//	OnFinishCast();
	//}

	// 캐스팅 타임 업데이트
	if (CurrentSkill->ReduceCastingTime(DeltaTime))
	{
		OnFinishCast();
	}
}

void UGOSkillCastComponent::OnFinishCast()
{
	if (CurrentSkill == nullptr)
	{
		return;
	}
	CurrentSkill->ActivateEffect();
	CurrentSkill->FinishCast();  // 스킬의 완료 로직 실행
	CurrentSkill = nullptr;
	bIsOnCasting = false;
	// OwnerCharacter->SetPlayerActionState(EGOPlayerActionState::Cast, false);  // Cast 상태 비활성화
}

void UGOSkillCastComponent::OnInterruptCast()
{
	if (CurrentSkill == nullptr)
	{
		return;
	}

	CurrentSkill->InterruptedCast();  // 스킬 중단 로직 실행
	CurrentSkill = nullptr;
	bIsOnCasting = false;
	// OwnerCharacter->SetPlayerActionState(EGOPlayerActionState::Cast, false);  // Cast 상태 비활성화
}

void UGOSkillCastComponent::HandleTargetSkill(UGOSkillBase* Skill)
{
	UE_LOG(LogTemp, Log, TEXT("UGOSkillCastComponent::HandleTargetSkill is triggered."));
}

void UGOSkillCastComponent::HandleAutoTargetSkill(UGOSkillBase* Skill)
{
	UE_LOG(LogTemp, Log, TEXT("UGOSkillCastComponent::HandleAutoTargetSkill is triggered."));
}

void UGOSkillCastComponent::HandleDirectionalSkill(UGOSkillBase* Skill)
{
	UE_LOG(LogTemp, Log, TEXT("UGOSkillCastComponent::HandleDirectionalSkill is triggered."));
}

void UGOSkillCastComponent::HandleAreaOfEffectSkill(UGOSkillBase* Skill)
{
	UE_LOG(LogTemp, Log, TEXT("UGOSkillCastComponent::HandleAreaOfEffectSkill is triggered."));
}

