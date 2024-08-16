// Fill out your copyright notice in the Description page of Project Settings.
#include "drone.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SceneCaptureComponentCube.h"
#include "Engine/TextureRenderTarget2D.h"
#include "droneHUD.h"
#include "Blueprint/UserWidget.h"
// Sets default values
Adrone::Adrone()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    RootComponent = Mesh;
    SpringArm->SetupAttachment(Mesh);
    Camera->SetupAttachment(SpringArm);
    Mesh->SetSimulatePhysics(true);
    // Создание и инициализация компонентов моторов
    LeftFrontMotor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftFrontMotor"));
    RightFrontMotor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightFrontMotor"));
    LeftBackMotor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftBackMotor"));
    RightBackMotor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightBackMotor"));
    // Установка местоположения и ориентации компонентов
    LeftFrontMotor->SetRelativeLocation(FVector(80.0f, -80.0f, 0.0f));
    RightFrontMotor->SetRelativeLocation(FVector(80.0f, 80.0f, 0.0f));
    LeftBackMotor->SetRelativeLocation(FVector(-80.0f, -80.0f, 0.0f));
    RightBackMotor->SetRelativeLocation(FVector(-80.0f, 80.0f, 0.0f));
    // Добавляем симуляцию физики для моторов
    LeftFrontMotor->SetSimulatePhysics(true);
    RightFrontMotor->SetSimulatePhysics(true);
    LeftBackMotor->SetSimulatePhysics(true);
    RightBackMotor->SetSimulatePhysics(true);
    // Добавляем компоненты моторов в качестве дочерних к корневому компоненту
    LeftFrontMotor->SetupAttachment(RootComponent);
    RightFrontMotor->SetupAttachment(RootComponent);
    LeftBackMotor->SetupAttachment(RootComponent);
    RightBackMotor->SetupAttachment(RootComponent);
    // Создание и инициализация шарниров, скрепляющих моторы с телом дрона
    LeftFrontConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeftFrontConstraint"));
    RightFrontConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RightFrontConstraint"));
    LeftBackConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("LeftBackConstraint"));
    RightBackConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("RightBackConstraint"));
    LeftFrontConstraint->SetupAttachment(RootComponent);
    RightFrontConstraint->SetupAttachment(RootComponent);
    LeftBackConstraint->SetupAttachment(RootComponent);
    RightBackConstraint->SetupAttachment(RootComponent);
    InitializeMotorConstraint(LeftFrontConstraint, LeftFrontMotor);
    InitializeMotorConstraint(RightFrontConstraint, RightFrontMotor);
    InitializeMotorConstraint(LeftBackConstraint, LeftBackMotor);
    InitializeMotorConstraint(RightBackConstraint, RightBackMotor);
    // Сила тяги каждого ротора
    LeftFrontThrust = 1911.0f / 8.0f;
    RightFrontThrust = 1911.0f / 8.0f;
    LeftBackThrust = 1911.0f / 8.0f;
    RightBackThrust = 1911.0f / 8.0f;
    // Инициализация значений момента
    LeftFrontTorque = 3000.0f; // Положительное значение для винтов, вращающихся по часовой стрелке
    RightFrontTorque = -3000.0f; // Отрицательное значение для винтов, вращающихся против часовой стрелки
    LeftBackTorque = -3000.0f; // Отрицательное значение для винтов, вращающихся против часовой стрелки
    RightBackTorque = 3000.0f; // Положительное значение для винтов, вращающихся по часовой стрелке
    // Сохранение исходных значений моментов и тяги
    OriginalLeftFrontTorque = LeftFrontTorque;
    OriginalRightFrontTorque = RightFrontTorque;
    OriginalLeftBackTorque = LeftBackTorque;
    OriginalRightBackTorque = RightBackTorque;
    OriginalLeftFrontThrust = LeftFrontThrust;
    OriginalRightFrontThrust = RightFrontThrust;
    OriginalLeftBackThrust = LeftBackThrust;
    OriginalRightBackThrust = RightBackThrust;
    
    LeftFrontConstraint->SetDisableCollision(true);
    RightFrontConstraint->SetDisableCollision(true);
    LeftBackConstraint->SetDisableCollision(true);
    RightBackConstraint->SetDisableCollision(true);
    //?
    /*
    SpringArm0->bInheritPitch = false;
    SpringArm0->bInheritYaw = false;
    SpringArm0->bInheritRoll = false;
    */
        // here's issue ?? 
    SpringArm0 = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinMapSA"));
    SpringArm0->SetupAttachment(Mesh);
   
    SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CameraAbove"));
    SceneCaptureComponent->SetupAttachment(SpringArm0);
    SpringArm0->TargetArmLength = 500.f; 
}
// Called when the game starts or when spawned
void Adrone::BeginPlay()
{
    Super::BeginPlay();
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        // Получаем ссылку на HUD
        AHUD* HUD = PlayerController->GetHUD();
        if (HUD)
        {
            // Приводим HUD к вашему типу AdroneHUD
            AdroneHUD* MyDroneHUD = Cast<AdroneHUD>(HUD);
            if (MyDroneHUD)
            {
                // Вызываем метод BeginPlay у HUD или выполняем другие действия
                MyDroneHUD->BeginPlay();  // Это может быть не нужно, если HUD сам управляет своим BeginPlay
                // Либо добавляем виджет на экран напрямую, если это нужно:
                if (MyDroneHUD->MainHUD)
                {
                    UUserWidget* droneWidget = CreateWidget<UUserWidget>(GetWorld(), MyDroneHUD->MainHUD);
                    if (droneWidget)
                    {
                        droneWidget->AddToViewport();
                    }
                }
            }
        }
    }
}
// Called every frame
void Adrone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Применение сил к каждому из моторов
    FVector UpVector = Mesh->GetUpVector();
    LeftFrontMotor->AddForce(UpVector * LeftFrontThrust);
    RightFrontMotor->AddForce(UpVector * RightFrontThrust);
    LeftBackMotor->AddForce(UpVector * LeftBackThrust);
    RightBackMotor->AddForce(UpVector * RightBackThrust);
    // Применение момента закручивания к каждому из моторов
    LeftFrontMotor->AddTorqueInDegrees(GetActorUpVector() * LeftFrontTorque);
    RightFrontMotor->AddTorqueInDegrees(GetActorUpVector() * RightFrontTorque);
    LeftBackMotor->AddTorqueInDegrees(GetActorUpVector() * LeftBackTorque);
    RightBackMotor->AddTorqueInDegrees(GetActorUpVector() * RightBackTorque);
    FVector DroneVelocity = Mesh->GetComponentVelocity();
    float Speed = DroneVelocity.Size();
    FVector ActorLocation = GetActorLocation();
    float A = ActorLocation.Z;
    A58 = A - 58, 8;
    if (GEngine)
    {
        FString AStr = FString::Printf(TEXT("Altitude: %.2f"), A58);
        static const int32 Key1 = 1;
        GEngine->AddOnScreenDebugMessage(Key1, 100, FColor::Green, AStr);
        static const int32 Key2 = 2;
        GEngine->AddOnScreenDebugMessage(Key2, 0.0f, FColor::Green, FString::Printf(TEXT("Speed: %.2f m/s"), Speed));
    }
}
void Adrone::InitializeMotorConstraint(UPhysicsConstraintComponent* Constraint, UStaticMeshComponent* Motor)
{
    //Ограничение на движение шарниров
    Constraint->SetConstrainedComponents(Mesh, NAME_None, Motor, NAME_None);
    Constraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
    Constraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
    Constraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
    Constraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
    Constraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
    Constraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.0f);
}
// Called to bind functionality to input
void Adrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAction("MoveUp", IE_Pressed, this, &Adrone::MoveUp);
    PlayerInputComponent->BindAction("MoveUp", IE_Released, this, &Adrone::AdjustForwardThrustInputReleased);
    PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &Adrone::MoveDown);
    PlayerInputComponent->BindAction("MoveDown", IE_Released, this, &Adrone::AdjustForwardThrustInputReleased);
    PlayerInputComponent->BindAction("AdjustRightYawTorque", IE_Pressed, this, &Adrone::AdjustRightYawTorque);
    PlayerInputComponent->BindAction("AdjustRightYawTorque", IE_Released, this, &Adrone::AdjustYawTorqueInputReleased);
    PlayerInputComponent->BindAction("AdjustLeftYawTorque", IE_Pressed, this, &Adrone::AdjustLeftYawTorque);
    PlayerInputComponent->BindAction("AdjustLeftYawTorque", IE_Released, this, &Adrone::AdjustYawTorqueInputReleased);
    PlayerInputComponent->BindAction("AdjustForwardThrust", IE_Pressed, this, &Adrone::AdjustForwardThrust);
    PlayerInputComponent->BindAction("AdjustForwardThrust", IE_Released, this, &Adrone::AdjustForwardThrustInputReleased);
    PlayerInputComponent->BindAction("AdjustBackThrust", IE_Pressed, this, &Adrone::AdjustBackThrust);
    PlayerInputComponent->BindAction("AdjustBackThrust", IE_Released, this, &Adrone::AdjustForwardThrustInputReleased);
    PlayerInputComponent->BindAction("AdjustRightThrust", IE_Pressed, this, &Adrone::AdjustRightThrust);
    PlayerInputComponent->BindAction("AdjustRightThrust", IE_Released, this, &Adrone::AdjustForwardThrustInputReleased);
    PlayerInputComponent->BindAction("AdjustLeftThrust", IE_Pressed, this, &Adrone::AdjustLeftThrust);
    PlayerInputComponent->BindAction("AdjustLeftThrust", IE_Released, this, &Adrone::AdjustForwardThrustInputReleased);
}
void Adrone::MoveUp()
{
    float AdjustedThrust = OriginalLeftFrontThrust + 3000.0f; // Коэффициент для регулировки тяги
    // Увеличение всех винтов для движения вверх  
    LeftFrontThrust = AdjustedThrust;
    LeftBackThrust = AdjustedThrust;
    RightFrontThrust = AdjustedThrust;
    RightBackThrust = AdjustedThrust;
}
void Adrone::MoveDown()
{
    float AdjustedThrust = OriginalLeftFrontThrust - 3000.0f; // Коэффициент для регулировки тяги
    // Увеличение всех винтов для движения вниз 
    LeftFrontThrust = AdjustedThrust;
    LeftBackThrust = AdjustedThrust;
    RightFrontThrust = AdjustedThrust;
    RightBackThrust = AdjustedThrust;
}
void Adrone::AdjustYawTorqueInputReleased() {
    LeftFrontTorque = OriginalLeftFrontTorque;
    RightFrontTorque = OriginalRightFrontTorque;
    LeftBackTorque = OriginalLeftBackTorque;
    RightBackTorque = OriginalRightBackTorque;
}
void Adrone::AdjustRightYawTorque()
{
    float AdjustedTorque = 2000.0f; // Коэффициент для регулировки момента
    // Уменьшение момента винтов, вращающихся по часовой стрелке, для вращения по часовой стрелке
    RightFrontTorque = AdjustedTorque;
    LeftBackTorque = AdjustedTorque;
}
void Adrone::AdjustLeftYawTorque() {
    float AdjustedTorque = -2000.0f; // Коэффициент для регулировки момента
    // Уменьшение момента винтов, вращающихся против часовой стрелки, для вращения против часовой стрелке
    LeftFrontTorque = AdjustedTorque;
    RightBackTorque = AdjustedTorque;
}
void Adrone::AdjustForwardThrustInputReleased()
{
    LeftFrontThrust = OriginalLeftFrontThrust;
    RightFrontThrust = OriginalRightFrontThrust;
    LeftBackThrust = OriginalLeftBackThrust;
    RightBackThrust = OriginalRightBackThrust;
}
// Регулировка тяги для движения вперед/назад (тангаж)
void Adrone::AdjustForwardThrust()
{
    float AdjustedThrust = OriginalLeftFrontThrust + 400.0f; // Коэффициент для регулировки тяги
    // Увеличение тяги задних винтов для движения вперед
    LeftBackThrust = AdjustedThrust;
    RightBackThrust = AdjustedThrust;
}
void Adrone::AdjustBackThrust() {
    float AdjustedFrontThrust = OriginalLeftFrontThrust + 400.0f; // Коэффициент для регулировки тяги
    // Увеличение тяги передних винтов для движения назад
    LeftFrontThrust = AdjustedFrontThrust;
    RightFrontThrust = AdjustedFrontThrust;
}
// Регулировка тяги для движения вправо/влево (крен)
void Adrone::AdjustRightThrust()
{
    float AdjustedThrust = OriginalLeftFrontThrust + 400.0f; // Коэффициент для регулировки тяги
    // Увеличение тяги левых винтов для движения вправо   
    LeftFrontThrust = AdjustedThrust;
    LeftBackThrust = AdjustedThrust;
}
void Adrone::AdjustLeftThrust() {
    float AdjustedThrust = OriginalLeftFrontThrust + 400.0f; // Коэффициент для регулировки тяги
    // Увеличение тяги правых винтов для движения влево
    RightFrontThrust = AdjustedThrust;
    RightBackThrust = AdjustedThrust;
}
