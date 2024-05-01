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

    FGOCharacterData* CharacterData = GOGameInstance->GetCharacterData(InCharacterName);
    if (CharacterData == nullptr) return;

    //TMap<ECharacterSkills, TSubclassOf<UGOSkillBase>> SkillClassTypeData = {
    //    {ECharacterSkills::BaseSkill, CharacterData->BaseSkillClass},
    //    {ECharacterSkills::Skill01, CharacterData->SkillQClass},
    //    {ECharacterSkills::Skill02, CharacterData->SkillWClass},
    //    {ECharacterSkills::Skill03, CharacterData->SkillEClass},
    //    {ECharacterSkills::UltimateSkill, CharacterData->SkillRClass}
    //};
    
    SkillClassTypeData = {
        {ECharacterSkills::BaseSkill, FSkillInfo(CharacterData->BaseSkillClass, CharacterData->DefaultBaseSkillName)},
        {ECharacterSkills::Skill01, FSkillInfo(CharacterData->SkillQClass, CharacterData->DefaultSkillNameQ)},
        {ECharacterSkills::Skill02, FSkillInfo(CharacterData->SkillWClass, CharacterData->DefaultSkillNameW)},
        {ECharacterSkills::Skill03, FSkillInfo(CharacterData->SkillEClass, CharacterData->DefaultSkillNameE)},
        {ECharacterSkills::UltimateSkill, FSkillInfo(CharacterData->SkillRClass, CharacterData->DefaultSkillNameR)}
    };

    //for (const auto& Pair : SkillClassTypeData)
    //{
    //    UGOSkillBase* SkillInstance = NewObject<UGOSkillBase>(this, Pair.Value);
    //    SkillInstance->InitializeSkill(skillStatDataRow);
    //    Skills.Add(Pair.Key, SkillInstance);
    //}

    for (const auto& Pair : SkillClassTypeData)
    {
        UGOSkillBase* SkillInstance = NewObject<UGOSkillBase>(this, Pair.Value.SkillClass);
        SkillInstance->InitializeSkill(Pair.Value.SkillStatName);
        Skills.Add(Pair.Key, SkillInstance);
    }

}

UGOSkillBase* UGOSkills::GetSkill(ECharacterSkills SkillType) 
{
    if (UGOSkillBase** FoundSkill = Skills.Find(SkillType))
    {
        return *FoundSkill;
    }
    return nullptr;
}
