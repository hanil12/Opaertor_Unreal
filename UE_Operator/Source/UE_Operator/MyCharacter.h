// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class UE_OPERATOR_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void UpDown(float value);
	void LeftRight(float value);
	void Yaw(float value);
	void Pitch(float value);

	UFUNCTION(BlueprintCallable)
	float UpDownValue() { return _upDown; }

	UFUNCTION(BlueprintCallable)
	float LeftRightValue() { return _leftRight; }

	UFUNCTION(BlueprintCallable)
	bool IsTurnLeft() { return _isTurnLeft; }

	UFUNCTION(BlueprintCallable)
	bool IsTurnRight() { return _isTurnRight; }

	UFUNCTION(BlueprintCallable)
	bool IsAttacking() { return _isAttacking; }

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* montage, bool bInterrupted);

	void Attack();
	void AttackCheck();

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMesh* _mesh;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* _springArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* _camera;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	float _upDown;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	float _leftRight;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	bool _isTurnRight;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	bool _isTurnLeft;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	bool _isAttacking = false;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	int32 _attackIndex = 0;

	UPROPERTY()
	class UMyAnimInstance* _animInstance;
	UPROPERTY()
	class UStatComponent* _statComponent;
	UPROPERTY()
	class UWidgetComponent* _hpBarWidget;
};
