// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        SetupGame();
    }  
    else
    {
        ProcessGuess(PlayerInput);        
    }
    
}

void UBullCowCartridge::SetupGame()
{
    TArray<FString> ValidWords = GetValidWords(Words);
    HiddenWord = ValidWords[FMath::RandRange(0, ValidWords.Num()-1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    WelcomePlayer();
}

void UBullCowCartridge::WelcomePlayer()
{
    PrintLine(TEXT("Welcome to Bull Cow Game!"));
    PrintLine(TEXT("The hidden word is %i letters long."), HiddenWord.Len()); 
    PrintLine(TEXT("You have %i lives left."), Lives);
    PrintLine(TEXT("Type your guess and press Enter to continue."));

  //  PrintLine(TEXT("The Hidden Word is: %s."), *HiddenWord); // Just for debug purposes
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress Enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(Guess == HiddenWord)
    {
        PrintLine(TEXT("You won!"));
        EndGame();
        return;
    }

    int32 HiddenWordLength = HiddenWord.Len();
    int32 InputLength = Guess.Len();
    if(HiddenWordLength != InputLength)
    {
        PrintLine(TEXT("Wrong! Try guessing again.\nLives left: %i"), Lives);      
        PrintLine(TEXT("The hidden word is %i letters long."), HiddenWordLength);      
    }  

    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters! Guess again."));
        return;
    }

    --Lives;
    PrintLine(TEXT("You lost a life =("));

    if(Lives <= 0) 
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left."));
        PrintLine(TEXT("The hidden word was %s"), *HiddenWord);

        EndGame();
        return;             
    }    

    FBullCowCount Score = GetBullsCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows."), Score.Bulls, Score.Cows); 

    PrintLine(TEXT("Guess again, you have %i lives left."), Lives);      
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    int32 WordLength = Word.Len();
    for (int32 Index = 0; Index < WordLength; Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < WordLength; Comparison++)
        {
            if(Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }    

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString CurrentWord : WordList)
    {
        if (CurrentWord.Len() >= 4 && CurrentWord.Len() <= 8 && IsIsogram(CurrentWord))
        {
            ValidWords.Emplace(CurrentWord);           
        }        
    }
    
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullsCows(const FString& Guess) const
{
    FBullCowCount BullCowCount;
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            BullCowCount.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                BullCowCount.Cows++;
                break;
            }
        }       
    }
    return BullCowCount;
}