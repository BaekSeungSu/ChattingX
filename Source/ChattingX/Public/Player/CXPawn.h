#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CXPawn.generated.h"

UCLASS()
class CHATTINGX_API ACXPawn : public APawn
{
	GENERATED_BODY()

public:
	ACXPawn();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
