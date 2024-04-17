// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_LossScreen.h"
#include "Kismet/GameplayStatics.h"
#include "../NEA2GameModeBase.h"
#include "../GameManager.h"
#include "../ReadWrite.h"
#include <Shlobj.h> // Include the necessary header file



void UCB_LossScreen::NativeConstruct()
{
    Super::NativeConstruct();
    // Bind the buttons to their respective functions
    MainMenuButton->OnClicked.AddUniqueDynamic(this, &UCB_LossScreen::OnMainMenuButtonClicked);
    SubmitScoreButton->OnClicked.AddUniqueDynamic(this, &UCB_LossScreen::OnSubmitScoreButtonClicked);

    // Get the player's time and points
    GetTime();
    GetPoints();

    // Create a dictionary to store player names and scores
    PlayerScoreMap = {
        { Player1, Score_1 },
        { Player2, Score_2 },
        { Player3, Score_3 },
        { Player4, Score_4 },
        { Player5, Score_5 },
        { Player6, Score_6 },
        { Player7, Score_7 },
        { Player8, Score_8 },
        { Player9, Score_9 },
        { Player10, Score_10 }
    };

    // Get the leaderboard every second
    GetLeaderboard();
    FTimerHandle LeaderboardTimer;

    // Update the leaderboard every second
    GetWorld()->GetTimerManager().SetTimer(LeaderboardTimer, this, &UCB_LossScreen::GetLeaderboard, 1.0f, true);

}

void UCB_LossScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

}

void UCB_LossScreen::GetTime()
{
    ANEA2GameModeBase* GameMode = Cast<ANEA2GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

    // Get the final time
    if (GameMode) {
        FinalTime = GameMode->time;
        // Display the final time
        TimeLabel->SetText(FText::AsNumber(FinalTime));
    }
}

void UCB_LossScreen::GetPoints()
{
    AGameManager* GameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));

    // Get the final score
    if (GameManager) {
        FinalScore = GameManager->GetScore();
        // Display the final score
        PointsLabel->SetText(FText::AsNumber(FinalScore));
    }
}

void UCB_LossScreen::OnMainMenuButtonClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}

void UCB_LossScreen::OnSubmitScoreButtonClicked()
{
    try
    {
        // Get the player name and score
        FString pname = PlayerName->GetText().ToString();
        int score = FinalScore;

        // Validate the player name
        bool IsAlnum = true;
        for (const TCHAR& Char : pname)
        {
            // Check if the character is alphanumeric
            if (!FChar::IsAlnum(Char))
            {
                IsAlnum = false;
                break;
            }
        }
        if (!IsAlnum || pname.IsEmpty()) // If the player name is not alphanumeric or empty, throw an exception
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid player name. Only alphanumeric characters are allowed."));
            throw ("Invalid player name. \n Only alphanumeric characters are allowed.");
        }

        // Log for debugging purposes
        UE_LOG(LogTemp, Warning, TEXT("Submit score button clicked"));

        // Format the URL to include the player name and score
        FString url = FString::Printf(TEXT("http://dreamlo.com/lb/p_oRtzwC5kG87-JML4pFXwaQpwt7gjE0KlWrFeYUfIAw/add/%s/%d"), *pname, score);

        // Create a POST request to submit the player name and score
        FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
        Request->SetURL(url);
        Request->SetVerb("POST");

        // Bind the function to update the leaderboard once the request is complete
        Request->ProcessRequest();

        // Update the leaderboard
        GetLeaderboard();

        // Hide the submit score button and error label
        SubmitScoreButton->SetVisibility(ESlateVisibility::Hidden);
        ErrorLabel->SetVisibility(ESlateVisibility::Hidden);
    }
    catch(const char* e)
    {
        // Display the error message
        ErrorLabel->SetText(FText::FromString(FString(e)));
    }
    catch(const std::exception& e)
    {
        // Display the error message
        ErrorLabel->SetText(FText::FromString(FString(e.what())));
    }
    
    
}


void UCB_LossScreen::GetLeaderboard()
{
    // Create a GET request to get the leaderboard
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
    Request->SetURL("http://dreamlo.com/lb/6614785b8f40bb8118153adb/json");
    Request->SetVerb("GET");

    // Bind the function to update the leaderboard once the request is complete
    Request->OnProcessRequestComplete().BindUObject(this, &UCB_LossScreen::UpdateLeaderboardEntry);

    // Process the request
    Request->ProcessRequest();
}

void UCB_LossScreen::UpdateLeaderboardEntry(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    // Get the response content
    FString fileContent;
    if (bWasSuccessful) {
        fileContent = Response->GetContentAsString();
    }
    else {
        // Display an error message if the request was unsuccessful
        fileContent = "Failed to get leaderboard";
    }

    // Parse the JSON data
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(fileContent);
    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        // Get the leaderboard entries
        TArray<TSharedPtr<FJsonValue>> entries = JsonObject->GetObjectField("dreamlo")->GetObjectField("leaderboard")->GetArrayField("entry");

        // Create a map to store player names and scores
        TMap<FString, int> playerScores;

        // Iterate over each entry
        for (auto& entryVal : entries) {
            TSharedPtr<FJsonObject> entry = entryVal->AsObject();

            // Get the player name and score
            FString playerName = entry->GetStringField("name");
            int score = FCString::Atoi(*entry->GetStringField("score"));

            // Add the player name and score to the map
            playerScores.Add(playerName, score);
        }

        // Sort the map by value
        playerScores.ValueSort([](const int& A, const int& B) {
            return A > B;
            });

        // Update the leaderboard
        int i = 0;
        for (auto& elem : PlayerScoreMap) {
            int v = 0;
            for (auto& playerScore : playerScores) {
                if (v == i) {
                    elem.Key->SetText(FText::FromString(playerScore.Key));
                    elem.Value->SetText(FText::AsNumber(playerScore.Value));
                }
                v++;
            }
            i++;
        }
    }
}


