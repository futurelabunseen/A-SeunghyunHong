// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillBase.h"
#include "Net/UnrealNetwork.h"
#include "GameData/GOGameSubsystem.h"
#include "Subsystems/SubsystemCollection.h"
#include "Kismet/GameplayStatics.h"
#include "GameData/GOCharacterData.h"
#include "GameData/GOCharacterStat.h"

UGOSkillBase::UGOSkillBase()
{

}


void UGOSkillBase::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGOSkillBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UGOSkillBase::SetSkillOwner(AActor* NewOwner)
{
	SkillOwnerCharacter = NewOwner;
}

void UGOSkillBase::InitializeSkill(FName InSkillName)
{
	UE_LOG(LogTemp, Warning, TEXT("UGOSkillBase::InitializeSkill InSkillName: %s"), InSkillName);

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!ensure(GameInstance)) return;

	// Retrieve the subsystem from the game instance.
	auto GOGameInstance = GameInstance->GetSubsystem<UGOGameSubsystem>();
	if (GOGameInstance)
	{
		FGOSkillData* SkillDataRow = GOGameInstance->GetSkillData(InSkillName);
		SkillData = *SkillDataRow; 
		// 이게 테이블을 직접 참조하는 것이다..! 내가 쓸 구조체로 만들어서 복사해서 써야 한다.
		// 이 클래스 인스턴스에서 전용으로 쓸 구조체를 만들어서 써야 한다!!
		// 안그러면 테이블 값이 바뀔 수 있다. 
		FGOSkillStat* SkillStatRow = GOGameInstance->GetSkillStatData(SkillDataRow->SkillStatName);

		if (SkillStatRow)
		{
			SkillStat = *SkillStatRow; // 이게 테이블을 직접 참조하는 것이다..!
			UE_LOG(LogTemp, Warning, TEXT("SkillStat = *SkillStatRow; is called."));
		}
	}	
}

void UGOSkillBase::StartCast()
{
	if (IsCastable() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill is not castable due to cooldown or other conditions."));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("[SkillSystem] UGOSkillBase::StartCast() is Called."));

	bIsCasting = true;
	SetIsOnCoolTime(true);

	SetCoolDownTimer();
	// 델리게이트로 알려주장
	UGOSkillBaseFIsOnCooldown.Broadcast(GetIsOnCoolTime());  // 쿨다운 시작 시 즉시 UI 업데이트
	UE_LOG(LogTemp, Warning, TEXT("[ UGOSkillBase::StartCast] Broadcast"));

	//if (GetWorld()->GetTimerManager().IsTimerActive(CoolDownTickTimerHandle))
	//{
	//	GetWorld()->GetTimerManager().ClearTimer(CoolDownTickTimerHandle);
	//}
	//GetWorld()->GetTimerManager().SetTimer(CoolDownTickTimerHandle, this, &UGOSkillBase::CheckCooldownTick, 0.1f, true);
}

void UGOSkillBase::UpdateCast(float DeltaTime)
{
	//if (CoolDownTimer > 0.0f)
	//{
	//	CoolDownTimer -= DeltaTime;
	//	if (CoolDownTimer <= 0.0f)
	//	{
	//		CoolDownTimer = 0.0f;
	//		bIsCastable = true;
	//	}
	//	OnCooldownUpdated.Broadcast(CoolDownTimer);
	//	UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI UGOSkillBase::UpdateCast] is called. %d"), CoolDownTimer);
	//}
}

void UGOSkillBase::Activate()
{
	// 스킬 효과 발동 로직, 예: 대미지 처리, 상태 효과 적용 등
	UE_LOG(LogTemp, Log, TEXT("[UGOSkillBase::Activate()] Skill %s activated."), *SkillData.SkillName);
}

void UGOSkillBase::FinishCast()
{
	// bIsCasting = false;
	// bIsCastable = false; // 쿨다운이 진행되므로 다시 캐스트할 수 없음
	SetCoolDownTimer();  // 쿨다운 타이머 재설정 ㅠㅠ....
	UE_LOG(LogTemp, Log, TEXT("[SkillBarUI UGOSkillBase::FinishCast()] is called "));
	SetIsOnCoolTime(false);

}

void UGOSkillBase::InterruptedCast()
{
	// bIsCasting = false;
	// 필요한 경우 캐스팅 중단 처리, 예: 애니메이션 중단, 효과 제거 등
	UE_LOG(LogTemp, Log, TEXT("Skill casting interrupted."));
}

void UGOSkillBase::ActivateEffect()
{

}

void UGOSkillBase::CheckCooldownTick()
{
	if (CoolDownTimer > 0.0f)
	{
		CoolDownTimer -= 0.1f;
		UE_LOG(LogTemp, Warning, TEXT("[SkillBarUI UGOSkillBase::CheckCooldownTick()] CoolDownTimer : %f "), CoolDownTimer);
		//OnCooldownUpdated.Broadcast(CoolDownTimer);  // UI 업데이트를 위해 델리게이트 호출
		//if (CoolDownTimer <= 0.0f)
		//{
		//	EndCooldown();
		//}
	}
}

void UGOSkillBase::EndCooldown()
{
	GetWorld()->GetTimerManager().ClearTimer(CoolDownTickTimerHandle);
	CoolDownTimer = 0.0f;
	// bIsCastable = true;
	//OnCooldownUpdated.Broadcast(CoolDownTimer);  // 최종적으로 쿨다운 완료 알림
}