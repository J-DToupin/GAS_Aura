// Copyright JDgame


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"


AAuraProjectile::AAuraProjectile()
{

	bReplicates = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionObjectType(ECC_Projectile);
	SphereComponent->SetCollisionResponseToChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SetRootComponent(SphereComponent);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Niagara");
	NiagaraComponent->SetupAttachment(GetRootComponent());

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::AAuraProjectile::OnOverlap);
	
	UGameplayStatics::SpawnSoundAttached(ProjectileSound,GetRootComponent(), NAME_None, FVector(ForceInit),
		FRotator::ZeroRotator,EAttachLocation::KeepRelativeOffset, true);
}

void AAuraProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	const AActor* SourceAvatarActor =  DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	if (SourceAvatarActor != OtherActor && UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor))
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());

		if (HasAuthority())
		{
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				DamageEffectParams.TargetAbilitySystemComponent = TargetASC;

				UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			
			}
			
			Destroy();
		}
		else
		{
			bHit = true;
		}
	}
}

void AAuraProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	}
	Super::Destroyed();
}
