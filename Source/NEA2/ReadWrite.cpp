// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadWrite.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"
#include <iostream>
#include <string>
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



