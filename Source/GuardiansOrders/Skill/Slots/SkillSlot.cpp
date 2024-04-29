// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Slots/SkillSlot.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASkillSlot::ASkillSlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true; // 추가
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void ASkillSlot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkillSlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillSlot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASkillSlot, SkillInstance); // SkillInstance 가 복제되도록 설정
}

void ASkillSlot::InitializeSkillSlot(TSubclassOf<UGOSkillBase> NewSkillClass, FName SkillStatDataTableRowName)
{
    SkillClassType = NewSkillClass;
    SkillInstance = NewObject<UGOSkillBase>(this, SkillClassType);
    SkillInstance->InitializeSkill(SkillStatDataTableRowName);
}