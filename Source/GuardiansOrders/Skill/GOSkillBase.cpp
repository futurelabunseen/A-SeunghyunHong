// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillBase.h"
#include "Net/UnrealNetwork.h"
#include "GameData/GOGameSubsystem.h"
#include "Subsystems/SubsystemCollection.h"
#include "Kismet/GameplayStatics.h"

UGOSkillBase::UGOSkillBase()
{
	ConstructorHelpers::FObjectFinder<UDataTable> SkillDataObj(TEXT("DataTable'/Game/GameData/SkillDataTable/GOSkillDataTable.GOSkillDataTable'"));
	if (SkillDataObj.Succeeded())
	{
		SkillDataTable = SkillDataObj.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> SkillStatDataObj(TEXT("DataTable'/Game/GameData/SkillStatDataTable/GOSkillStatDataTable.GOSkillStatDataTable'"));
	if (SkillStatDataObj.Succeeded())
	{
		SkillStatDataTable = SkillStatDataObj.Object;
	}
}

void UGOSkillBase::PostInitProperties()
{
	Super::PostInitProperties();
	// InitializeSkillStats();

}

void UGOSkillBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool UGOSkillBase::ReduceCastingTime(float DeltaTime)
{
	// CastingTimeLeft -= DeltaTime;
	// return CastingTimeLeft <= 0.0f;
	return true;
}

//void UGOSkillBase::SetSkillStat(int8 InNewSkillType)
//{
//}
//
//void UGOSkillBase::ResetSkillStat()
//{
//}

void UGOSkillBase::InitializeSkillStats()
{
	// DataAsset
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

	// 게임 서브시스템에서 스킬 타입에 맞는 스킬 스탯을 검색하고 설정
	//if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this))
	//{
	//	UGOGameSubsystem* GameSubsystem = GameInstance->GetSubsystem<UGOGameSubsystem>();
	//	if (GameSubsystem)
	//	{
	//		SkillStat = GameSubsystem->GetSkillStat(CurrentSkillType);
	//		SetSkillStat(SkillStat);
	//	}
	//}
}

void UGOSkillBase::Set(FName InSkillName)
{
	if (!SkillDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillDataTable is not set for this skill base class."));
		return;
	}

	FGOSkillData* SkillDataRow = SkillDataTable->FindRow<FGOSkillData>(InSkillName, TEXT(""), true);
	if (SkillDataRow)
	{
		// 데이터 테이블에서 찾은 데이터로 멤버 변수를 설정합니다.
		SkillData = *SkillDataRow;

		// 스킬 스탯 설정
		FGOSkillStat* SkillStatRow = SkillStatDataTable->FindRow<FGOSkillStat>(SkillDataRow->SkillStatName, TEXT(""), true);
		if (SkillStatRow)
		{
			SkillStat = *SkillStatRow;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill data not found for name: %s"), *InSkillName.ToString());
	}
}
