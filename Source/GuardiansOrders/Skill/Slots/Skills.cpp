// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Slots/Skills.h"

// Sets default values
ASkills::ASkills()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASkills::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ASkills::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkills::PostInitializeComponents()
{
	Super::PostInitializeComponents(); 
}

void ASkills::InitializeSkills(TArray<TSubclassOf<UGOSkillBase>> SkillClasses, TArray<FName> SkillStatDataRowName)
{
	for (int32 i = 0; i < SkillClasses.Num(); ++i)
	{
		Skills.Add(GetWorld()->SpawnActor<ASkillSlot>());
		Skills[i]->InitializeSkillSlot(SkillClasses[i], SkillStatDataRowName[i]);
		UE_LOG(LogTemp, Warning, TEXT("Skills[%d]: %s"), i, *Skills[i].GetName());
	}
}