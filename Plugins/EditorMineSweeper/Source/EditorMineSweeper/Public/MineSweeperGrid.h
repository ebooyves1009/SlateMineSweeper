// Copyright © by Tyni Boat. All Rights Reserved.

#pragma once
#include "Widgets/Views/STileView.h"


struct FMineSweeperCellData
{
public:

	FMineSweeperCellData();
	FMineSweeperCellData(int CellIndex, bool bAsMine = false);
	
	int32 Index = -1;
	bool bIsAMine = false;
	bool bIsBeenDiscovered = false;
	FText MineText = FText::FromString(" ");
};


/**
 * The Slate Widget representing the mine sweeper's grid
 */
class EDITORMINESWEEPER_API  SMineSweeperGrid : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMineSweeperGrid)
		: _CellSize(64)
		, _ColumnCount(8)
	{
	}
		SLATE_ATTRIBUTE( float, CellSize )
		SLATE_ATTRIBUTE( int32, ColumnCount )

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
	// Construct a tile widget
	TSharedRef<ITableRow> OnGenerateWidgetForTileView(TSharedPtr<FMineSweeperCellData> Item, const TSharedRef<STableViewBase>& OwnerTable);
	
	// On cell Selection.
	void OnSelectionChanged(TSharedPtr<FMineSweeperCellData> MineSweeperCellData, ESelectInfo::Type SelectionInfos);
	
	// generate a tile with a set number of Mines
	void GenerateNewGrid(int MineNumber, const FVector2D GridSize);
	
	// On Game Over
	void OnGameOver(bool bWonGame = false);

	// get neighbor cells indexes at are nor yet been discovered
	bool GetNeighborHiddenCellsIndexes(int Index, int GridWidth, TArray<int>& OutIndexes);

	// Check if a cell index is undiscovered
	bool IsUndiscoveredCell(int Index);


protected:
	
	// the list of mine sweeper cells
	TArray<TSharedPtr<FMineSweeperCellData>> _CellDatas;

	// The reference to the Tile widget
	TSharedPtr<STileView<TSharedPtr<FMineSweeperCellData>>> _TileViewWidget;

	// The Horizontal box that control width
	TSharedPtr<SHorizontalBox> _WidthControlBox;

	// the reference to the game Over text block
	TSharedPtr<STextBlock> _GameOverText;

	// the Desired number of Columns
	int32 _DesiredColumnCount = 8;

	// The Default Item Size
	float _ItemSize = 64;
};
