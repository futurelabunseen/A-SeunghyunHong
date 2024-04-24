// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillCastComponent.h"
#include "Character/GOCharacterBase.h"

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

	// ��ų ĳ���� ���� ����
	bIsOnCasting = true;

	// Cast���� Ȱ��ȭ ��. �� ������ ����� ����Ѵ�
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

	// ��ų ĳ���� �� ������Ʈ ����
	CurrentSkill->UpdateCast(DeltaTime);

	//�׽�Ʈ�� �ڵ� ... ��������Ʈ�� ����ؾ�����?
	AActor* OwnerActor = GetOwner();
	AGOCharacterBase* OwnerCharacter = Cast<AGOCharacterBase>(OwnerActor);
	if (OwnerCharacter)
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(CurrentSkill->GetTotalSkillData().SkillAnim);
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
	CurrentSkill->FinishCast();  // ��ų�� �Ϸ� ���� ����
	CurrentSkill = nullptr;
	bIsOnCasting = false;
	//GetOwner()->SetPlayerActionState(EGOPlayerActionState::Cast, false);  // Cast ���� ��Ȱ��ȭ

}

void UGOSkillCastComponent::OnInterruptCast()
{
	if (CurrentSkill == nullptr)
	{
		return;
	}

	CurrentSkill->InterruptedCast();  // ��ų �ߴ� ���� ����
	CurrentSkill = nullptr;
	bIsOnCasting = false;
	// GetOwner()->SetPlayerActionState(EGOPlayerActionState::Cast, false);  // Cast ���� ��Ȱ��ȭ
}

