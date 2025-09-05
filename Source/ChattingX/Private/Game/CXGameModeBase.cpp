#include "Game/CXGameModeBase.h"
#include "Game/CXGameStateBase.h"
#include "Player/CXPlayerController.h"
#include "Player/CXPlayerState.h"
#include "EngineUtils.h"

void ACXGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ACXPlayerController* CXPlayerController = Cast<ACXPlayerController>(NewPlayer);
	if (IsValid(CXPlayerController) == true)
	{
		CXPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server"));
		AllPlayerControllers.Add(CXPlayerController);

		ACXPlayerState* CXPlayerState = CXPlayerController->GetPlayerState<ACXPlayerState>();
		if (IsValid(CXPlayerState) == true)
		{
			CXPlayerState->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ACXGameStateBase* CXGameStateBase = GetGameState<ACXGameStateBase>();
		if (IsValid(CXGameStateBase) == true)
		{
			CXGameStateBase->MulticastRPCBroadcastLoginMessage(CXPlayerState->PlayerNameString);
		}
	}
}

void ACXGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
}

FString ACXGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for(int32 i=1; i<= 9; ++i)
	{
		Numbers.Add(i);
	}
	
	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) {return Num>0;});

	FString Result;
	for (int32 i=0; i<3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ACXGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do
	{
		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C =='0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;
		
	}while (false);

	return bCanPlay;
}

FString ACXGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i=0; i<3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}



void ACXGameModeBase::PrintChatMessageString(ACXPlayerController* InChattingPlayerController,
	const FString& InChatMessageString)
{
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		IncreaseGuessCount(InChattingPlayerController);
		
		for (TActorIterator<ACXPlayerController> It(GetWorld()); It; ++It)
		{
			ACXPlayerController* CXPlayerController = *It;
			if (IsValid(CXPlayerController) == true)
			{
				FString CombineMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				CXPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);

				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<ACXPlayerController> It(GetWorld()); It; ++It)
		{
			ACXPlayerController* CXPlayerController = *It;
			if (IsValid(CXPlayerController) == true)
			{
				CXPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void ACXGameModeBase::IncreaseGuessCount(ACXPlayerController* InChattingPlayerController)
{
	ACXPlayerState* CXPlayerState = InChattingPlayerController->GetPlayerState<ACXPlayerState>();
	if (IsValid(CXPlayerState) == true)
	{
		CXPlayerState->CurrentGuessCount++;
	}
}

void ACXGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& CXPlayerController : AllPlayerControllers)
	{
		ACXPlayerState* CXPlayerState = CXPlayerController->GetPlayerState<ACXPlayerState>();
		if (IsValid(CXPlayerState) == true)
		{
			CXPlayerState->CurrentGuessCount = 0;
		}
	}
}

void ACXGameModeBase::JudgeGame(ACXPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (InStrikeCount==3)
	{
		ACXPlayerState* CXPlayerState = InChattingPlayerController->GetPlayerState<ACXPlayerState>();
		for (const auto& CXPlayerController : AllPlayerControllers)
		{
			if (IsValid(CXPlayerController) == true)
			{
				FString CombinedMessageString = CXPlayerState->PlayerNameString + TEXT(" has won the game.");
				CXPlayerController->NotificationText = FText::FromString(CombinedMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& CXPlayerController : AllPlayerControllers)
		{
			ACXPlayerState* CXPlayerState = CXPlayerController->GetPlayerState<ACXPlayerState>();
			if (IsValid(CXPlayerState) == true)
			{
				if (CXPlayerState->CurrentGuessCount < CXPlayerState->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
			
		}

		if (bIsDraw == true)
		{
			for (const auto& CXPlayerController : AllPlayerControllers)
			{
				CXPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
				ResetGame();
			}
		}
	}
}
