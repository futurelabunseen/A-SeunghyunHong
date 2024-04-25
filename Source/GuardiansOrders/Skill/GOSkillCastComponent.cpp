// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillCastComponent.h"
#include "Character/GOCharacterBase.h"
#include <Interface/GOPlaySkillAnimInterface.h>

UGOSkillCastComponent::UGOSkillCastComponent()
	: bIsOnCasting(false)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

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

	if (CurrentSkill != nullptr && CurrentSkill->bIsCasting)
	{
		OnUpdateCast(DeltaTime);
	}

}

void UGOSkillCastComponent::OnStartCast(UGOSkillBase* InSkillInstance)
{
	UE_LOG(LogTemp, Log, TEXT("[SkillSystem] UGOSkillCastComponent OnStartCast() is Called."));

	if (InSkillInstance == nullptr)
	{
		return;
	}

	ESkillTriggerType TriggerType = InSkillInstance->GetSkillTriggerType();
	ESkillAffectType AffectType = InSkillInstance->GetSkillAffectType();

	// 스킬 캐스팅 시작 로직
	bIsOnCasting = true;

	// Cast상태 활성화 예. 더 유연한 방법을 써야한다
	// GetOwner()->SetPlayerActionState(EGOPlayerActionState::Cast, true);  

	CurrentSkill = InSkillInstance;
	CurrentSkill->StartCast();
}

void UGOSkillCastComponent::OnUpdateCast(float DeltaTime)
{
	if (CurrentSkill == nullptr || !bIsOnCasting)
	{
		return;
	}

	/*
	CastComponent는 OnUpdateCast에서 공격 판정과 상관없이 캐릭터가 해야하는 일을 하도록 시킨다.
	- 스킬 유효 범위 시각화하세요
	- 마나 소모하세요
	- 스킬 애니메이션 재생하세요
	- RPC 하세요
	- 이펙트 재생하세요
	- 등등..
	*/

	// 스킬 캐스팅 중 업데이트 로직
	CurrentSkill->UpdateCast(DeltaTime);

	if (AActor* Owner = GetOwner())
	{
		if (IGOPlaySkillAnimInterface* GOPlaySkillAnimInterface = Cast<IGOPlaySkillAnimInterface>(Owner))
		{
			GOPlaySkillAnimInterface->PlaySkillAnim(CurrentSkill);
			UE_LOG(LogTemp, Warning, TEXT("[UGOSkillCastComponent::OnUpdateCast] called. This function call CharacterBase's PlaySkillAnim "));

		}

		if (IGOAnimationAttackInterface* GOAnimAttackInterfac = Cast<IGOAnimationAttackInterface>(Owner))
		{
			GOAnimAttackInterfac->AttackHitCheck();
		}
	}

	if (DeltaTime > CurrentSkill->GetCastingTime())
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
	//GetOwner()->SetPlayerActionState(EGOPlayerActionState::Cast, false);  // Cast 상태 비활성화

}

// Tick에서 확인해야겠지? 스킬 취소
// 애초에 스킬이 언제 취소되는지 확인해주기-> 상태 체크? 
// PlayerActionState 말고 어떤 상태를 또 만들어줘야 하나? Cast에 대한 상태..? 으악
void UGOSkillCastComponent::OnInterruptCast()
{
	if (CurrentSkill == nullptr)
	{
		return;
	}

	CurrentSkill->InterruptedCast();  // 스킬 중단 로직 실행
	CurrentSkill = nullptr;
	bIsOnCasting = false;
	// GetOwner()->SetPlayerActionState(EGOPlayerActionState::Cast, false);  // Cast 상태 비활성화
}

