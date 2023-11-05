// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindAndUseSmartObject_Enhanced.h"

#include "AIController.h"
#include "GameplayTagAssetInterface.h"
#include "SmartObjectSubsystem.h"

#include "AI/BTTask_FindAndUseGameplayBehaviorSmartObject.h"
#include "BlackboardKeyType_SOClaimHandle.h"
#include "AI/AITask_UseGameplayBehaviorSmartObject.h"
#include "GameplayBehaviorSmartObjectBehaviorDefinition.h"
#include "VisualLogger/VisualLogger.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvQueryItemType_SmartObject.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindAndUseSmartObject_Enhanced::UBTTask_FindAndUseSmartObject_Enhanced()
{
	NodeName = "Find and use smart object (enhanced)";
	Radius = 500.f;
	EQSQueryFinishedDelegate = FQueryFinishedSignature::CreateUObject(this, &UBTTask_FindAndUseSmartObject_Enhanced::OnQueryFinished);
	EQSRequest.RunMode = EEnvQueryRunMode::AllMatching;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_FindAndUseSmartObject_Enhanced::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	UWorld* World = GetWorld();
	USmartObjectSubsystem* SmartObjectSubsystem = USmartObjectSubsystem::GetCurrent(World);
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (SmartObjectSubsystem == nullptr || MyController == nullptr || MyController->GetPawn() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FBTUseSOTaskMemory* MyMemory = reinterpret_cast<FBTUseSOTaskMemory*>(NodeMemory);
	MyMemory->TaskInstance.Reset();
	MyMemory->EQSRequestID = INDEX_NONE;

	AActor& Avatar = *MyController->GetPawn();

	if (EQSRequest.IsValid() && (EQSRequest.EQSQueryBlackboardKey.IsSet() || EQSRequest.QueryTemplate))
	{
		const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
		MyMemory->EQSRequestID = EQSRequest.Execute(Avatar, BlackboardComponent, EQSQueryFinishedDelegate);

		if (MyMemory->EQSRequestID != INDEX_NONE)
		{
			NodeResult = EBTNodeResult::InProgress;
		}
	}
	else 
	{
		const FVector UserLocation = Avatar.GetActorLocation();

		// Create filter
		FSmartObjectRequestFilter Filter;
		Filter.ActivityRequirements = ActivityRequirements;
		Filter.BehaviorDefinitionClasses = { UGameplayBehaviorSmartObjectBehaviorDefinition::StaticClass() };
		const IGameplayTagAssetInterface* TagsSource = Cast<const IGameplayTagAssetInterface>(&Avatar);
		if (TagsSource != nullptr)
		{
			TagsSource->GetOwnedGameplayTags(Filter.UserTags);
		}

		// Create request
		FSmartObjectRequest Request(FBox(UserLocation, UserLocation).ExpandBy(FVector(Radius), FVector(Radius)), Filter);
		TArray<FSmartObjectRequestResult> Results;
		const FSmartObjectActorUserData ActorUserData(&Avatar);
		const FConstStructView ActorUserDataView(FConstStructView::Make(ActorUserData));
	
		if (SmartObjectSubsystem->FindSmartObjects(Request, Results, ActorUserDataView))
		{
			for (const FSmartObjectRequestResult& Result : Results)
			{
				FSmartObjectClaimHandle ClaimHandle = SmartObjectSubsystem->MarkSlotAsClaimed(Result.SlotHandle, ActorUserDataView);
				if (ClaimHandle.IsValid())
				{
					UseClaimedSmartObject(OwnerComp, ClaimHandle, *MyMemory);

					NodeResult = EBTNodeResult::InProgress;
					UE_VLOG_UELOG(MyController, LogSmartObject, Verbose, TEXT("%s claimed smart object: %s"), *GetNodeName(), *LexToString(ClaimHandle));
					break;
				}
			}

			UE_CVLOG_UELOG(NodeResult == EBTNodeResult::Failed, MyController, LogSmartObject, Warning, TEXT("%s failed to claim smart object"), *GetNodeName());
		}
		else
		{
			UE_VLOG_UELOG(MyController, LogSmartObject
				, Verbose, TEXT("%s failed to find smart objects for request: %s")
				, *GetNodeName(), *Avatar.GetName());
		}
	}

	return NodeResult;
}

EBTNodeResult::Type UBTTask_FindAndUseSmartObject_Enhanced::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	check(NodeMemory);

	FBTUseSOTaskMemory* MyMemory = reinterpret_cast<FBTUseSOTaskMemory*>(NodeMemory);
	if (UAITask_UseGameplayBehaviorSmartObject* UseSOTask = MyMemory->TaskInstance.Get())
	{
		UseSOTask->ExternalCancel();
		MyMemory->TaskInstance.Reset();
	}

	if (MyMemory->EQSRequestID != INDEX_NONE)
	{
		if (UWorld* World = OwnerComp.GetWorld())
		{
			if (UEnvQueryManager* EnvQueryManager = UEnvQueryManager::GetCurrent(World))
			{
				EnvQueryManager->AbortQuery(MyMemory->EQSRequestID);
			}
		}
		MyMemory->EQSRequestID = INDEX_NONE;
	}

	return EBTNodeResult::Aborted;
}

void UBTTask_FindAndUseSmartObject_Enhanced::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	FBTUseSOTaskMemory* MyMemory = reinterpret_cast<FBTUseSOTaskMemory*>(NodeMemory);
	if (UAITask_UseGameplayBehaviorSmartObject* UseSOTask = MyMemory->TaskInstance.Get())
	{
		check(UseSOTask->IsFinished());
	}
	check(MyMemory->EQSRequestID == INDEX_NONE);
}

FString UBTTask_FindAndUseSmartObject_Enhanced::GetStaticDescription() const
{
	FString Result;
	if (ActivityRequirements.IsEmpty() == false)
	{
		Result += FString::Printf(TEXT("Object requirements: %s")
			, *ActivityRequirements.GetDescription());
	}

	return Result.Len() > 0
		? Result
		: Super::GetStaticDescription();
}

void UBTTask_FindAndUseSmartObject_Enhanced::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	EQSRequest.InitForOwnerAndBlackboard(*this, GetBlackboardAsset());
}

void UBTTask_FindAndUseSmartObject_Enhanced::OnQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	if (!Result)
	{
		return;
	}

	AActor* MyOwner = Cast<AActor>(Result->Owner.Get());
	if (APawn* PawnOwner = Cast<APawn>(MyOwner))
	{
		MyOwner = PawnOwner->GetController();
	}

	UBehaviorTreeComponent* BTComponent = MyOwner ? MyOwner->FindComponentByClass<UBehaviorTreeComponent>() : NULL;
	if (!BTComponent)
	{
		return;
	}

	const FEnvQueryResult& QueryResult = *Result.Get();

	uint8* RawMemory = BTComponent->GetNodeMemory(this, BTComponent->FindInstanceContainingNode(this));
	check(RawMemory);
	FBTUseSOTaskMemory* MyMemory = reinterpret_cast<FBTUseSOTaskMemory*>(RawMemory);
	if (MyMemory->EQSRequestID != QueryResult.QueryID)
	{
		UE_VLOG_UELOG(BTComponent, LogBehaviorTree, Log, TEXT("%s [%s] ignoring EQS result due to QueryID mismatch.")
			, ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(MyOwner));

		check(MyMemory->EQSRequestID != INDEX_NONE)

		return;
	}
	else if (QueryResult.IsAborted())
	{
		UE_VLOG_UELOG(BTComponent, LogBehaviorTree, Log, TEXT("%s [%s] observed EQS query finished as Aborted. Aborting the BT node as well.")
			, ANSI_TO_TCHAR(__FUNCTION__), *GetNameSafe(MyOwner));

		FinishLatentTask(*BTComponent, EBTNodeResult::Aborted);
		return;
	}

	// at this point we've already confirmed that QueryResult does indeed corresponds to the the query we're waiting for
	// so we need to clear the EQSRequestID here in case the next task we're about to issue (the UAITask_UseGameplayBehaviorSmartObject)
	// might fail instantly and we do check EQSRequestID on tasks end to make sure everything has been cleaned up properly.
	MyMemory->EQSRequestID = INDEX_NONE;

	bool bSmartObjectClaimed = false;

	if (QueryResult.IsSuccessful() && (QueryResult.Items.Num() >= 1))
	{
		if (QueryResult.ItemType->IsChildOf(UEnvQueryItemType_SmartObject::StaticClass()) == false)
		{
			UE_VLOG_UELOG(BTComponent, LogSmartObject, Error, TEXT("%s used EQS query that did not generate EnvQueryItemType_SmartObject items"), *GetNodeName());
		}
		else if (USmartObjectSubsystem* SmartObjectSubsystem = USmartObjectSubsystem::GetCurrent(MyOwner->GetWorld()))
		{
			const FSmartObjectActorUserData ActorUserData(Cast<AActor>(Result->Owner.Get()));
			const FConstStructView ActorUserDataView(FConstStructView::Make(ActorUserData));

			// we could use QueryResult.GetItemAsTypeChecked, but the below implementation is more efficient
			for (int i = 0; i < QueryResult.Items.Num(); ++i)
			{
				const FSmartObjectSlotEQSItem& Item = UEnvQueryItemType_SmartObject::GetValue(QueryResult.GetItemRawMemory(i));
				const FSmartObjectClaimHandle ClaimHandle = SmartObjectSubsystem->MarkSlotAsClaimed(Item.SlotHandle, ActorUserDataView);
				if (ClaimHandle.IsValid())
				{
					UseClaimedSmartObject(*BTComponent, ClaimHandle, *MyMemory);
					bSmartObjectClaimed = true;

					UE_VLOG_UELOG(BTComponent, LogSmartObject, Verbose, TEXT("%s claimed EQS-found smart object: %s"), *GetNodeName(), *LexToString(ClaimHandle));
					break;
				}
			}
		}
	}
	
	if (bSmartObjectClaimed == false)
	{
		FinishLatentTask(*BTComponent, EBTNodeResult::Failed);
	}
}

void UBTTask_FindAndUseSmartObject_Enhanced::UseClaimedSmartObject(UBehaviorTreeComponent& OwnerComp, FSmartObjectClaimHandle ClaimHandle,
	FBTUseSOTaskMemory& MyMemory)
{
	checkSlow(ClaimHandle.IsValid());
	// UAITask_UseGameplayBehaviorSmartObject* UseSOTask = NewBTAITask<UAITask_UseGameplayBehaviorSmartObject>(OwnerComp);
	// UseSOTask->SetClaimHandle(ClaimHandle);
	UAITask_UseGameplayBehaviorSmartObject* UseSOTask = UAITask_UseGameplayBehaviorSmartObject::MoveToAndUseSmartObjectWithGameplayBehavior(OwnerComp.GetAIOwner(),
		ClaimHandle, false);
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_SOClaimHandle>(SOClaimHandleBBKey.SelectedKeyName, ClaimHandle);
	UseSOTask->ReadyForActivation();

	MyMemory.TaskInstance = UseSOTask;
}
