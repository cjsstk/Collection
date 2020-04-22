// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MakePartyCharacterPanel.generated.h"

UCLASS()
class POKECOLLECTION_API AMakePartyCharacterPanel : public AActor
{
	GENERATED_BODY()
	
public:	
	AMakePartyCharacterPanel();

	int32 GetSlotNum() const { return SlotNum; }
	void SetFlipbook(class UPaperFlipbook* NewFlipbook);
	void SetIsEmptySlot(bool bInIsEmptySlot) { bIsEmptySlot = bInIsEmptySlot; }

	virtual void NotifyActorOnReleased(FKey PressedButton = EKeys::LeftMouseButton) override;
	virtual void NotifyActorOnInputTouchBegin(const ETouchIndex::Type FingerIndex = ETouchIndex::Type::Touch1) override;
	virtual void NotifyActorOnInputTouchEnd(const ETouchIndex::Type FingerIndex = ETouchIndex::Type::Touch1) override;

protected:
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 SlotNum = 0;

	bool bIsEmptySlot = false;

};
