// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GOHeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANSORDERS_API UGOHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:
	UGOHeroAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// �ִ� �׷����� ������ ������

	// AnimInstance �� �����ϴ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
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
};
