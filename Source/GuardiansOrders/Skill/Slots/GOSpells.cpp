// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Slots/GOSpells.h"
#include "GameData/GOGameSubsystem.h"
#include <Kismet/GameplayStatics.h>

UGOSpells::UGOSpells()
{
}

void UGOSpells::InitializeSpells(FName InCharacterName)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();
	if (GOGameInstance == nullptr) return;

	EHeroType HeroType = ConvertFNameToHeroTypes(InCharacterName);
	Spells = GOGameInstance->GetCharacterSpellSet(HeroType);
}

UGOSpellBase* UGOSpells::GetSpell(ECharacterSpells SpellType)
{
	if (UGOSpellBase** FoundSpell = Spells.Find(SpellType))
	{
		return *FoundSpell;
	}
	return nullptr;
}

EHeroType UGOSpells::ConvertFNameToHeroTypes(const FName& InCharacterName) const
{

	FString NameString = InCharacterName.ToString();

	// Optionally, convert to lowercase or perform other normalizations
	NameString = NameString.ToLower();

	if (NameString == TEXT("rogers"))
		return EHeroType::Rogers;
	else if (NameString == TEXT("katniss"))
		return EHeroType::Katniss;
	else if (NameString == TEXT("beast"))
		return EHeroType::Beast;
	else if (NameString == TEXT("bride"))
		return EHeroType::Bride;
	else
		return EHeroType::None; // Default case

}
