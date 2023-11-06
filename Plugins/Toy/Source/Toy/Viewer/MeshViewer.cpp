#include "MeshViewer.h"

TSharedPtr<FMeshViewer> FMeshViewer::Instance = nullptr;

void FMeshViewer::OpenWindow(UObject* InAsset)
{
	if (Instance.IsValid())
	{
		Instance->CloseWindow();
		Instance = nullptr;
	}

	Instance = MakeShareable(new FMeshViewer());
	Instance->OpenWindow_Internal(InAsset);
}

void FMeshViewer::Shutdow()
{
	if (Instance.IsValid())
		Instance.Reset();
}

void FMeshViewer::OpenWindow_Internal(UObject* InAsset)
{
	TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("MeshViewer_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
		);

	//Todo. 12121221
	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone/*별도의 창으로*/, TSharedPtr<IToolkitHost>(), ToolkitName, layout, true/*검색창*/, true/*툴바아이콘*/, InAsset);
}

FName FMeshViewer::GetToolkitFName() const
{
	return FName();
}

FText FMeshViewer::GetBaseToolkitName() const
{
	return FText();
}

FString FMeshViewer::GetWorldCentricTabPrefix() const
{
	return FString();
}

FLinearColor FMeshViewer::GetWorldCentricTabColorScale(void) const
{
	return FLinearColor();
}
