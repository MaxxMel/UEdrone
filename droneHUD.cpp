// Fill out your copyright notice in the Description page of Project Settings.
#include "droneHUD.h"
#include "Blueprint/UserWidget.h"
AdroneHUD::AdroneHUD() : Super() 
{
    ConstructorHelpers::FClassFinder<UUserWidget> WBP(TEXT("/Game/WBP.WBP"));
    if (WBP.Succeeded())
    {
        MainHUD = WBP.Class;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ErRor"));
    }
}
//                    /Script/UMGEditor.WidgetBlueprint'/Game/WBP.WBP'
void AdroneHUD::BeginPlay()
{
    Super :: BeginPlay(); 
    if (MainHUD)
    {
        UUserWidget* droneWidget = CreateWidget(GetWorld(), MainHUD);
        if (droneWidget) 
        {
            droneWidget->AddToViewport();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ErRor"));
        }
        
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ErRor"));
    }
    
}
