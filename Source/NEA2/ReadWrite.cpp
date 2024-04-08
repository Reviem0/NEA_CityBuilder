// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadWrite.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"
#include <iostream>
#include <string>
#include "../../ThirdParty/cryptopp/Includes/aes.h"
#include "../../ThirdParty/cryptopp/Includes/modes.h"
#include "../../ThirdParty/cryptopp/Includes/filters.h"
#include "../../ThirdParty/cryptopp/Includes/osrng.h"
#include "../../ThirdParty/cryptopp/Includes/hex.h"
#include <fstream>
#include <Shlobj.h> 

ReadWrite::ReadWrite()
{
}

ReadWrite::~ReadWrite()
{
}

FString ReadWrite::ReadFile(FString path)
{
    // read the file into a string
    FString content;
    FFileHelper::LoadFileToString(content, *path);
    return content;

}

void ReadWrite::WriteFile(FString path, FString content)
{
    // write the string to a file
	FFileHelper::SaveStringToFile(content, *path);
}



