// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillBase.h"
#include "Net/UnrealNetwork.h"

UGOSkillBase::UGOSkillBase()
{
}

void UGOSkillBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGOSkillBase, CoolDownTime);

}
