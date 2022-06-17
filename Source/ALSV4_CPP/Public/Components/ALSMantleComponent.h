﻿// Project:         Advanced Locomotion System V4 on C++
// Copyright:       Copyright (C) 2021 Doğa Can Yanıkoğlu
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/dyanikoglu/ALSV4_CPP
// Original Author: Doğa Can Yanıkoğlu

#pragma once

#include "CoreMinimal.h"

#include "Character/ALSBaseCharacter.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "ALSMantleComponent.generated.h"

// forward declarations
class UALSDebugComponent;


UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API UALSMantleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UALSMantleComponent();

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Mantle System")
	bool MantleCheck(const FALSMantleTraceSettings& TraceSettings,
	                 EDrawDebugTrace::Type DebugType);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Mantle System")
	void MantleStart(float MantleHeight, const FALSComponentAndTransform& MantleLedgeWS,
	                EALSMantleType MantleType);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Mantle System")
	void MantleUpdate(float BlendIn);

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Mantle System")
	void MantleEnd();

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Mantle System")
	void OnOwnerJumpInput();

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Mantle System")
	void OnOwnerRagdollStateChanged(bool bRagdollState);

	/** Implement on BP to get correct mantle parameter set according to character state */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALS|Mantle System")
	FALSMantleAsset GetMantleAsset(EALSMantleType MantleType, EALSOverlayState CurrentOverlayState);

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// Called when the game starts
	virtual void BeginPlay() override;

	/** Mantling*/
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "ALS|Mantle System")
	void Server_MantleStart(float MantleHeight, const FALSComponentAndTransform& MantleLedgeWS,
	                        EALSMantleType MantleType);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "ALS|Mantle System")
	void Multicast_MantleStart(float MantleHeight, const FALSComponentAndTransform& MantleLedgeWS,
	                           EALSMantleType MantleType);


	// Mantle checking conditions
	UFUNCTION(BlueprintNativeEvent, Category="ALS|Mantle System")
	bool IsComponentValidForMantle(UPrimitiveComponent* PrimitiveComponent) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Mantle System")
	UTimelineComponent* MantleTimeline = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Mantle System")
	FALSMantleTraceSettings GroundedTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Mantle System")
	FALSMantleTraceSettings AutomaticTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Mantle System")
	FALSMantleTraceSettings FallingTraceSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Mantle System")
	UCurveFloat* MantleTimelineCurve;

	static FName NAME_IgnoreOnlyPawn;
	/** Profile to use to detect objects we allow mantling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Mantle System")
	FName MantleObjectDetectionProfile = NAME_IgnoreOnlyPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Mantle System")
	TEnumAsByte<ECollisionChannel> WalkableSurfaceDetectionChannel = ECC_Visibility;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Mantle System")
	FALSMantleParams MantleParams;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Mantle System")
	FALSComponentAndTransform MantleLedgeLS;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Mantle System")
	FTransform MantleTarget = FTransform::Identity;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Mantle System")
	FTransform MantleActualStartOffset = FTransform::Identity;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Mantle System")
	FTransform MantleAnimatedStartOffset = FTransform::Identity;

	/** If a dynamic object has a velocity bigger than this value, do not start mantle */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Mantle System")
	float AcceptableVelocityWhileMantling = 10.0f;

private:
	UPROPERTY()
	AALSBaseCharacter* OwnerCharacter;

	UPROPERTY()
	UALSDebugComponent* ALSDebugComponent = nullptr;
};
