
// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Bot_Controller.h"
#include "AI_Bot_Enemy1.h"
#include "SpaceShooterCharacter.h"
#include "GenericTeamAgentInterface.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"




AAI_Bot_Controller::AAI_Bot_Controller()

{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	{
		// Team 5
		SetGenericTeamId(FGenericTeamId(5));
	}

	// Create sight component
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	// Configure AI sightraduis
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILosesRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfview;
	SightConfig->SetMaxAge(AISightAge);

	// Detect other charcater by affiliation
	SightConfig->DetectionByAffiliation.bDetectEnemies = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// Get perception component
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAI_Bot_Controller::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

// This pawn is hostile to other team members outside its own team. (Team 5)
ETeamAttitude::Type AAI_Bot_Controller::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other)) {

		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
			if (OtherTeamID == 10) {
				return ETeamAttitude::Neutral;
			}
			else {
				return ETeamAttitude::Hostile;
			}
		}
	}

	return ETeamAttitude::Neutral;
}

void AAI_Bot_Controller::BeginPlay()
{
	Super::BeginPlay();

	// Get perception component or log probelm
	if (GetPerceptionComponent() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Systems Set"));
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Some Problem Occured"));
	}
}

void AAI_Bot_Controller::OnPossess(APawn* OnPawn)
{
	// Can posses a pawn as a controller
	Super::OnPossess(OnPawn);
}
void AAI_Bot_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AAI_Bot_Enemy1* Actor = Cast<AAI_Bot_Enemy1>(GetPawn());

	if (DistanceToPlayer > AISightRadius)
	{
		// Check this pawns detection
		bIsPlayerDetected = false;

	}

	// if detection is true. Move to actor
	if (bIsPlayerDetected == true)
	{
		AActor* Player = Cast< AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		MoveToActor(Player, 1.0f);
	}
}

FRotator AAI_Bot_Controller::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		// Get this pawns rotaion and get this actors rotation
		return FRotator(0.0f, 0.0f, 0.0f);
	}
	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AAI_Bot_Controller::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		// On pawn detected get distance to the pawn
		DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);

		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);
	}
	bIsPlayerDetected = true;
}


