// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillCastComponent.h"

UGOSkillCastComponent::UGOSkillCastComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UGOSkillCastComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGOSkillCastComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

