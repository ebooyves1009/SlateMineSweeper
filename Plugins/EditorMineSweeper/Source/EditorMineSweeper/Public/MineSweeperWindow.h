// Copyright © by Tyni Boat. All Rights Reserved.

#pragma once
#include "MineSweeperGrid.h"
#include "SMineSweeperGridParams.h"

/**
 * The Mine Sweeper Game window
 */
class EDITORMINESWEEPER_API SMineSweeperWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMineSweeperWindow)
		{
		}
	SLATE_END_ARGS()

	
	void Construct(const FArguments& InArgs);

	FReply GenerateNewGrid();
	
protected:

	TSharedPtr<SMineSweeperGrid> _MineGrid;
	TSharedPtr<SMineSweeperGridParams> _MineGridParams;
};
