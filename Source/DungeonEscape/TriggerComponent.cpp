// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (moverActor) 
	{
		mover = moverActor->FindComponentByClass<UMover>();
		if(!mover)
		{
			UE_LOG(LogTemp, Error, TEXT("Mover component not found in Mover Actor"));
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Mover component found in Mover Actor"));
		}
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Mover Actor is not assigned in TriggerComponent"));
	}

	if (isTriggerActive)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapBegin);
		OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapEnd);
	}
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

}

void UTriggerComponent::Trigger(bool newTriggerValue)
{
	isTriggered = newTriggerValue;

	if (mover)
	{
		mover->SetShouldMove(newTriggerValue);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Mover component is not assigned in TriggerComponent"));
	}
}

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->ActorHasTag("PressurePlateActivator"))
	{
		activatorCount++;
		if(!isTriggered)
			Trigger(true);
	}
}

void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator"))
	{
		activatorCount--;
		if (activatorCount == 0 && isTriggered)
			Trigger(false);
	}
}


