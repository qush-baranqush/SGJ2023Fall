// 


#include "BTService_UpdatePerception.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "SGJ2023Fall/SGJ2023FallCharacter.h"

UBTService_UpdatePerception::UBTService_UpdatePerception()
{
	NodeName = "Update perception";
	TargetBBKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdatePerception, TargetBBKey), AActor::StaticClass());
}

void UBTService_UpdatePerception::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (auto PerceptionComponent = OwnerComp.GetAIOwner()->GetAIPerceptionComponent())
	{
		// TODO ... dude, this is game jam
		TArray<AActor*> SeenActors;
		PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SeenActors);
		for (auto* SeenActor : SeenActors)
		{
			if (SeenActor->IsA<ASGJ2023FallCharacter>())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetBBKey.SelectedKeyName, SeenActor);
				SetNextTickTime(NodeMemory, 5.f);
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->ClearValue(TargetBBKey.SelectedKeyName);
}
