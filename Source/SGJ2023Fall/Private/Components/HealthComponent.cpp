
#include "Components/HealthComponent.h"
#include "GameFramework/Actor.h"

UHealthComponent::UHealthComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
}


void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!GetWorld() && Damage <= 0 || IsDead())
		return;
	SetHealth(Health - Damage, MaxHealth);
	if (IsDead() && !CompletlyDead)
	{
		CompletlyDead = true;
		OnDeath.Broadcast();
	}
}

void UHealthComponent::SetHealth(float NewHealth, float MaxHealthToRecovery)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
}

bool UHealthComponent::IsDead()
{
	return FMath::IsNearlyZero(Health);
}

bool UHealthComponent::AddHealth(float AddHealth)
{
	if (IsHealthFull() || IsDead())
	return false;

	SetHealth(Health + AddHealth, MaxHealth);
	return true;
}

bool UHealthComponent::IsHealthFull()
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

