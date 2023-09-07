// Copyright JDgame


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SphereComponent.h"
#include "GAS/AuraAbilitySystemComponent.h"


// Sets default values
AAuraEffectActor::AAuraEffectActor()
{

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SphereComponent->SetupAttachment(GetRootComponent());

	
}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && GamePlayEffectClass )
	{
		ApplyEffectToInstigator(OtherActor, GamePlayEffectClass);
		this->Destroy();
	}
}

void AAuraEffectActor::EndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::EndOverLap);
}

void AAuraEffectActor::ApplyEffectToInstigator(AActor* InstigatorActor, TSubclassOf<UGameplayEffect> GEClass)
{

	check(GEClass)
	
	UAbilitySystemComponent* InstigatorActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstigatorActor);

	if (InstigatorActorASC)
	{
		FGameplayEffectContextHandle EffectContextHandle = InstigatorActorASC->MakeEffectContext();
		// indique objet de la cause de l'effet
		EffectContextHandle.AddSourceObject(this);
	
		const FGameplayEffectSpecHandle EffectSpecHandle =  InstigatorActorASC->MakeOutgoingSpec(GEClass, 1.f, EffectContextHandle);
		InstigatorActorASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
	
}
