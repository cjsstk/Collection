// Copyright Me. All Rights Reserved.


#include "CharacterDetailInfoWidget.h"

#include "Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TextBlock.h"

#include "CMS.h"
#include "PokeCharacter.h"

void UCharacterDetailInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UCharacterDetailInfoWidget::OnOpen()
{
	Super::OnOpen();

	if (TypeMaterial)
	{
		if (!Type1MaterialInstance)
		{
			Type1MaterialInstance = UMaterialInstanceDynamic::Create(TypeMaterial, this);
		}
		if (!Type2MaterialInstance)
		{
			Type2MaterialInstance = UMaterialInstanceDynamic::Create(TypeMaterial, this);
		}
	}

	APokeCharacter* SelectedCharacter = GetSelectedCharacter();
	if (ensure(SelectedCharacter))
	{
		if (CharacterImage)
		{
			CharacterImage->SetBrushFromTexture(SelectedCharacter->GetCharacterImage());
		}

		if (CharacterName)
		{
			CharacterName->SetText(FText::FromName(SelectedCharacter->GetCharacterName()));
		}

		if (CharacterLevel)
		{
			CharacterLevel->SetText(FText::FromString(FString::FromInt(SelectedCharacter->GetLevel())));
		}

		if (CurrentExp)
		{
			CurrentExp->SetText(FText::FromString(FString::FromInt(SelectedCharacter->GetCurrentExp())));
		}

		if (MaxExp)
		{
			MaxExp->SetText(FText::FromString(FString::FromInt(SelectedCharacter->GetMaxExp())));
		}

		FStatus FinalStatus = SelectedCharacter->GetFinalStatus();

		if (HealthPoint)
		{
			HealthPoint->SetText(FText::FromString(FString::FromInt(FinalStatus.HealthPoint)));
		}

		if (MaxHealthPoint)
		{
			MaxHealthPoint->SetText(FText::FromString(FString::FromInt(FinalStatus.HealthPoint)));
		}

		if (Attack)
		{
			Attack->SetText(FText::FromString(FString::FromInt(FinalStatus.Attack)));
		}

		if (Defence)
		{
			Defence->SetText(FText::FromString(FString::FromInt(FinalStatus.Defense)));
		}

		if (SpecialAttack)
		{
			SpecialAttack->SetText(FText::FromString(FString::FromInt(FinalStatus.SpecialAttack)));
		}

		if (SpecialDefence)
		{
			SpecialDefence->SetText(FText::FromString(FString::FromInt(FinalStatus.SpecialDefense)));
		}

		if (Speed)
		{
			Speed->SetText(FText::FromString(FString::FromInt(FinalStatus.Speed)));
		}

		if (AttackRange)
		{
			AttackRange->SetText(FText::FromString(FString::FromInt(SelectedCharacter->GetAttackRange())));
		}

		SetTypeImages(SelectedCharacter->GetCharacterType());
		SetBackgroundColor(SelectedCharacter->GetCharacterRank());
	}
}

void UCharacterDetailInfoWidget::SetTypeImages(CharacterType InCharacterType)
{
	if (!Type1MaterialInstance || !Type2MaterialInstance || !Type1Image || !Type2Image)
	{
		ensure(0);
		return;
	}

	const FTypeInfo* Type1Info = CMS::GetTypeDataTable(InCharacterType.Type1);
	const FTypeInfo* Type2Info = CMS::GetTypeDataTable(InCharacterType.Type2);

	int32 Type1TextureNum = Type1Info ? Type1Info->TextureNum : -1;
	int32 Type2TextureNum = Type2Info ? Type2Info->TextureNum : -1;

	if (Type1TextureNum < 0)
	{
		Type1Image->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Type1Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		int32 Type1ColumnIndex = Type1TextureNum % CharacterTypeColumnNum;
		int32 Type1RawIndex = Type1TextureNum / CharacterTypeColumnNum;

		Type1MaterialInstance->SetScalarParameterValue("ColumnIndexParam", Type1ColumnIndex);
		Type1MaterialInstance->SetScalarParameterValue("RowIndexParam", Type1RawIndex);

		Type1Image->SetBrushFromMaterial(Type1MaterialInstance);
	}

	if (Type2TextureNum < 0)
	{
		Type2Image->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Type2Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		int32 Type2ColumnIndex = Type2TextureNum % CharacterTypeColumnNum;
		int32 Type2RawIndex = Type2TextureNum / CharacterTypeColumnNum;

		Type2MaterialInstance->SetScalarParameterValue("ColumnIndexParam", Type2ColumnIndex);
		Type2MaterialInstance->SetScalarParameterValue("RowIndexParam", Type2RawIndex);

		Type2Image->SetBrushFromMaterial(Type2MaterialInstance);
	}
}

void UCharacterDetailInfoWidget::SetBackgroundColor(ERank InRank)
{
	if (RankBackground)
	{
		FLinearColor Color;

		switch (InRank)
		{
		case ERank::Normal:
			Color = NormalColor;
			break;
		case ERank::Rare:
			Color = RareColor;
			break;
		case ERank::SRare:
			Color = SRareColor;
			break;
		case ERank::SSRare:
			Color = SSRareColor;
			break;
		default:
			break;
		}

		RankBackground->SetColorAndOpacity(Color);
	}
}
