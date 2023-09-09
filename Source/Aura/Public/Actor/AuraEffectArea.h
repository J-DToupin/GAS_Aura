// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "AuraEffectActor.h"
#include "AuraEffectArea.generated.h"

class UNiagaraComponent;
class UBoxComponent;

UCLASS()
class AURA_API AAuraEffectArea : public AAuraEffectActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAuraEffectArea();

	
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void EndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
};
