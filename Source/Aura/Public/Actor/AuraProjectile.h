// Copyright JDgame

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UNiagaraComponent;
class USphereComponent;
class UProjectileMovementComponent;
UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAuraProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult & SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

};
