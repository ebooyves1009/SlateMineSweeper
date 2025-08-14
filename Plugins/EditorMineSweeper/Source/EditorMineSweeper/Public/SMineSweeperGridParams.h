// Copyright © by Tyni Boat. All Rights Reserved.

#pragma once
#include "Widgets/Input/SNumericEntryBox.h"

/**
 * Parameter Of A Mine Sweeper grid
 */
class EDITORMINESWEEPER_API SMineSweeperGridParams : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMineSweeperGridParams)
		{
		}

		/** Called when the generate button is clicked */
		SLATE_EVENT(FOnClicked, OnClickedGenerateGrid)

	SLATE_END_ARGS()

	
	void Construct(const FArguments& InArgs);

	FReply OnClickedOnButtonGenerate();
	
	int32 GetMineCount() const;
	
	FVector2D GetGridSize() const;

protected:
	TSharedPtr<SNumericEntryBox<int32>> _Entry_MineCount;
	TSharedPtr<SNumericEntryBox<int32>> _Entry_Width;
	TSharedPtr<SNumericEntryBox<int32>> _Entry_Height;

	FOnClicked OnClickedGenerateGridEvent;

	int32 _MineCount = 12;
	int32 _Width = 8;
	int32 _Height = 8;
};
