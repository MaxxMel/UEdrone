#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
//#include "Blueprint/UserWidget.h"
#include "drone.generated.h"
UCLASS()
class P3_API Adrone : public APawn
{
    GENERATED_BODY()
public:
    Adrone();
protected:
    virtual void BeginPlay() override;
public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* Mesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USpringArmComponent* SpringArm;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UCameraComponent* Camera;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementForce;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Drone Components")
    UStaticMeshComponent* LeftFrontMotor;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Drone Components")
    UStaticMeshComponent* RightFrontMotor;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Drone Components")
    UStaticMeshComponent* LeftBackMotor;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Drone Components")
    UStaticMeshComponent* RightBackMotor;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone Components", meta = (AllowPrivateAccess = "true"))
    UPhysicsConstraintComponent* LeftFrontConstraint;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone Components", meta = (AllowPrivateAccess = "true"))
    UPhysicsConstraintComponent* RightFrontConstraint;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone Components", meta = (AllowPrivateAccess = "true"))
    UPhysicsConstraintComponent* LeftBackConstraint;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone Components", meta = (AllowPrivateAccess = "true"))
    UPhysicsConstraintComponent* RightBackConstraint;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thrust")
    float LeftFrontThrust;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thrust")
    float RightFrontThrust;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thrust")
    float LeftBackThrust;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thrust")
    float RightBackThrust;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torque")
    float LeftFrontTorque;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torque")
    float RightFrontTorque;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torque")
    float LeftBackTorque;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torque")
    float RightBackTorque;
    float OriginalLeftFrontTorque;
    float OriginalRightFrontTorque;
    float OriginalLeftBackTorque;
    float OriginalRightBackTorque;
    float OriginalLeftFrontThrust;
    float OriginalRightFrontThrust;
    float OriginalLeftBackThrust;
    float OriginalRightBackThrust;
    void InitializeMotorConstraint(UPhysicsConstraintComponent* Constraint, UStaticMeshComponent* Motor);
    void MoveUp();
    void MoveDown();
    void AdjustLeftYawTorque();
    void AdjustRightYawTorque();
    void AdjustForwardThrustInputReleased();
    void AdjustYawTorqueInputReleased();
    void AdjustForwardThrust();
    void AdjustRightThrust();
    void AdjustBackThrust();
    void AdjustLeftThrust();
    float A58;
    //TSubclassOf<UUserWidget> WB;
    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArm0; 
    UPROPERTY(VisibleAnywhere)
    USceneCaptureComponent2D* SceneCaptureComponent;
};
