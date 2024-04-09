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
    MainMenuButton->OnClicked.AddUniqueDynamic(this, &UCB_LossScreen::OnMainMenuButtonClicked);
    SubmitScoreButton->OnClicked.AddUniqueDynamic(this, &UCB_LossScreen::OnSubmitScoreButtonClicked);

    GetTime();
    GetPoints();

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
    GetWorld()->GetTimerManager().SetTimer(LeaderboardTimer, this, &UCB_LossScreen::GetLeaderboard, 1.0f, true);

}

void UCB_LossScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

}

void UCB_LossScreen::GetTime()
{
    ANEA2GameModeBase* GameMode = Cast<ANEA2GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode) {
        FinalTime = GameMode->time;
        TimeLabel->SetText(FText::AsNumber(FinalTime));
    }
}

void UCB_LossScreen::GetPoints()
{
    AGameManager* GameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
    if (GameManager) {
        FinalScore = GameManager->GetScore();
        PointsLabel->SetText(FText::AsNumber(FinalScore));
    }
}

void UCB_LossScreen::OnMainMenuButtonClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}

void UCB_LossScreen::OnSubmitScoreButtonClicked()
{
    // Get the player name and score
    FString pname = PlayerName->GetText().ToString();
    int score = FinalScore;
    UE_LOG(LogTemp, Warning, TEXT("Submit score button clicked"));

    // Format the URL to include the player name and score
    FString url = FString::Printf(TEXT("http://dreamlo.com/lb/p_oRtzwC5kG87-JML4pFXwaQpwt7gjE0KlWrFeYUfIAw/add/%s/%d"), *pname, score);

    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(url);
    Request->SetVerb("POST");
    Request->ProcessRequest();
    GetLeaderboard();

    SubmitScoreButton->SetVisibility(ESlateVisibility::Hidden);
}


void UCB_LossScreen::GetLeaderboard()
{
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
    Request->SetURL("http://dreamlo.com/lb/6614785b8f40bb8118153adb/json");
    Request->SetVerb("GET");
    Request->OnProcessRequestComplete().BindUObject(this, &UCB_LossScreen::UpdateLeaderboardEntry);
    Request->ProcessRequest();
}

void UCB_LossScreen::UpdateLeaderboardEntry(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    FString fileContent;
    if (bWasSuccessful) {
        fileContent = Response->GetContentAsString();
    }
    else {
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


