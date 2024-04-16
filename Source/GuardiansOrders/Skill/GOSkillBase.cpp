// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillBase.h"
#include "Net/UnrealNetwork.h"

UGOSkillBase::UGOSkillBase()
{

}

void UGOSkillBase::PostInitProperties()
{
	Super::PostInitProperties();

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
}

void UGOSkillBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}