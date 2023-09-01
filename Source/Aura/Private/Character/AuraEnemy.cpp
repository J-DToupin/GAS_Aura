// Copyright JDgame


#include "Character/AuraEnemy.h"


// Sets default values
AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AAuraEnemy::HighLightActor_Implementation()
{
	ITargetInterface::HighLightActor_Implementation();

	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
	//UE_LOG(LogTemp, Warning, TEXT("HighLightActor %s"), *GetName());
	
}

void AAuraEnemy::UnHighLightActor_Implementation()
{
	ITargetInterface::UnHighLightActor_Implementation();

	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
	//UE_LOG(LogTemp, Warning, TEXT("UnHighLight %s"), *GetName());

}

// Called when the game starts or when spawned
void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}
