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

    UpdateLeaderboard();

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
    UE_LOG(LogTemp, Warning, TEXT("Submit score button clicked"));
    ReadWrite* rw = new ReadWrite();
    TCHAR path[MAX_PATH];
    HRESULT hr = SHGetFolderPath(NULL, CSIDL_COMMON_DOCUMENTS, NULL, SHGFP_TYPE_CURRENT, path);

    if (FAILED(hr)) {
        UE_LOG(LogTemp, Error, TEXT("Failed to get the documents folder path"));
    }
    // Convert the TCHAR array to a FString
    FString pathString = FString(path);
    // log the path to the console
    
    FString filePath = pathString + TEXT("\\NEA\\Scores.txt");
    UE_LOG(LogTemp, Warning, TEXT("Path: %s"), *filePath);
    FString playerNameString = PlayerName->GetText().ToString();
    // READ THE FILE
    FString fileContent = rw->ReadFile(filePath);
    // Append the new score to the file
    fileContent += playerNameString + TEXT(" ") + FString::FromInt(FinalScore) + TEXT("\n");
    // Write the new content to the file
    rw->WriteFile(filePath, fileContent);

    UpdateLeaderboard();

    // delete button
    SubmitScoreButton->SetVisibility(ESlateVisibility::Hidden);

}

void UCB_LossScreen::UpdateLeaderboard()
{
    ReadWrite* rw = new ReadWrite();
    TCHAR path[MAX_PATH];
    HRESULT hr = SHGetFolderPath(NULL, CSIDL_COMMON_DOCUMENTS, NULL, SHGFP_TYPE_CURRENT, path);

    if (FAILED(hr)) {
        UE_LOG(LogTemp, Error, TEXT("Failed to get the documents folder path"));
    }
    // Convert the TCHAR array to a FString
    FString pathString = FString(path);
    // log the path to the console

    FString filePath = pathString + TEXT("\\NEA\\Scores.txt");
    UE_LOG(LogTemp, Warning, TEXT("Path: %s"), *filePath);
    // READ THE FILE
    FString fileContent = rw->ReadFile(filePath);
    // Split the file content into lines
    TArray<FString> lines;
    fileContent.ParseIntoArray(lines, TEXT("\n"), true);
    // Create a map to store the player names and scores
    TMap<FString, int> playerScores;
    // Iterate through the lines and split them into player name and score
    for (FString line : lines) {
        TArray<FString> parts;
        line.ParseIntoArray(parts, TEXT(" "), true);
        if (parts.Num() == 2) {
            FString playerName = parts[0];
            int score = FCString::Atoi(*parts[1]);
            playerScores.Add(playerName, score);
        }
    }
    // Sort the map by value
    playerScores.ValueSort([this](int A, int B) {
        return A > B;
    });
    // Iterate through the map and update the leaderboard
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
