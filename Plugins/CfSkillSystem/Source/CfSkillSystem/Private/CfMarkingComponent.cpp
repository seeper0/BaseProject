// Fill out your copyright notice in the Description page of Project Settings.


#include "CfMarkingComponent.h"

#include "CanvasItem.h"
#include "NativeGameplayTags.h"

// Sets default values for this component's properties
UCfMarkingComponent::UCfMarkingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SphereRadius = 5.0f;
}


// Called when the game starts
void UCfMarkingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}


// Called every frame
void UCfMarkingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// if (GEngine && GEngine->GameViewport)
	// {
	// 	FPrimitiveDrawInterface* PDI = GEngine->GameViewport->GetDebugCanvas()->GetPrimitiveDrawer();
	//
	// 	// 기즈모 그리기
	// 	PDI->DrawPoint(GetComponentLocation(), FColor::Red, 10.0f, SDPG_World);
	//
	// 	// 글자 그리기
	// 	FVector TextLocation = GetComponentLocation() + FVector(20.0f, 0.0f, 0.0f); // 글자 위치 조정
	// 	FString Text = TEXT("Marker"); // 표시할 텍스트
	// 	FCanvasTextItem TextItem(TextLocation, FText::FromString(Text), GEngine->GetSmallFont(), FColor::White);
	// 	GEngine->GameViewport->GetDebugCanvas()->DrawItem(TextItem);
	// }
}

