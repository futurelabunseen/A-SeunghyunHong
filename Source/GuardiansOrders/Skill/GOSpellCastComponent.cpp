// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSpellCastComponent.h"
#include "GameData/GOGameSubsystem.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UGOSpellCastComponent::UGOSpellCastComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGOSpellCastComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGOSpellCastComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if (CurrentSpell != nullptr && CurrentSpell->bIsCasting)
	//{
	//	OnUpdateCast(DeltaTime);
	//}
}

bool UGOSpellCastComponent::OnStartCast(FHeroSpellKey Key)
{
	SetCurrentSpellKey(Key);
	SetCurrentSpellByKey(SpellKey);
	if (CurrentSpell->bIsCasting == false && CurrentSpell->bIsCastable == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("[newspell UGOSkillspellComponent::OnStartCast] O"));
		CurrentSpell->StartCast();
		return true;
	}
	//bIsOnCasting = true;
	//UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	//if (!ensure(GameInstance)) return;
	//auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();

	//SpellKey = Key;
	//CurrentSpell = GOGameInstance->GetSpellByHeroSpellKey(Key);
	
	return false;
}

void UGOSpellCastComponent::OnUpdateCast(float DeltaTime)
{
	if (CurrentSpell == nullptr)
	{
		return;
	}
	CastDownTimer += DeltaTime;

	// 스킬 캐스팅 중 업데이트 로직
	//CurrentSpell->UpdateCast(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI UGOSkillCastComponent::OnUpdateCast] CurrentSkill->UpdateCast(DeltaTime);"));

	//if (CurrentSpell->bIsCastable)
	//{
	//	CurrentSpell->bIsCastable = false;
	//}

	// TODO
	if (CastDownTimer > CurrentSpell->GetCastingTime())
	{
		OnFinishCast();
	}
}

void UGOSpellCastComponent::OnFinishCast()
{
	if (CurrentSpell == nullptr)
	{
		return;
	}
	CurrentSpell->ActivateEffect();
	CurrentSpell->FinishCast();  // 스킬의 완료 로직 실행
	CurrentSpell = nullptr;
	bIsOnCasting = false;
	CastDownTimer = 0.f;
}

void UGOSpellCastComponent::OnInterruptCast()
{
	if (CurrentSpell == nullptr)
	{
		return;
	}

	CurrentSpell->InterruptedCast();  // 스킬 중단 로직 실행
	CurrentSpell = nullptr;
	bIsOnCasting = false;
}

void UGOSpellCastComponent::UpdateCoolDownTime(float DeltaTime)
{
}

void UGOSpellCastComponent::SetCurrentSpellByKey(FHeroSpellKey Key)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();

	CurrentSpell = GOGameInstance->GetSpellByHeroSpellKey(Key);
}

void UGOSpellCastComponent::SetCurrentSpellKey(FHeroSpellKey Key)
{
	SpellKey = Key;
}

TObjectPtr<UGOSpellBase> UGOSpellCastComponent::GetCurrentSpell()
{
	return CurrentSpell;
}

FHeroSpellKey UGOSpellCastComponent::GetCurrentSpellKey()
{
	return SpellKey;
}

