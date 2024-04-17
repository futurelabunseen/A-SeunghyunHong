// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillBase.h"
#include "Net/UnrealNetwork.h"
#include "GameData/GOGameSubsystem.h"
#include "Subsystems/SubsystemCollection.h"
#include "Kismet/GameplayStatics.h"

UGOSkillBase::UGOSkillBase()
{
}

void UGOSkillBase::PostInitProperties()
{
	Super::PostInitProperties();
	InitializeSkillStats();

}

void UGOSkillBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UGOSkillBase::SetSkillStat(int8 InNewSkillType)
{
}

void UGOSkillBase::ResetSkillStat()
{
}

void UGOSkillBase::InitializeSkillStats()
{
	// DataAsset
	if (SkillDataAsset)
	{
		Name = SkillDataAsset->Name;
		Description = SkillDataAsset->Description;
		Type = SkillDataAsset->Type;
		Texture = SkillDataAsset->Texture;
		SkillAnim = SkillDataAsset->SkillAnim;
		// DecalMaterial = SkillDataAsset->DecalMaterial;
		// VFX = SkillDataAsset->VFX;
	}

	// ���� ����ý��ۿ��� ��ų Ÿ�Կ� �´� ��ų ������ �˻��ϰ� ����
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this))
	{
		UGOGameSubsystem* GameSubsystem = GameInstance->GetSubsystem<UGOGameSubsystem>();
		if (GameSubsystem)
		{
			SkillStat = GameSubsystem->GetSkillStat(CurrentSkillType);
			SetSkillStat(SkillStat);
		}
	}
}
