// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string>
/**
 * 
 */
class NEA2_API ReadWrite
{
public:
	ReadWrite();
	~ReadWrite();
	FString ReadFile(FString path);
	void WriteFile(FString path, FString content);
};
