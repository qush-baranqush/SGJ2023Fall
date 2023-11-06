
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SGJ2023FALL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHealthComponent();

	FOnDeath OnDeath;

	bool IsAlive() const { return Health > 0.f; }
	
protected:

	virtual void BeginPlay() override;

    void SetHealth(float NewHealth, float MaxHealthToRecovery);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const { return Health; };

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead();

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool AddHealth(float AddHealth);

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsHealthFull();

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent();


	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", Meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float MaxHealth = 100.f;

	

public:	

private:
	float Health;

	bool CompletlyDead{ false };

private:
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

};
