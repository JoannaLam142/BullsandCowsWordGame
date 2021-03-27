// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    FLevel Start;
    PrintLine(TEXT("Current Level is: %i, %i."), Start.MinLevel, Start.MaxLevel);
    Isogram = GetValidWords(Words, Start.MinLevel, Start.MaxLevel);//Must stay!! Stop taking it out
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameEnded)
    {
        if (bCompletedLevel)
        {
            ClearScreen();
            FLevel LevelUp = IncreaseLevel();
            Isogram = GetValidWords (Words, LevelUp.MinLevel, LevelUp.MaxLevel);
            SetupGame();
        }
        else
        {
            ClearScreen();
            FLevel StartAgain;
            Isogram = GetValidWords(Words, StartAgain.MinLevel, StartAgain.MaxLevel);
            PrintLine (TEXT("Start Again at Level: %i"), StartAgain.MinLevel);
            SetupGame();
        }
    }
    else // Check PlayerGuess
    {
        ProcessGuess(Input);//And provide Hint
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine (TEXT("Hello! Welcome to Bulls and Cows."));// Welcoming Player
    HiddenWord = Isogram[FMath::RandRange (0, Isogram.Num() -1)];
    StartLives = HiddenWord.Len();
    CurrentLives = HiddenWord.Len();
    bGameEnded = false;
    Move = 0;

    PrintLine (TEXT("Guess the %i letter isogramic word.\nType in your guess.\nPress enter to continue."), HiddenWord.Len()); // Prompt Player
    PrintLine (TEXT("The hidden word is: %s"), *HiddenWord); // Debug Line
}

void UBullCowCartridge::EndGame()
{
    bGameEnded = true;
    bCompletedLevel = false;
    PrintLine (TEXT("MOO!! Game Over."));
    PrintLine (TEXT("The word was: %s."), *HiddenWord);
    PrintLine (TEXT("Press enter to play same level again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord) //Check if correct 
    {
        WinGame();
        return;
    }
    
    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("This hidden word is %i long.\nTry again."), HiddenWord.Len());
        PrintLine(TEXT("You still have %i lives remaining."), CurrentLives);
        return;
    }

    if (!IsIsogram(Guess))    //Check if Isogram
    {
        PrintLine (TEXT("There are no repeating letters guess again."));
        return;
    }

    if (CurrentLives <= 0) //Lives less than zero
    {
        ClearScreen();
        EndGame();
        return;
    }

    //Remove Life
    --CurrentLives;
    PrintLine (TEXT("You have lost a life. \nTry Again."));
    PrintLine (TEXT("You have %i Lives remaining."), CurrentLives);

    //Include a hint after so many lives lost
    HintTime = StartLives*0.75;
    // PrintLine (TEXT("HintTime number: %i"), HintTime);
    if (CurrentLives <= HintTime)
    {
        PrintLine(TEXT("Heres a hint, the %i letter is: %c"), Move + 1, HiddenWord[Move]);
        Move++;
        PrintLine(TEXT("Move Number: %i"), Move);
    }

    // Show Player Bulls and Cows
    FBullCowCount Score = GetBullsCows(Guess);
    PrintLine (TEXT("You have %i many Bulls.\nYou have %i many Cows"), Score.Bulls, Score.Cows);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index]==Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

TArray <FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList, int32 LowLevel, int32 HiLevel) const
{
    TArray<FString> ValidWords;//This is an Array that stores Valid words between 4 and 8 and Isograms 

    for (FString Word : WordList)
    {
         if (Word.Len() >= LowLevel && Word.Len() <= HiLevel && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullsCows (const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
    for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
        {
            Count.Cows++;
            break;
        }
    }
    }
    return Count;
}
//Have turn time
//on the second mistake turn have the game show the second letter etc. 

void UBullCowCartridge::WinGame()
{
    bGameEnded = true;
    bCompletedLevel = true;
    PrintLine (TEXT("Congraulations! You got the word right."));
    PrintLine (TEXT("Press enter to go to the next level."));
}

FLevel UBullCowCartridge::IncreaseLevel()
{
    FLevel LevelUp;

    LevelUp.MinLevel++;
    LevelUp.MaxLevel++;
    
    return LevelUp;
}