// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOHeroAnimInstance.h"
#include "GOKatnissAnimInstance.generated.h"

/**
 /Script/Engine.SkeletalMesh'/Game/Resource/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow'
 */
UCLASS()
class GUARDIANSORDERS_API UGOKatnissAnimInstance : public UGOHeroAnimInstance
{
	GENERATED_BODY()
	
public:
	UGOKatnissAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

// �ִ� �׷����� ������ ������
	/*
	// AnimInstance �� �����ϴ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Character)
	TObjectPtr<class ACharacter> Owner;

	// ĳ���� �����Ʈ ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// ���� ĳ������ �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	// �������� ĳ������ �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	// ���� ĳ���Ͱ� Idle ��������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	// ���� �ִ���? �����̰� �ִ���?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	// ���� �������� �ִ���?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	// ���� �����ϰ� �ִ���?
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	// ���� ���� �������� ��Ÿ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;
	*/
};
