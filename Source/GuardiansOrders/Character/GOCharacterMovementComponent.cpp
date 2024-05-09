// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GOCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Interface/GOSpellFlashInterface.h"

UGOCharacterMovementComponent::UGOCharacterMovementComponent()
{
	bPressedFlashSpell = false;
	bDidFlash = false;

	FlashMovementOffset = 600.0f; // 6M
	FlashCoolTime = 3.0f; // 3 Seconds

	bPressedGhostSpell = false;
	bDidGhost = false;

	GhostSpeedMultiplier = 2.0f;
	GhostCoolTime = 4.0f;
}

void UGOCharacterMovementComponent::SetFlashSpellCommand()
{
	bPressedFlashSpell = true;
}

void UGOCharacterMovementComponent::SetGhostSpellCommand()
{
	bPressedGhostSpell = true;
}

FNetworkPredictionData_Client* UGOCharacterMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		UGOCharacterMovementComponent* MutableThis = const_cast<UGOCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FGONetworkPredictionData_Client_Character(*this);
	}
	return ClientPredictionData;
}

void UGOCharacterMovementComponent::GOFlash()
{
	if (CharacterOwner)
	{
		FVector TargetLocation =
			CharacterOwner->GetActorLocation()
			+ CharacterOwner->GetActorForwardVector() * FlashMovementOffset;

		CharacterOwner->TeleportTo(TargetLocation,
			CharacterOwner->GetActorRotation(),
			false, // 테스트?
			true); // 장애물 체크?

		//if (AActor* Owner = GetOwner())
		//{
		//	if (IGOSpellFlashInterface* GOPlaySkillAnimInterface = Cast<IGOSpellFlashInterface>(Owner))
		//	{
		//		// GOPlaySkillAnimInterface->ActivateSkill(CurrentSkill);
		//		GOPlaySkillAnimInterface->ActivateSpellFlash();
		//		
		//		UE_LOG(LogTemp, Warning, TEXT("[UGOCharacterMovementComponent::GOFlash] called."));
		//	}
		//}

		bDidFlash = true;

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
			{
				bDidFlash = false;
				UE_LOG(LogTemp, Log, TEXT("[Spell Flash] ended."));
			}
		), FlashCoolTime, false, -1.0f); // 어쩌지 ?!?!
	}
}

void UGOCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	// Flash 발동 조건
	if (bPressedFlashSpell && !bDidFlash)
	{
		GOFlash();
	}

	if (bPressedFlashSpell)
	{
		bPressedFlashSpell = false;

	}

	// Ghost 발동 조건
	if (bPressedGhostSpell && !bDidGhost)
	{
		GOGhost();
	}

	if (bPressedGhostSpell)
	{
		bPressedGhostSpell = false;
	}
}

void UGOCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	// 디코딩
	// Flash
	bPressedFlashSpell = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	bDidFlash = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;

	if (CharacterOwner && CharacterOwner->GetLocalRole() == ROLE_Authority)
	{
		if (bPressedFlashSpell && !bDidFlash)
		{
			GOFlash();
		}
	}

	// 디코딩
	// Ghost
	bPressedGhostSpell = (Flags & FSavedMove_Character::FLAG_Custom_2) != 0;
	bDidGhost = (Flags & FSavedMove_Character::FLAG_Custom_3) != 0;

	if (CharacterOwner && CharacterOwner->GetLocalRole() == ROLE_Authority)
	{
		if (bPressedGhostSpell && !bDidGhost)
		{
			GOGhost();
		}
	}
}

void UGOCharacterMovementComponent::GOGhost()
{
	if (CharacterOwner)
	{
		CharacterOwner->GetCharacterMovement()->MaxWalkSpeed *= GhostSpeedMultiplier;

		bDidGhost = true;
		UE_LOG(LogTemp, Log, TEXT("[Ghost] Cooltime started: %d"), bDidGhost);

		// 이동 속도를 원래대로 복원하고, 4초 후에 다시 사용 가능하게 설정
		FTimerHandle SpeedResetHandle;
		GetWorld()->GetTimerManager().SetTimer(SpeedResetHandle, FTimerDelegate::CreateLambda([&]
			{
				CharacterOwner->GetCharacterMovement()->MaxWalkSpeed /= GhostSpeedMultiplier;
			}
		), GhostCoolTime, false);

		// 4초 후에 기능 재활성화 허용
		FTimerHandle CooldownHandle;
		GetWorld()->GetTimerManager().SetTimer(CooldownHandle, FTimerDelegate::CreateLambda([&]
			{
				bDidGhost = false;
				UE_LOG(LogTemp, Log, TEXT("[Ghost] Cooltime ended."));
			}
		), GhostCoolTime + 4.0f, false); // 기능 활성화 시간과 쿨타임을 더하여 설정
	}
}

// ============== FGONetworkPredictionData_Client_Character ==============

FGONetworkPredictionData_Client_Character::FGONetworkPredictionData_Client_Character(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr FGONetworkPredictionData_Client_Character::AllocateNewMove()
{
	return FSavedMovePtr(new FGOSavedMove_Character);
}

// ============== FGOSavedMove_Character ==============

void FGOSavedMove_Character::Clear()
{
	Super::Clear();

	bPressedFlashSpell = false;
	bDidFlash = false;
}

void FGOSavedMove_Character::SetInitialPosition(ACharacter* Character)
{
	Super::SetInitialPosition(Character);

	UGOCharacterMovementComponent* GOMovement = Cast<UGOCharacterMovementComponent>(Character->GetCharacterMovement());
	if (GOMovement)
	{
		bPressedFlashSpell = GOMovement->bPressedFlashSpell;
		bDidFlash = GOMovement->bDidFlash;
	}
}

uint8 FGOSavedMove_Character::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	// Flash 인코딩 플래그
	if (bPressedFlashSpell)
	{
		Result |= FLAG_Custom_0;
	}

	if (bDidFlash)
	{
		Result |= FLAG_Custom_1;
	}

	// Ghost 인코딩 플래그
	if (bPressedGhostSpell)
	{
		Result |= FLAG_Custom_2;
	}
	if (bDidGhost)
	{
		Result |= FLAG_Custom_3;
	}

	return Result;
}
