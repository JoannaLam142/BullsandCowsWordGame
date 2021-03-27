// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

struct FLevel
{
	int32 MinLevel = 2;
	int32 MaxLevel = 4;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void SetupGame();
	void EndGame();
	void ProcessGuess(const FString& Guess);
	bool IsIsogram(const FString& Word) const;
	TArray <FString> GetValidWords(const TArray <FString>& WordList, int32 LowLevel, int32 HiLevel)const;
	FBullCowCount GetBullsCows (const FString& Guess) const;
	void WinGame();
	FLevel IncreaseLevel ();

	// Your declarations go below!
	private:
	FString HiddenWord;
	int32 CurrentLives;
	bool bGameEnded;
	TArray<FString> Isogram;
	int32 HintTime;
	int32 StartLives;
	int32 Move;
	bool bCompletedLevel;
};
