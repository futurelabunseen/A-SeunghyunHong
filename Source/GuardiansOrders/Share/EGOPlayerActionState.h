// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
namespace EGOPlayerActionState
{
	/**
		* 플레이어 캐릭터 GOPlayerCharacter 가 배틀 중 가질 수 있는 상태입니다.
		* Move:				이동 중일 때 활성화됩니다.
		* Flash:				점멸 시 활성화됩니다.
		* Cast:				스킬이나 공격을 시전할 때 활성화됩니다.
		* Impacted:		공격을 받아 피해를 입었을 때 활성화됩니다. /피해 반응 애니메이션 재생
		* Blown:			큰 충격을 받아 넉백되거나 날아갔을 때 활성화됩니다 /복구 시간 필요
		* Invincible:		캐릭터가 무적 상태일 때 활성화됩니다.
		* Died:				Hp가 0이 되어 사망했을 때 활성화됩니다. /플레이 불가 /부활 로직 처리
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
	* 캐릭터가 Impacted, Blown, Cast, Died 중 하나 이상의 상태에 있을 때,
	* 새로운 명령을 받지 않거나 실행하지 않음을 나타냅니다.
	* 예) 캐릭터가 공격을 받아 영향을 받는 중이거나 사망한 상태에서는 새로운 이동이나 스킬 사용 명령을 받아들일 수 없습니다.
*/
namespace EGOPlayerActionStateValue
{
	static uint8 OrderUnAcceptableBitMask =
		EGOPlayerActionState::Impacted |
		EGOPlayerActionState::Cast |
		EGOPlayerActionState::Died;
}