// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UDebuffNiagaraComponent();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DebuffTag;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	void DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
};
