// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
class AAuraPlayerState;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraCharacter();
	

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;


	// Combat Interface
	virtual int32 GetCharacterLevel_Implementation() override;
	virtual FVector GetMouseLocation() override;

	// Player Interface
	virtual void AddToXp_Implementation(int32 InXp) override;
	virtual void AddToPlayerLevel_Implementation(int32 NumLevel) override;
	virtual void AddToAttributePoint_Implementation(int32 NumAttributePoint) override;
	virtual void AddToSpellPoint_Implementation(int32 NumSpellPoint) override;
	virtual int32 GetCurrentXp_Implementation() const override;
	virtual const int32 GetLevelForXp_Implementation(const int32 TotalXp) override;
	virtual int32 GetAttributePointsReward_Implementation(const int32 Level) const override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPointsReward_Implementation(const int32 Level) const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual void LevelUp_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

private:

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
	
	//component
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY()
	TObjectPtr<AAuraPlayerState> AuraPlayerState;
};
