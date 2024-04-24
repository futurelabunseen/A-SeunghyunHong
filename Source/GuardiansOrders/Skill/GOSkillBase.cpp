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
