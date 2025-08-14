// Copyright © by Tyni Boat. All Rights Reserved.

#include "MineSweeperGrid.h"


FMineSweeperCellData::FMineSweeperCellData()
{
}

FMineSweeperCellData::FMineSweeperCellData(int CellIndex, bool bAsMine): Index(CellIndex), bIsAMine(bAsMine)
{
}

void SMineSweeperGrid::Construct(const FArguments& InArgs)
{
	_ItemSize = InArgs._CellSize.Get();
	_DesiredColumnCount = InArgs._ColumnCount.Get();
	int width = _ItemSize * _DesiredColumnCount;
	FSlateFontInfo gameOverFont = FCoreStyle::Get().GetFontStyle("NormalFont");
	gameOverFont.Size = 40;
	ChildSlot[
		SNew(SOverlay)
		+ SOverlay::Slot().HAlign(HAlign_Center)
		[
			SAssignNew(_WidthControlBox, SHorizontalBox)
			+ SHorizontalBox::Slot().FillWidth(1).MaxWidth(width).MinWidth(width)
			[
				SAssignNew(_TileViewWidget, STileView<TSharedPtr<FMineSweeperCellData>>)
				.ItemWidth_Lambda([&]()-> float { return _ItemSize; })
				.ItemHeight_Lambda([&]()-> float { return _ItemSize; })
				.ListItemsSource(&_CellDatas)
				.OnGenerateTile(this, &SMineSweeperGrid::OnGenerateWidgetForTileView)
				.SelectionMode(ESelectionMode::Single)
				.OnSelectionChanged(this, &SMineSweeperGrid::OnSelectionChanged)
			]
		]
		+ SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SAssignNew(_GameOverText, STextBlock)
			.Text(FText::FromString("Game Over"))
			.Font(gameOverFont)
			.Visibility(EVisibility::Hidden)
		]
	];

	GenerateNewGrid(12, FVector2D(8));
}

TSharedRef<ITableRow> SMineSweeperGrid::OnGenerateWidgetForTileView(TSharedPtr<FMineSweeperCellData> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return
			SNew(STableRow< TSharedPtr<FMineSweeperCellData>>, OwnerTable)
			.Padding(0)
			[
				SNew(SBorder).HAlign(HAlign_Center).VAlign(VAlign_Center).BorderBackgroundColor(FSlateColor(FLinearColor(0, 0, 0, 0.6)))
				[
					SNew(STextBlock)
					.Justification(ETextJustify::Center)
					.Text_Lambda([Item]()-> FText { return Item.Get()->MineText; })
				]
			];
}

void SMineSweeperGrid::OnSelectionChanged(TSharedPtr<FMineSweeperCellData> MineSweeperCellData, ESelectInfo::Type SelectionInfos)
{
	_TileViewWidget->ClearSelection();
	if (!_TileViewWidget)
		return;
	if (MineSweeperCellData == nullptr)
		return;
	if (MineSweeperCellData->bIsBeenDiscovered)
		return;
	// Discover cell
	MineSweeperCellData->bIsBeenDiscovered = true;
	MineSweeperCellData->MineText = MineSweeperCellData->bIsAMine ? FText::FromString("X") : FText::FromString(" ");
	if (MineSweeperCellData->bIsAMine)
	{
		// If It's A mine Display Game Over
		OnGameOver();
	}
	else
	{
		// Discover all it's neighbors as well
		TArray<int> neighborIndexes;
		if (GetNeighborHiddenCellsIndexes(_CellDatas.IndexOfByKey(MineSweeperCellData), _DesiredColumnCount, neighborIndexes))
		{
			for (int i = 0; i < neighborIndexes.Num(); i++)
			{
				_CellDatas[neighborIndexes[i]]->bIsBeenDiscovered = true;
				_CellDatas[neighborIndexes[i]]->MineText = _CellDatas[neighborIndexes[i]]->bIsAMine ? FText::FromString("X") : FText::FromString(" ");
			}
		}

		// Check if there are still undiscovered cells
		int undiscoveredIndex = _CellDatas.IndexOfByPredicate([](const TSharedPtr<FMineSweeperCellData>& cell)-> bool { return cell && !cell.Get()->bIsBeenDiscovered; });
		int mineIndex = _CellDatas.IndexOfByPredicate([](const TSharedPtr<FMineSweeperCellData>& cell)-> bool
		{
			return cell && cell.Get()->bIsAMine && !cell.Get()->bIsBeenDiscovered;
		});
		// If there are no more mine to discover or if there are no more cell to discover at all
		if (undiscoveredIndex == INDEX_NONE || mineIndex == INDEX_NONE)
			OnGameOver(true);
	}

	for (auto& Cell : _CellDatas)
		_TileViewWidget->SetItemHighlighted(Cell, Cell->bIsBeenDiscovered);
}

void SMineSweeperGrid::GenerateNewGrid(int MineNumber, const FVector2D GridSize)
{
	if (!_TileViewWidget)
		return;
	// Clears and initializations
	_DesiredColumnCount = FMath::CeilToInt(GridSize.X);
	if (_WidthControlBox)
	{
		_WidthControlBox->GetSlot(0).SetMaxWidth(_DesiredColumnCount * _ItemSize);
		_WidthControlBox->GetSlot(0).SetMinWidth(_DesiredColumnCount * _ItemSize);
	}
	_CellDatas.Empty();
	const int32 cellCount = FMath::CeilToInt(GridSize.Y) * _DesiredColumnCount;
	// handle visibility
	_TileViewWidget->SetSelectionMode(ESelectionMode::Single);
	if (_GameOverText)
		_GameOverText->SetVisibility(EVisibility::Hidden);
	// Mine Placement Logic
	TArray<int> availableIndexes;
	TArray<int> mineIndexes;
	for (int i = 0; i < cellCount; i++)
		availableIndexes.Add(i);
	for (int i = 0; i < MineNumber; i++)
	{
		int randMineIndex = FMath::RandRange(0, availableIndexes.Num() - 1);
		if (!availableIndexes.IsValidIndex(randMineIndex))
			break;
		mineIndexes.Add(availableIndexes[randMineIndex]);
		availableIndexes.RemoveAt(randMineIndex);
	}
	// Generate cells
	for (int i = 0; i < cellCount; i++)
		_CellDatas.Add(MakeShareable(new FMineSweeperCellData(i, mineIndexes.Contains(i))));
	_TileViewWidget->RequestListRefresh();
}

void SMineSweeperGrid::OnGameOver(bool bWonGame)
{
	if (_TileViewWidget)
		_TileViewWidget->SetSelectionMode(ESelectionMode::None);
	if (_GameOverText)
	{
		_GameOverText->SetText(bWonGame ? FText::FromString("Congratulation!") : FText::FromString("Game Over"));
		_GameOverText->SetVisibility(EVisibility::Visible);
	}
}

bool SMineSweeperGrid::GetNeighborHiddenCellsIndexes(int Index, int GridWidth, TArray<int>& OutIndexes)
{
	if (!_CellDatas.IsValidIndex(Index))
		return false;
	//Left
	if ((Index % GridWidth) > 0 && IsUndiscoveredCell(Index - 1))
		OutIndexes.AddUnique(Index - 1);
	//Right
	if ((Index % GridWidth) < (GridWidth - 1) && IsUndiscoveredCell(Index + 1))
		OutIndexes.AddUnique(Index + 1);
	//Up
	if (IsUndiscoveredCell(Index - GridWidth))
		OutIndexes.AddUnique(Index - GridWidth);
	//Up-Left
	if ((Index % GridWidth) > 0 && IsUndiscoveredCell(Index - GridWidth - 1))
		OutIndexes.AddUnique(Index - GridWidth - 1);
	//Up-Right
	if ((Index % GridWidth) < (GridWidth - 1) && IsUndiscoveredCell(Index - GridWidth + 1))
		OutIndexes.AddUnique(Index - GridWidth + 1);
	//Down
	if (IsUndiscoveredCell(Index + GridWidth))
		OutIndexes.AddUnique(Index + GridWidth);
	//Down-Left
	if ((Index % GridWidth) > 0 && IsUndiscoveredCell(Index + GridWidth - 1))
		OutIndexes.AddUnique(Index + GridWidth - 1);
	//Down-Right
	if ((Index % GridWidth) < (GridWidth - 1) && IsUndiscoveredCell(Index + GridWidth + 1))
		OutIndexes.AddUnique(Index + GridWidth + 1);

	if (OutIndexes.Contains(Index))
		OutIndexes.Remove(Index);

	return OutIndexes.Num() > 0;
}

bool SMineSweeperGrid::IsUndiscoveredCell(int Index)
{
	if (!_CellDatas.IsValidIndex(Index))
		return false;
	if (_CellDatas[Index]->bIsBeenDiscovered)
		return false;
	return true;
}
