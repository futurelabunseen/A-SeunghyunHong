// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillCastComponent.h"
#include "Character/GOCharacterBase.h"
#include <Interface/GOPlaySkillAnimInterface.h>
#include "GameData/GOGameSubsystem.h"
#include <Kismet/GameplayStatics.h>
#include <UI/GOSkillSlotWidget.h>

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

void UGOSkillCastComponent::OnStartCast(FHeroSkillKey Key)
{
	UE_LOG(LogTemp, Log, TEXT("[SkillSystem] UGOSkillCastComponent OnStartCast() is Called."));

	/*
	ESkillTriggerType TriggerType = InSkillInstance->GetSkillTriggerType();
	ESkillAffectType AffectType = InSkillInstance->GetSkillAffectType();*/

	// 스킬 캐스팅 시작 로직
	bIsOnCasting = true;

	// Cast상태 활성화 예. 더 유연한 방법을 써야한다
	// GetOwner()->SetPlayerActionState(EGOPlayerActionState::Cast, true);  

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();

	SkillKey = Key;
	CurrentSkill = GOGameInstance->GetSkillByHeroSkillKey(Key);

	// If there is an active skill with a running cooldown, end it safely
	//if (CurrentSkill && CurrentSkill->IsCasting())
	//{
	//	CurrentSkill->EndCooldown();
	//}

	CurrentSkill->StartCast();
}

void UGOSkillCastComponent::OnUpdateCast(float DeltaTime)
{
	if (CurrentSkill == nullptr)
	{
		return;
	}
	CastDownTimer += DeltaTime;

	// 스킬 캐스팅 중 업데이트 로직
	CurrentSkill->UpdateCast(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI UGOSkillCastComponent::OnUpdateCast] CurrentSkill->UpdateCast(DeltaTime);"));

	if (bIsOnCasting) 
	{
		if (AActor* Owner = GetOwner())
		{
			if (IGOPlaySkillAnimInterface* GOPlaySkillAnimInterface = Cast<IGOPlaySkillAnimInterface>(Owner))
			{
				// GOPlaySkillAnimInterface->ActivateSkill(CurrentSkill);
				GOPlaySkillAnimInterface->ActivateSkillByKey(SkillKey);
				bIsOnCasting = false;
				UE_LOG(LogTemp, Warning, TEXT("[UGOSkillCastComponent::OnUpdateCast] called. This function call CharacterBase's PlaySkillAnim "));
			}
		}
	}

	// TODO
	if (CastDownTimer > CurrentSkill->GetCastingTime())
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
	CastDownTimer = 0.f;
	//GetOwner()->SetPlayerActionState(EGOPlayerActionState::Cast, false);  // Cast 상태 비활성화

}

// 스킬 취소 및 상태 체크
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

void UGOSkillCastComponent::UpdateCoolDownTime(float DeltaTime)
{
}

