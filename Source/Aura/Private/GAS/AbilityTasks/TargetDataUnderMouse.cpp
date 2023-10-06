// Copyright JDgame


#include "GAS/AbilityTasks/TargetDataUnderMouse.h"


UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	FHitResult CursorHit;
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	ValidDataDelegate.Broadcast(CursorHit.Location);
}
