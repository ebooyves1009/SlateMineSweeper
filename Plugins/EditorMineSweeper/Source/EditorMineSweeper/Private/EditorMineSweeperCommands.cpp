// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorMineSweeperCommands.h"

#define LOCTEXT_NAMESPACE "FEditorMineSweeperModule"

void FEditorMineSweeperCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "EditorMineSweeper", "Open Editor Mine Sweeper Mini-Game", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
