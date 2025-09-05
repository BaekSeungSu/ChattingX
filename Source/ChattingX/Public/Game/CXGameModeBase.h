#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CXGameModeBase.generated.h"

class ACXPlayerController;

UCLASS()
class CHATTINGX_API ACXGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void BeginPlay() override;
	
	FString GenerateSecretNumber();
	bool IsGuessNumberString(const FString& InNumberString);
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
	
	void PrintChatMessageString(ACXPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	void IncreaseGuessCount(ACXPlayerController* InChattingPlayerController);

	void ResetGame();
	void JudgeGame(ACXPlayerController* InChattingPlayerController, int InStrikeCount);
protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ACXPlayerController>> AllPlayerControllers;
};
