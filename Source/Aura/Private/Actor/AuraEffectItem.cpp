// Copyright JDgame


#include "Actor/AuraEffectItem.h"
#include "Components/SphereComponent.h"


// Sets default values
AAuraEffectItem::AAuraEffectItem()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SphereComponent->SetupAttachment(GetRootComponent());
}

void AAuraEffectItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	this->Destroy();
}

void AAuraEffectItem::EndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverLap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

// Called when the game starts or when spawned
void AAuraEffectItem::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectItem::OnOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectItem::EndOverLap);
}
