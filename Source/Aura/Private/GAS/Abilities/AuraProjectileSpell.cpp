// Copyright JDgame


#include "GAS/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	if (GetAvatarActorFromActorInfo()->HasAuthority())
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

		if (CombatInterface)
		{
			const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
			FTransform SpawnTransform;
			FRotator Rotator = (ProjectileTargetLocation - SocketLocation).Rotation();
			Rotator.Pitch = 0.f;
			
			SpawnTransform.SetLocation(SocketLocation);
			SpawnTransform.SetRotation(Rotator.Quaternion());
		
			AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(AuraProjectileClass, SpawnTransform,
				GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()),
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
			
			Projectile->DamageEffectSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		
			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}
