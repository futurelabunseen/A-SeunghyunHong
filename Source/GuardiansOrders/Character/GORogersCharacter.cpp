// Fill out your copyright notice in the Description page of Project Settings.


#include "GORogersCharacter.h"
#include "CharacterStat/GOCharacterStatComponent.h"
#include "GOCharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Physics/GOCollision.h"

AGORogersCharacter::AGORogersCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGOCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)),
    SphereRadius(150.0f)
{
	bReplicates = true;

    ShieldSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ShieldSphere"));
    ShieldSphere->SetupAttachment(RootComponent);
    ShieldSphere->InitSphereRadius(SphereRadius);
    ShieldSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ShieldSphere->SetCollisionObjectType(ECC_WorldDynamic);
    ShieldSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    ShieldSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    ShieldSphere->SetCollisionResponseToChannel(CCHANNEL_GOProjectile, ECR_Overlap);

    bShieldActive = false;
    MaxBlockAngle = 45.0f;  // 방패가 막을 수 있는 각도, 필요에 따라 조정
}

void AGORogersCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetData(FName(TEXT("Rogers")));
}

void AGORogersCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    DrawDebugSphere(
        GetWorld(),
        ShieldSphere->GetComponentLocation(),
        ShieldSphere->GetScaledSphereRadius(),
        32,  // 세그먼트 수
        FColor::Cyan,
        false,  // 지속 시간
        -1,  // 지속 시간(0이면 지속되지 않음)
        0,  // 두께
        1.0f  // 두께
    );
}

void AGORogersCharacter::BeginPlay()
{
	Super::BeginPlay();
    ShieldSphere->SetSphereRadius(SphereRadius);
    ShieldSphere->OnComponentBeginOverlap.AddDynamic(this, &AGORogersCharacter::OnSphereOverlap);
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

void AGORogersCharacter::ActivateShield()
{
    bShieldActive = true;
    UE_LOG(LogTemp, Log, TEXT("[Shield] AGORogersCharacter::ActivateShield() bShieldActive : %d"), bShieldActive);

    ShieldDirection = GetActorForwardVector();
    ShieldSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);  // 방패의 충돌 활성화
    UE_LOG(LogTemp, Log, TEXT("[Shield] AGORogersCharacter::ActivateShield() QueryOnly"));

}

void AGORogersCharacter::DeactivateShield()
{
    bShieldActive = false;
    UE_LOG(LogTemp, Log, TEXT("[Shield] AGORogersCharacter::DeactivateShield() bShieldActive : %d"), bShieldActive);

    ShieldSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // 방패의 충돌 비활성화
    UE_LOG(LogTemp, Log, TEXT("[Shield] AGORogersCharacter::DeactivateShield() NoCollision"));

}

bool AGORogersCharacter::IsShieldBlocking(const FVector& AttackDirection) const
{
    if (!bShieldActive)
    {
        return false;
    }

    float DotProduct = FVector::DotProduct(ShieldDirection, AttackDirection.GetSafeNormal());
    float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

    return AngleDegrees <= MaxBlockAngle;
}

void AGORogersCharacter::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (bShieldActive)
    {
        UE_LOG(LogTemp, Log, TEXT("[Shield] bShieldActive : %d"), bShieldActive);

        FVector AttackDirection = (OtherActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        if (IsShieldBlocking(AttackDirection))
        {
            // Block the attack
            UE_LOG(LogTemp, Log, TEXT("[Shield] Attack blocked!"));
            // Implement logic to negate or reduce damage
        }
    }
}

float AGORogersCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    if (bShieldActive && IsShieldBlocking((DamageCauser->GetActorLocation() - GetActorLocation()).GetSafeNormal()))
    {
        UE_LOG(LogTemp, Log, TEXT("Attack blocked! Damage reduced to 0."));
        return 0.0f; // 데미지를 0으로 설정
    }

    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}