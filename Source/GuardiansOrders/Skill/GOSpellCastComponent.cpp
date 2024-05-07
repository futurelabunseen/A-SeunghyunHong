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

	if (CurrentSpell != nullptr && CurrentSpell->bIsCasting)
	{
		OnUpdateCast(DeltaTime);
	}
}

void UGOSpellCastComponent::OnStartCast(FHeroSpellKey Key)
{
	bIsOnCasting = true;
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();

	SpellKey = Key;
	CurrentSpell = GOGameInstance->GetSpellByHeroSpellKey(Key);
	CurrentSpell->StartCast();
}

void UGOSpellCastComponent::OnUpdateCast(float DeltaTime)
{
	if (CurrentSpell == nullptr)
	{
		return;
	}
	CastDownTimer += DeltaTime;

	// ��ų ĳ���� �� ������Ʈ ����
	CurrentSpell->UpdateCast(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI UGOSkillCastComponent::OnUpdateCast] CurrentSkill->UpdateCast(DeltaTime);"));

	if (bIsOnCasting)
	{
		if (AActor* Owner = GetOwner())
		{
			//if (IGOPlaySkillAnimInterface* GOPlaySkillAnimInterface = Cast<IGOPlaySkillAnimInterface>(Owner))
			//{
			//	// GOPlaySkillAnimInterface->ActivateSkill(CurrentSkill);
			//	GOPlaySkillAnimInterface->ActivateSkillByKey(SkillKey);
			//	bIsOnCasting = false;
			//	UE_LOG(LogTemp, Warning, TEXT("[UGOSkillCastComponent::OnUpdateCast] called. This function call CharacterBase's PlaySkillAnim "));
			//}
		}
	}

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
	CurrentSpell->FinishCast();  // ��ų�� �Ϸ� ���� ����
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

	CurrentSpell->InterruptedCast();  // ��ų �ߴ� ���� ����
	CurrentSpell = nullptr;
	bIsOnCasting = false;
}

void UGOSpellCastComponent::UpdateCoolDownTime(float DeltaTime)
{
}
