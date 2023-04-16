// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FAttackEnd)

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
	
	float UpDownValue() const { return _upDown; }
	float LeftRightValue() const { return _leftRight; }
	bool IsTurnLeft() const { return _isTurnLeft; }
	bool IsTurnRight() const { return _isTurnRight; }
	bool IsAttacking() const { return _isAttacking; }
	bool IsDead() const { return _statComponent->IsDead();}

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* montage, bool bInterrupted);

	void Attack();
	virtual void AttackCheck() {}

	FAttackEnd _attackEnd;

protected:
	virtual void SetMesh() {}
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMesh* _mesh;
	
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
