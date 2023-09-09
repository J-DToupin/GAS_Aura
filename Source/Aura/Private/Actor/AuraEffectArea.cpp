// Copyright JDgame


#include "Actor/AuraEffectArea.h"

#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
AAuraEffectArea::AAuraEffectArea()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(GetRootComponent());

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Niagara");
	NiagaraComponent->SetupAttachment(GetRootComponent());
}

void AAuraEffectArea::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
}

void AAuraEffectArea::EndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverLap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

// Called when the game starts or when spawned
void AAuraEffectArea::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectArea::OnOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectArea::EndOverLap);
	
}
