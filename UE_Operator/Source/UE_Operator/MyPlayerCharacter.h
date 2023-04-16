// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "MyPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UE_OPERATOR_API AMyPlayerCharacter : public AMyCharacter
{
	GENERATED_BODY()
public:
	AMyPlayerCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	
	void UpDown(float value);
	void LeftRight(float value);
	void Yaw(float value);
	void Pitch(float value);

virtual void AttackCheck() override;
	
private:
	virtual void SetMesh() override;
	void SetUpSpringArmAndCamera();
	
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* _springArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* _camera;
};
