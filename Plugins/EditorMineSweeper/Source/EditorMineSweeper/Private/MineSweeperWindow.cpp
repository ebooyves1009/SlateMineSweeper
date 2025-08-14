// Copyright © by Tyni Boat. All Rights Reserved.

#include "MineSweeperWindow.h"
#include "MineSweeperGrid.h"
#include "SlateFwd.h"


void SMineSweeperWindow::Construct(const FArguments& InArgs)
{
	ChildSlot[
		SNew(SVerticalBox)
		// Parameters
		+ SVerticalBox::Slot().Padding(15).AutoHeight()
		[
			SNew(SBorder)
			[
				SAssignNew(_MineGridParams, SMineSweeperGridParams)
				.OnClickedGenerateGrid(this, &SMineSweeperWindow::GenerateNewGrid)
			]
		]
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SSpacer)
		]
		// Grid Part
		+ SVerticalBox::Slot().Padding(15).FillHeight(1)
		[
			SNew(SBorder)
			[
				SAssignNew(_MineGrid, SMineSweeperGrid)
				.CellSize(64)
			]
		]
	];
}

FReply SMineSweeperWindow::GenerateNewGrid()
{
	if (_MineGrid != nullptr && _MineGridParams != nullptr)
	{
		_MineGrid->GenerateNewGrid(_MineGridParams->GetMineCount(), _MineGridParams->GetGridSize());
	}
	return FReply::Handled();
}