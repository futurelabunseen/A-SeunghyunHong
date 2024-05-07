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

void UGOCharacterMovementComponent::SetFlashCommand()
{
	bPressedFlashSpell = true;
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
