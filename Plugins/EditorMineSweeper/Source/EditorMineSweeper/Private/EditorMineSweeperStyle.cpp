// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorMineSweeperStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FEditorMineSweeperStyle::StyleInstance = nullptr;

void FEditorMineSweeperStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FEditorMineSweeperStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FEditorMineSweeperStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("EditorMineSweeperStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FEditorMineSweeperStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("EditorMineSweeperStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("EditorMineSweeper")->GetBaseDir() / TEXT("Resources"));

	Style->Set("EditorMineSweeper.PluginAction", new IMAGE_BRUSH_SVG(TEXT("bomb"), Icon20x20));
	return Style;
}

void FEditorMineSweeperStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FEditorMineSweeperStyle::Get()
{
	return *StyleInstance;
}
