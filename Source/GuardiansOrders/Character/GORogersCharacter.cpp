// Fill out your copyright notice in the Description page of Project Settings.


#include "GORogersCharacter.h"
#include "CharacterStat/GOCharacterStatComponent.h"
#include "GOCharacterMovementComponent.h"

AGORogersCharacter::AGORogersCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGOCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	bReplicates = true;
}

void AGORogersCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetData(FName(TEXT("Rogers")));
}

void AGORogersCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGORogersCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGORogersCharacter::OnBaseSkill()
{
	Super::OnBaseSkill();
	UE_LOG(LogTemp, Log, TEXT("Rogers Base Skill is triggered."));
}

void AGORogersCharacter::OnSkillQ()
{
	Super::OnSkillQ();
	UE_LOG(LogTemp, Warning, TEXT("Rogers Skill Q is triggered."));
}

void AGORogersCharacter::OnSkillW()
{
	Super::OnSkillW();
	// ProcessComboCommand();
	UE_LOG(LogTemp, Warning, TEXT("Rogers Skill W is triggered."));
}

void AGORogersCharacter::OnSkillE()
{
    Super::OnSkillE();
    if (HasAuthority())
    {
        UGOSkillBase* Skill = SkillCastComponent->GetCurrentSkill();
        if (Skill)
        {
            ServerActivateSkillWithMovement(FHeroSkillKey(EHeroType::Rogers, ECharacterSkills::Skill03), Skill->GetMovementDistance(), Skill->GetMovementDuration(), Skill->GetAcceleration());
        }
    }
    else
    {
        UGOSkillBase* Skill = SkillCastComponent->GetCurrentSkill();
        if (Skill)
        {
            ServerActivateSkillWithMovement(FHeroSkillKey(EHeroType::Rogers, ECharacterSkills::Skill03), Skill->GetMovementDistance(), Skill->GetMovementDuration(), Skill->GetAcceleration());
        }
    }
}

void AGORogersCharacter::OnSkillR()
{
	Super::OnSkillR();
	UE_LOG(LogTemp, Warning, TEXT("Rogers Skill R is triggered."));
}

void AGORogersCharacter::HighlightActor()
{
    Super::HighlightActor();
}

void AGORogersCharacter::UnHighlightActor()
{
    Super::UnHighlightActor();
}

//void AGORogersCharacter::PlaySkillAnim(UGOSkillBase* CurrentSkill)
//{
//	 GetMesh()->GetAnimInstance()->Montage_Play(CurrentSkill->GetTotalSkillData().SkillAnim);
//	 UE_LOG(LogTemp, Warning, TEXT("[AGORogersCharacter::PlaySkillAnim]  1 called. This function is inherited from GOPlaySkillAnimInterface. "));
//}
