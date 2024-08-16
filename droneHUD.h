// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "droneHUD.generated.h"
/**
 * 
 */
UCLASS()
class P3_API AdroneHUD : public AHUD
{
    GENERATED_BODY()
    
public: 
    AdroneHUD(); 
    virtual void BeginPlay() override; 
    TSubclassOf<class UUserWidget> MainHUD; 
};
