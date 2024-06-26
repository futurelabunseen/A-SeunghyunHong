// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillCastComponent.h"
#include "Interface/GOPlaySkillAnimInterface.h"
#include "Interface/GOCombatInterface.h"
#include "GameData/GOGameSubsystem.h"
#include <Kismet/GameplayStatics.h>
#include "UI/SkillWidget/GOSkillSlotWidget.h"
#include "Character/GOCharacterBase.h"
#include "Character/GOPlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "Physics/GOCollision.h"
#include "Skill/Projectile/GOProjectileSkillBase.h"
#include "CharacterStat/GOCharacterStatComponent.h"

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

	//if (CurrentSkill != nullptr && CurrentSkill->bIsCasting)
	if (CurrentSkill != nullptr && CurrentSkill->bIsCasting && CurrentSkill->bIsCastable) // 이케 함
	{
		OnUpdateCast(DeltaTime);
	}
}

void UGOSkillCastComponent::OnStartCast(FHeroSkillKey Key)
{

	/*
	ESkillTriggerType TriggerType = InSkillInstance->GetSkillTriggerType();
	ESkillAffectType AffectType = InSkillInstance->GetSkillAffectType();*/

	// 스킬 캐스팅 시작 로직
	//bIsOnCasting = true;

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
	AGOPlayerCharacter* Owner = Cast<AGOPlayerCharacter>(GetOwner());
	if (Owner->Stat->GetCurrentMana() < CurrentSkill->GetTotalSkillStat().ManaCost)
	{
		return;
	}

	if (CurrentSkill->bIsCasting == false && CurrentSkill->bIsCastable == false)//여기
	{

		UE_LOG(LogTemp, Log, TEXT("[NewSkill] UGOSkillCastComponent OnStartCast() is Called."));

		CurrentSkill->SetSkillOwner(GetOwner());
		CurrentSkill->HandleSkillTrigger();  // Trigger 처리
		CurrentSkill->StartCast();  // 타겟이 설정된 후 캐스팅 프로세스 시작
		//bIsOnCasting = true;
		CurrentSkill->bIsCastable = true;
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
	//CurrentSkill->UpdateCast(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("[NewSkill UGOSkillCastComponent::OnUpdateCast] CurrentSkill->UpdateCast(DeltaTime);"));

	if (CurrentSkill->bIsCastable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[NewSkill UGOSkillCastComponent::OnUpdateCast] bIsOnCasting 0"));

		CurrentSkill->bIsCastable = false; // 이케 함		
		UE_LOG(LogTemp, Warning, TEXT("[NewSkill UGOSkillCastComponent::OnUpdateCast] bIsOnCasting 0"));

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
				GOPlaySkillAnimInterface->ActivateSkillByKey(SkillKey);
				CurrentSkill->HandleSkillAffect();
				CurrentSkill->ActivateSkill();//사용하지않음

				if (CurrentSkill)
				{
					CurrentSkill->SetSkillOwner(GetOwner());
					CurrentSkill->HandleSkillTrigger();  // Trigger 처리
					//CurrentSkill->StartCast();  // 타겟이 설정된 후 캐스팅 프로세스 시작

					if (CurrentSkill->GetSkillCastType() == ESkillCastType::Projectile)
					{
						UGOProjectileSkillBase* ProjectileSkill = Cast<UGOProjectileSkillBase>(CurrentSkill);
						if (ProjectileSkill)
						{
							IGOCombatInterface* GOCombatInterface = Cast<IGOCombatInterface>(Owner);
							if (GOCombatInterface)
							{
								const FVector SocketLocation = GOCombatInterface->GetBattleSocketLocation();

								FVector SpawnLocation = GetOwner()->GetActorLocation();
								FRotator SpawnRotation = GetOwner()->GetActorRotation();
								//FTransform SpawnTransform = GetOwner()->GetActorTransform();
								FTransform SpawnTransform;
								SpawnTransform.SetLocation(SocketLocation);

								UE_LOG(LogTemp, Warning, TEXT("[Projectile]  UGOSkillCastComponent::OnUpdateCast")); // 1번 실행

								HandleProjectileSkill(ProjectileSkill, SocketLocation, SpawnRotation, SpawnTransform);
							}

						}
					}
				}

				//bIsOnCasting = false; //원래 잇던
				UE_LOG(LogTemp, Warning, TEXT("[UGOSkillCastComponent::OnUpdateCast] called. This function call CharacterBase's PlaySkillAnim "));
			}
		}
	}

	// TODO
	if (CastDownTimer > CurrentSkill->GetCastingTime())
	{
		//OnFinishCast();
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

void UGOSkillCastComponent::HandleProjectileSkill(UGOProjectileSkillBase* ProjectileSkill, FVector Location, FRotator Rotation, FTransform SpawnTransform)
{
	if (ProjectileSkill && ProjectileSkill->ProjectileClass)
	{
		if (ProjectileSkill->bIsSpreadSkill)
		{
			//ServerHandleProjectileSkill(ProjectileSkill->ProjectileClass, Location, Rotation, SpawnTransform);
			HandleSpreadProjectiles(ProjectileSkill, Location, Rotation);
		}
		else if (ProjectileSkill->bIsAreaSkill)
		{
			ServerStartArrowRain(ProjectileSkill->ProjectileClass, Location, 5.0f, 10.f, 10);
		}
		else
		{
			ServerHandleProjectileSkill(ProjectileSkill->ProjectileClass, Location, Rotation, SpawnTransform);

		}
	}
}

void UGOSkillCastComponent::HandleSpreadProjectiles(UGOProjectileSkillBase* ProjectileSkill, FVector Location, FRotator Rotation)
{
	const FVector Forward = Rotation.Vector();
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSkill->SpreadAngle / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSkill->SpreadAngle / 2.f, FVector::UpVector);

	if (ProjectileSkill->NumProjectiles > 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Projectile] UGOSkillCastComponent::HandleSpreadProjectiles spawned. NumProjectiles: %d. Owner: %s"), ProjectileSkill->NumProjectiles, *GetOwner()->GetName());

		const float DeltaSpread = ProjectileSkill->SpreadAngle / (ProjectileSkill->NumProjectiles - 1);
		for (int32 i = 0; i < ProjectileSkill->NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			FVector SpawnLocation = Location + FVector(0, 0, 50); // Adjust the spawn location as needed
			FRotator SpawnRotation = Direction.Rotation();
			FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

			ServerHandleProjectileSkill(ProjectileSkill->ProjectileClass, SpawnLocation, SpawnRotation, SpawnTransform);
		}
	}
}

void UGOSkillCastComponent::ServerHandleProjectileSkill_Implementation(TSubclassOf<AGOProjectile> ProjectileClass, FVector Location, FRotator Rotation, FTransform SpawnTransform)
{
	if (ProjectileClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetOwner()->GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn Projectile
		AGOProjectile* Projectile = GetWorld()->SpawnActor<AGOProjectile>(ProjectileClass, Location, Rotation, SpawnParams);
	}
}

bool UGOSkillCastComponent::ServerHandleProjectileSkill_Validate(TSubclassOf<AGOProjectile> ProjectileClass, FVector Location, FRotator Rotation, FTransform SpawnTransform)
{
	return true;
}

void UGOSkillCastComponent::ServerStartArrowRain_Implementation(TSubclassOf<AGOProjectile> ProjectileClass, FVector Location, float Duration, float Interval, int32 NumProjectilesPerSpawn)
{
	FVector Forward = GetOwner()->GetActorForwardVector();
	FVector RainCenterLocation = Location + Forward * 1000.0f; // 1000 units in front of the character

	for (int32 i = 0; i < NumProjectilesPerSpawn; ++i)
	{
		FVector Offset = FVector::ZeroVector;
		float Distance = 1000.0f * (static_cast<float>(i) / (NumProjectilesPerSpawn - 1));
		FVector SpawnLocation = Location + Forward * Distance + Offset + FVector(0, 0, 300.0f); // Linear distribution of projectiles within 1000 units

		FRotator SpawnRotation = FRotator(-90.0f, 0.0f, 0.0f); // Rotate downwards

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetOwner()->GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AGOProjectile* Projectile = GetWorld()->SpawnActor<AGOProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}
bool UGOSkillCastComponent::ServerStartArrowRain_Validate(TSubclassOf<AGOProjectile> ProjectileClass, FVector Location, float Duration, float Interval, int32 NumProjectilesPerSpawn)
{
	return true;
}
