// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/GOSkillCastComponent.h"
#include "Character/GOCharacterBase.h"

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
	UE_LOG(LogTemp, Log, TEXT("[SkillSystem] UGOSkillCastComponent::BeginPlay() called"));


}

void UGOSkillCastComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentSkill != nullptr && CurrentSkill->bIsCasting)
	{
		OnUpdateCast(DeltaTime);
	}
}

void UGOSkillCastComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UGOSkillCastComponent, CoolDown);
	//DOREPLIFETIME(UGOSkillCastComponent, CoolDownTimer);
}

void UGOSkillCastComponent::OnStartCast(UGOSkillBase* InSkillInstance)
{
	// 스킬 키 입력 시 여기로 들어온다!

	// TODO: 스킬 캐스팅이 가능한 상태에 있는지 확인하기 (상태 확인은 CharacterBase에서 해주기)
	// (예: 캐릭터가 죽은 상태이거나, 마나가 부족하거나, 이미 어떠한 스킬 발동 중이면 StartCast 불가)
	// 상태 확인 후, 스킬 캐스팅이 가능한 상태이면 이 함수로 들어오도록 하기 (이름도 그래서 OnStartCast)

	UE_LOG(LogTemp, Log, TEXT("[SkillSystem] UGOSkillCastComponent OnStartCast() is Called."));

	if (InSkillInstance == nullptr)
	{
		return;
	}

	ESkillTriggerType TriggerType = InSkillInstance->GetSkillTriggerType();
	ESkillAffectType AffectType = InSkillInstance->GetSkillAffectType();

	// TODO: 스킬 타입에 따른 스킬 처리 로직을 switch case 로 구분
	// 각 타입마다 필요한 데이터가 다르다 (캐릭터의 방어력을 Component의 Owner로 가져올지? 스킬의 방어력?)
	// (생각중) 이 방법 말고 또 다른 방법이 있을지

	// TODO: AffectType 은? 
	// (생각중) 각 스킬의 효과를 발동하는 로직은 스킬 객체 내에서 캡슐화하고, SkillCastComponent는 어떤 특정한 스킬이 발동되는지만 신경 쓰기
	switch (TriggerType)
	{
	case ESkillTriggerType::Target:
		HandleTargetSkill(InSkillInstance);
		break;

	case ESkillTriggerType::AutoTarget:
		HandleAutoTargetSkill(InSkillInstance);
		break;

	case ESkillTriggerType::NonTargetDirection:
		HandleDirectionalSkill(InSkillInstance);
		break;

	case ESkillTriggerType::NonTargetRange:
		HandleAreaOfEffectSkill(InSkillInstance);
		break;

	default:
		break;
	}

	// 스킬 캐스팅 시작 로직
	bIsOnCasting = true;

	// Cast상태 활성화 예. 더 유연한 방법을 써야한다
	// GetOwner()->SetPlayerActionState(EGOPlayerActionState::Cast, true);  

	CurrentSkill = InSkillInstance;
	CurrentSkill->StartCast(); 
}

void UGOSkillCastComponent::OnUpdateCast(float DeltaTime)
{
	if (CurrentSkill == nullptr || !bIsOnCasting)
	{
		return;
	}

	// 스킬 캐스팅 중 업데이트 로직
	CurrentSkill->UpdateCast(DeltaTime);

	//테스트용 코드 ... 델리게이트를 사용해야할지?
	AActor* OwnerActor = GetOwner();
	AGOCharacterBase* OwnerCharacter = Cast<AGOCharacterBase>(OwnerActor);
	if (OwnerCharacter)
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(CurrentSkill->GetTotalSkillData().SkillAnim);
	}

	if (DeltaTime > CurrentSkill->GetCastingTime())
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
	CurrentSkill = nullptr;
	bIsOnCasting = false;
	//GetOwner()->SetPlayerActionState(EGOPlayerActionState::Cast, false);  // Cast 상태 비활성화
	
}

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

void UGOSkillCastComponent::HandleTargetSkill(UGOSkillBase* SkillInstance)
{
	UE_LOG(LogTemp, Log, TEXT("UGOSkillCastComponent::HandleTargetSkill is triggered."));
}

void UGOSkillCastComponent::HandleAutoTargetSkill(UGOSkillBase* SkillInstance)
{
	UE_LOG(LogTemp, Log, TEXT("UGOSkillCastComponent::HandleAutoTargetSkill is triggered."));
}

void UGOSkillCastComponent::HandleDirectionalSkill(UGOSkillBase* SkillInstance)
{
	UE_LOG(LogTemp, Log, TEXT("UGOSkillCastComponent::HandleDirectionalSkill is triggered."));
}

void UGOSkillCastComponent::HandleAreaOfEffectSkill(UGOSkillBase* SkillInstance)
{
	UE_LOG(LogTemp, Log, TEXT("UGOSkillCastComponent::HandleAreaOfEffectSkill is triggered."));
}