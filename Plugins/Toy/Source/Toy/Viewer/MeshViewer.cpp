#include "MeshViewer.h"

TSharedPtr<FMeshViewer> FMeshViewer::Instance = nullptr;
const static FName ToolkitName = TEXT("MeshViewer");
const static FName ViewportTabID = TEXT("Viewport");
const static FName PreviewTabID = TEXT("Preview");
const static FName DetailsTabID = TEXT("Details");

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

void FMeshViewer::Shutdown()
{
	if (Instance.IsValid())
		Instance->CloseWindow();
}

void FMeshViewer::OpenWindow_Internal(UObject* InAsset)
{
	//Create Layout
	TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("MeshViewer_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			//ToolBar
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)

			//Panel(대)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)

				//(중) Viewport
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.75f)
					->AddTab(ViewportTabID, ETabState::OpenedTab)
				)
				//END(중) Viewport

				//(중) RightPanel
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->SetSizeCoefficient(0.25f)

					//(소) Preview
					->Split
					(
						FTabManager::NewStack()
						->AddTab(PreviewTabID, ETabState::OpenedTab)
					)

					//(소) Details
					->Split
					(
						FTabManager::NewStack()
						->AddTab(DetailsTabID, ETabState::OpenedTab)
					)
				)//End(중) RightPanel
			)//End(대)
		);//End(Area)

	//Open Slate Window
	FAssetEditorToolkit::InitAssetEditor
	(
		EToolkitMode::Standalone,
		TSharedPtr<IToolkitHost>(),
		ToolkitName,
		layout,
		true,
		true,
		InAsset
	);
}

void FMeshViewer::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	FOnSpawnTab viewportSpawnEvent = FOnSpawnTab::CreateSP(this, &FMeshViewer::Spawn_ViewportTab);
	TabManager->RegisterTabSpawner(ViewportTabID, viewportSpawnEvent);
}

TSharedRef<SDockTab> FMeshViewer::Spawn_ViewportTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
		[
			SNew(SButton)
			.Text(FText::FromString("Test"))
		];
}

FName FMeshViewer::GetToolkitFName() const
{
	return ToolkitName;
}

FText FMeshViewer::GetBaseToolkitName() const
{
	return FText::FromName(ToolkitName);
}

FString FMeshViewer::GetWorldCentricTabPrefix() const
{
	return ToolkitName.ToString();
}

FLinearColor FMeshViewer::GetWorldCentricTabColorScale(void) const
{
	return FLinearColor();
}
