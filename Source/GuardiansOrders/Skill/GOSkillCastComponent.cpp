// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillCastComponent.h"
#include <Interface/GOPlaySkillAnimInterface.h>
#include "GameData/GOGameSubsystem.h"
#include <Kismet/GameplayStatics.h>
#include "UI/SkillWidget/GOSkillSlotWidget.h"
#include "Character/GOCharacterBase.h"
#include "Character/GOPlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "Physics/GOCollision.h"

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

void UGOSkillCastComponent::OnStartCast(FHeroSkillKey Key)
{
	UE_LOG(LogTemp, Log, TEXT("[SkillSystem] UGOSkillCastComponent OnStartCast() is Called."));

	/*
	ESkillTriggerType TriggerType = InSkillInstance->GetSkillTriggerType();
	ESkillAffectType AffectType = InSkillInstance->GetSkillAffectType();*/

	// 스킬 캐스팅 시작 로직
	bIsOnCasting = true;

	// Cast상태 활성화 예. 더 유연한 방법을 써야한다
	// GetOwner()->SetPlayerActionState(EGOPlayerActionState::Cast, true);  

	SetCurrentSkillKey(Key);
	SetCurrentSkillByKey(SkillKey);

	// If there is an active skill with a running cooldown, end it safely
	//if (CurrentSkill && CurrentSkill->IsCasting())
	//{
	//	CurrentSkill->EndCooldown();
	//}

	//CurrentSkill->StartCast();

	if (CurrentSkill)
	{
		CurrentSkill->SetSkillOwner(GetOwner());
		CurrentSkill->HandleSkillTrigger();  // Trigger 처리
		CurrentSkill->StartCast();  // 타겟이 설정된 후 캐스팅 프로세스 시작
	}

}

void UGOSkillCastComponent::OnUpdateCast(float DeltaTime)
{
	if (CurrentSkill == nullptr)
	{
		return;
	}
	CastDownTimer += DeltaTime;

	// 스킬 캐스팅 중 업데이트 로직
	CurrentSkill->UpdateCast(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI UGOSkillCastComponent::OnUpdateCast] CurrentSkill->UpdateCast(DeltaTime);"));

	if (bIsOnCasting) 
	{
		//if (AActor* Owner = GetOwner())
		if (AGOPlayerCharacter* Owner = Cast<AGOPlayerCharacter>(GetOwner()))
		{
			if (CurrentSkill->GetTarget() != nullptr)
			{
				FVector TargetLocation = CurrentSkill->GetTarget()->GetActorLocation();
				FVector Direction = TargetLocation - Owner->GetActorLocation();
				Direction.Z = 0; // Ignore Z axis for rotation
				FRotator NewRotation = Direction.Rotation();
				Owner->SetActorRotation(NewRotation);

				Owner->ServerSetRotation(NewRotation);
				UE_LOG(LogTemp, Warning, TEXT("[UGOSkillCastComponent::OnUpdateCast] called. SetActorRotation To Target !!!! "));
			}

			if (IGOPlaySkillAnimInterface* GOPlaySkillAnimInterface = Cast<IGOPlaySkillAnimInterface>(Owner))
			{
				// GOPlaySkillAnimInterface->ActivateSkill(CurrentSkill);
				GOPlaySkillAnimInterface->ActivateSkillByKey(SkillKey);
				CurrentSkill->HandleSkillAffect();  // 효과 처리
				CurrentSkill->ActivateSkill();
				bIsOnCasting = false;
				UE_LOG(LogTemp, Warning, TEXT("[UGOSkillCastComponent::OnUpdateCast] called. This function call CharacterBase's PlaySkillAnim "));
			}
		}
	}

	// TODO
	if (CastDownTimer > CurrentSkill->GetCastingTime())
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
	CurrentSkill->FinishCast();  // 스킬의 완료 로직 실행
	//CurrentSkill = nullptr;
	bIsOnCasting = false;
	CastDownTimer = 0.f;
	//GetOwner()->SetPlayerActionState(EGOPlayerActionState::Cast, false);  // Cast 상태 비활성화

}

// 스킬 취소 및 상태 체크
void UGOSkillCastComponent::OnInterruptCast()
{
	if (CurrentSkill == nullptr)
	{
		return;
	}

	CurrentSkill->InterruptedCast();  // 스킬 중단 로직 실행
	CurrentSkill = nullptr;
	bIsOnCasting = false;
	// GetOwner()->SetPlayerActionState(EGOPlayerActionState::Cast, false);  // Cast 상태 비활성화
}

void UGOSkillCastComponent::UpdateCoolDownTime(float DeltaTime)
{
}

void UGOSkillCastComponent::SetCurrentSkillByKey(FHeroSkillKey Key)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();

	CurrentSkill = GOGameInstance->GetSkillByHeroSkillKey(Key);
}

void UGOSkillCastComponent::SetCurrentSkillKey(FHeroSkillKey Key)
{
	SkillKey = Key;
}

TObjectPtr<UGOSkillBase> UGOSkillCastComponent::GetCurrentSkill()
{
	return CurrentSkill;
}

FHeroSkillKey UGOSkillCastComponent::GetCurrentSkillKey()
{
	return SkillKey;
}
