// Copyright © by Tyni Boat. All Rights Reserved.


#include "SMineSweeperGridParams.h"


void SMineSweeperGridParams::Construct(const FArguments& InArgs)
{
	float VHeight = 40;
	float SectionPadding = 15;
	float InnerPadding = 5;
	OnClickedGenerateGridEvent = InArgs._OnClickedGenerateGrid;

	ChildSlot
	[
		SNew(SVerticalBox)
		// Width and height
		+ SVerticalBox::Slot().MaxHeight(VHeight).Padding(SectionPadding, InnerPadding).AutoHeight()
		[
			SNew(SHorizontalBox)
			// Width Param
			+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(InnerPadding)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Width:")))
				.Justification(ETextJustify::Center)
			]
			+ SHorizontalBox::Slot().FillWidth(1).VAlign(VAlign_Center).Padding(InnerPadding)
			[
				SAssignNew(_Entry_Width, SNumericEntryBox<int32>)
				.Value_Lambda([&]()-> int32 { return _Width; })
				.MinValue(2)
				.OnValueChanged_Lambda([this](const int32& Value)-> void { _Width = Value; })
			]
			//
			+ SHorizontalBox::Slot().HAlign(HAlign_Left)
			[
				SNew(SSpacer)
			]
			//  Height Param
			+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(InnerPadding)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Height:")))
				.Justification(ETextJustify::Center)
			]
			+ SHorizontalBox::Slot().FillWidth(1).VAlign(VAlign_Center).Padding(InnerPadding)
			[
				SAssignNew(_Entry_Height, SNumericEntryBox<int32>)
				.Value_Lambda([&]()-> int32 { return _Height; })
				.MinValue(2)
				.OnValueChanged_Lambda([this](const int32& Value)-> void { _Height = Value; })
			]
		]
		// Bomb Count
		+ SVerticalBox::Slot().MaxHeight(VHeight).Padding(SectionPadding, InnerPadding).AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(InnerPadding)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Number of Mines:")))
				.Justification(ETextJustify::Center)
			]
			+ SHorizontalBox::Slot().FillWidth(1).VAlign(VAlign_Center).Padding(InnerPadding)
			[
				SAssignNew(_Entry_MineCount, SNumericEntryBox<int32>)
				.Value_Lambda([&]()-> int32 { return _MineCount; })
				.MinValue(1)
				.OnValueChanged_Lambda([this](const int32& Value)-> void { _MineCount = Value; })
			]
		]
		// Generate Button
		+ SVerticalBox::Slot().MaxHeight(VHeight).Padding(SectionPadding).AutoHeight()
		[
			SNew(SButton)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.OnClicked(this, &SMineSweeperGridParams::OnClickedOnButtonGenerate)
			.Text(FText::FromString(TEXT("Generate New Grid")))
		]
	];
}

FReply SMineSweeperGridParams::OnClickedOnButtonGenerate()
{
	if (OnClickedGenerateGridEvent.IsBound())
		return OnClickedGenerateGridEvent.Execute();
	return FReply::Unhandled();
}

int32 SMineSweeperGridParams::GetMineCount() const
{
	return _MineCount;
}

FVector2D SMineSweeperGridParams::GetGridSize() const
{
	return FVector2D(_Width, _Height);
}
