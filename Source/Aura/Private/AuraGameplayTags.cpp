// Copyright JDgame


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

//initialization static variable like global variable type Class::Variable
FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/*
	* Primary Attributes
	*/
	
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().
		AddNativeGameplayTag(FName("Attributes.Primary.Strength"),
			FString("Increases physical damage"));

	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().
		AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"),
			FString("Increases magical damage"));

	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().
		AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),
			FString("Increases Armor and Armor Penetration"));

	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().
		AddNativeGameplayTag(FName("Attributes.Primary.Vigor"),
			FString("Increases Health"));

	/*
	 * Secondary Attributes
	 */
	
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, improves Block Chance"));

	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"),
		FString("Ignores Percentage of enemy Armor, increases Critical Hit Chance"));

	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"),
		FString("Chance to cut incoming damage in half"));

	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to double damage plus critical hit bonus"));

	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus damage added when a critical hit is scored"));

	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduces Critical Hit Chance of attacking enemies"));

	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of Health regenerated every 1 second"));

	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of Mana regenerated every 1 second"));

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of Health obtainable"));

	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"),
		FString("Maximum amount of Mana obtainable"));


	/*
	* Input Tags
	*/
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button"));

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button"));

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("InputTag.1"),
		FString("Input Tag for 1 key"));
	
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("InputTag.2"),
		FString("Input Tag for 2 key"));

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("InputTag.3"),
		FString("Input Tag for 3 key"));

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("InputTag.4"),
		FString("Input Tag for 4 key"));


	/*
	* Damage
	*/

	GameplayTags.Damage = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Damage"),
		FString("Damage"));

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Damage.Fire"),
		FString("Fire Damage Type"));

	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Damage.Lightning"),
		FString("Lightning Damage Type"));

	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Damage.Arcane"),
		FString("Arcane Damage Type"));

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Damage.Physical"),
		FString("Physical Damage Type"));
	
	/*
	* Resistance
	*/

	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Resistance.Fire"),
		FString("Resistance to Fire damage"));

	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"),
		FString("Resistance to Lightning damage"));

	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"),
		FString("Resistance to Arcane damage"));

	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Resistance.Physical"),
		FString("Resistance to Physical damage"));

	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);

	/*
	* Debuffs
	*/
	
	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Debuff.Burn"),
		FString("Debuff for fire damage"));

	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Debuff.Stun"),
		FString("Debuff for Lightning damage"));

	GameplayTags.Debuff_Arcane = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Debuff.Arcane"),
		FString("Debuff for Arcane damage"));

	GameplayTags.Debuff_Physical = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Debuff.Physical"),
		FString("Debuff for  Physical damage"));

	GameplayTags.DamageTypeDebuffs.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypeDebuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypeDebuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypeDebuffs.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);


	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Debuff.Chance"),
		FString("Debuff Chance"));

	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Debuff.Damage"),
		FString("Debuff Damage"));


	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Debuff.Frequency"),
		FString("Debuff Frequency"));


	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Debuff.Duration"),
		FString("Debuff Duration"));
	
	/*
	* Meta
	*/

	GameplayTags.Attributes_Meta_IncomingXp = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Meta.IncomingXp"),
		FString("Meta IncomingXp"));

	/*
	* Combat Socket
	*/
	
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("CombatSocket.Weapon"),
		FString("Weapon Socket"));

	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("CombatSocket.RightHand"),
		FString("Right Hand Socket"));

	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("CombatSocket.LeftHand"),
		FString("Left Hand Socket"));

	/*
	* Montage
	*/

	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Effects.HitReact"),
		FString("Add this Tag when Hit"));

	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Montage.Attack.1"),
		FString("Montage Attack 1"));

	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Montage.Attack.2"),
		FString("Montage Attack 2"));
	
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Montage.Attack.3"),
		FString("Montage Attack 3"));

	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Montage.Attack.4"),
		FString("Montage Attack 4"));

	
	/*
	* Abilities
	*/

	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Abilities.Attack"),
		FString("Attack Ability Tag"));

	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Abilities.FIre.FireBolt"),
		FString("FireBolt Ability Tag"));


	//Cooldown
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Cooldown.FIre.FireBolt"),
		FString("Cooldown. FireBolt Ability Tag"));

	
}
