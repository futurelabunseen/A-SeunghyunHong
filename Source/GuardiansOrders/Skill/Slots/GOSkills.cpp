// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Slots/GOSkills.h"
#include "GameData/GOGameSubsystem.h"
#include <Kismet/GameplayStatics.h>

UGOSkills::UGOSkills()
{
}

void UGOSkills::InitializeSkills(FName InCharacterName)
{
    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
    if (!ensure(GameInstance)) return;
    auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();
    if (GOGameInstance == nullptr) return;
    
    EHeroType HeroType = ConvertFNameToHeroType(InCharacterName); 
    Skills = GOGameInstance->GetCharacterSkillSet(HeroType);
}

UGOSkillBase* UGOSkills::GetSkill(ECharacterSkills SkillType) 
{
    if (UGOSkillBase** FoundSkill = Skills.Find(SkillType))
    {
        return *FoundSkill;
    }
    return nullptr;
}