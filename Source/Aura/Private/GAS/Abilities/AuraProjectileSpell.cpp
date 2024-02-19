// Copyright JDgame


#include "GAS/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Aura/Public/AuraGameplayTags.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	

	if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),
			FAuraGameplayTags::Get().CombatSocket_Weapon);
		FTransform SpawnTransform;
		FRotator Rotator = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotator.Pitch = 0.f;
		
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotator.Quaternion());
	
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(AuraProjectileClass, SpawnTransform,
			GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	
		Projectile->FinishSpawning(SpawnTransform);
	}
	
}
