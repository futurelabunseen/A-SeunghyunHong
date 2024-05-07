// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GOCharacterMovementComponent.h"
#include "GameFramework/Character.h"

UGOCharacterMovementComponent::UGOCharacterMovementComponent()
{
	bPressedFlashSpell = false;
	bDidFlash = false;

	FlashMovementOffset = 600.0f; // 6M
	FlashCoolTime = 3.0f; // 3 Seconds
}

void UGOCharacterMovementComponent::SetFlashSpellCommand()
{
	bPressedFlashSpell = true;
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

		bDidFlash = true;

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
			{
				bDidFlash = false;
				UE_LOG(LogTemp, Log, TEXT("[Spell Flash] ended."));
			}
		), FlashCoolTime, false, -1.0f);
	}
}

void UGOCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	// 발동 조건
	if (bPressedFlashSpell && !bDidFlash)
	{
		GOFlash();
	}

	if (bPressedFlashSpell)
	{
		bPressedFlashSpell = false;

	}
}

void UGOCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	// 디코딩
	bPressedFlashSpell = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	bDidFlash = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;

	if (CharacterOwner && CharacterOwner->GetLocalRole() == ROLE_Authority)
	{
		if (bPressedFlashSpell && !bDidFlash)
		{
			GOFlash();
		}
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

	// 인코딩
	if (bPressedFlashSpell)
	{
		Result |= FLAG_Custom_0;
	}

	if (bDidFlash)
	{
		Result |= FLAG_Custom_1;
	}

	return Result;
}
