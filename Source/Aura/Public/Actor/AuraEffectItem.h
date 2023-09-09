// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "AuraEffectActor.h"
#include "AuraEffectItem.generated.h"

UCLASS()
class AURA_API AAuraEffectItem : public AAuraEffectActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAuraEffectItem();

	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void EndOverLap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
};
