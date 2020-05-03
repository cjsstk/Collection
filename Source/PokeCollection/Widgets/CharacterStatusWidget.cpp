// Copyright Me. All Rights Reserved.


#include "CharacterStatusWidget.h"

void UCharacterStatusWidget::OnOpen()
{
	Super::OnOpen();

	/*APokeCollectionCharacter* Player = GetPlayer();
	if (ensure(Player))
	{
		APokeCharacter* SelectedCharacter = Player->GetCharacterByID(SelectedCharacterID);
		if (SelectedCharacter)
		{
			if (CharacterImage)
			{
				CharacterImage->SetBrushFromTexture(SelectedCharacter->GetCharacterImage());
			}
			if (CharacterName)
			{
				CharacterName->SetText(FText::FromName(SelectedCharacter->GetCharacterName()));
			}
		}
	}*/
}

void UCharacterStatusWidget::SetTypeImages(CharacterType InCharacterType)
{
	/*if (!Type1MaterialInstance || !Type2MaterialInstance || !Type1Image || !Type2Image)
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
	}*/
}
