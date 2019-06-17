// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get patrol route data
	UBlackboardComponent* BlackboardComp{ OwnerComp.GetBlackboardComponent() };
	if (!ensure(BlackboardComp)) { return EBTNodeResult::Failed; }

	AAIController* AIController{ OwnerComp.GetAIOwner() };
	if (!ensure(AIController)) { return EBTNodeResult::Failed; }

	APawn* ControlledPawn{ AIController->GetPawn() };
	UPatrolRoute* PatrolRoute{ ControlledPawn->FindComponentByClass<UPatrolRoute>() };
	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }

	TArray<AActor*> PatrolPoints{ PatrolRoute->GetPatrolPoints() };
	if(PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("A guard is missing patrol points"))
		return EBTNodeResult::Failed;
	}

	// Set next waypoint
	int32 Index{ BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName) };
	AActor* Waypoint{ Cast<AActor>(BlackboardComp->GetValueAsObject(WaypointKey.SelectedKeyName)) };
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	// Cycle the index
	Index = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, Index);
	

	return EBTNodeResult::Succeeded;
}

