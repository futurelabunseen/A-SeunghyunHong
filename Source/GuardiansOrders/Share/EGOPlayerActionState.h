// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
namespace EGOPlayerActionState
{
	/**
		* �÷��̾� ĳ���� GOPlayerCharacter �� ��Ʋ �� ���� �� �ִ� �����Դϴ�.
		* Move:				�̵� ���� �� Ȱ��ȭ�˴ϴ�.
		* Flash:				���� �� Ȱ��ȭ�˴ϴ�.
		* Cast:				��ų�̳� ������ ������ �� Ȱ��ȭ�˴ϴ�.
		* Impacted:		������ �޾� ���ظ� �Ծ��� �� Ȱ��ȭ�˴ϴ�. /���� ���� �ִϸ��̼� ���
		* Blown:			ū ����� �޾� �˹�ǰų� ���ư��� �� Ȱ��ȭ�˴ϴ� /���� �ð� �ʿ�
		* Invincible:		ĳ���Ͱ� ���� ������ �� Ȱ��ȭ�˴ϴ�.
		* Died:				Hp�� 0�� �Ǿ� ������� �� Ȱ��ȭ�˴ϴ�. /�÷��� �Ұ� /��Ȱ ���� ó��
	*/

	enum State : int
	{
		None = 0 UMETA(Hidden),

		Move =				0x1,
		Flash =				0x1 << 1,
		Cast =				0x1 << 2,
		Impacted =			0x1 << 3,
		Blown =				0x1 << 4,
		Invincible =			0x1 << 5,
		Died =				0x1 << 6,

		Max = -1 UMETA(Hidden)
	};
}


/**
	* ĳ���Ͱ� Impacted, Blown, Cast, Died �� �ϳ� �̻��� ���¿� ���� ��,
	* ���ο� ����� ���� �ʰų� �������� ������ ��Ÿ���ϴ�.
	* ��) ĳ���Ͱ� ������ �޾� ������ �޴� ���̰ų� ����� ���¿����� ���ο� �̵��̳� ��ų ��� ����� �޾Ƶ��� �� �����ϴ�.
*/
namespace EGOPlayerActionStateValue
{
	static uint8 OrderUnAcceptableBitMask =
		EGOPlayerActionState::Impacted |
		EGOPlayerActionState::Cast |
		EGOPlayerActionState::Died;
}