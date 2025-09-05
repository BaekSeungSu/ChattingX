#include "Player/CXPawn.h"

#include "ChattingX.h"

ACXPawn::ACXPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACXPawn::BeginPlay()
{
	Super::BeginPlay();
	FString NetRoleString = ChattingXFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::BeginPlay() %s [%s]"), *ChattingXFunctionLibrary::GetNetModeString(this), *NetRoleString);
	ChattingXFunctionLibrary::MyPrintString(this, CombinedString, 10.0f);
}

void ACXPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	FString NetRoleString = ChattingXFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::PossessedBy() %s [%s]"), *ChattingXFunctionLibrary::GetNetModeString(this), *NetRoleString);
	ChattingXFunctionLibrary::MyPrintString(this, CombinedString, 10.0f);
	
}

void ACXPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACXPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

