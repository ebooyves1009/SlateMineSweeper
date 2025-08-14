// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorMineSweeper.h"
#include "EditorMineSweeperStyle.h"
#include "EditorMineSweeperCommands.h"
#include "MineSweeperWindow.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName EditorMineSweeperTabName("EditorMineSweeper");

#define LOCTEXT_NAMESPACE "FEditorMineSweeperModule"

void FEditorMineSweeperModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FEditorMineSweeperStyle::Initialize();
	FEditorMineSweeperStyle::ReloadTextures();

	FEditorMineSweeperCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FEditorMineSweeperCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FEditorMineSweeperModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FEditorMineSweeperModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(EditorMineSweeperTabName, FOnSpawnTab::CreateRaw(this, &FEditorMineSweeperModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FEditorMineSweeperTabTitle", "MineSweeper"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FEditorMineSweeperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FEditorMineSweeperStyle::Shutdown();

	FEditorMineSweeperCommands::Unregister();
	
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(EditorMineSweeperTabName);
}

void FEditorMineSweeperModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(EditorMineSweeperTabName);
}

void FEditorMineSweeperModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FEditorMineSweeperCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FEditorMineSweeperCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

TSharedRef<SDockTab> FEditorMineSweeperModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SMineSweeperWindow)
		];
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorMineSweeperModule, EditorMineSweeper)