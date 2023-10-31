#include "ButtonCommand.h"
#include "EditorStyleSet.h"

FButtonCommand::FButtonCommand()
	: TCommands("ContextName", FText::FromString("ContextDesc"), NAME_None, FEditorStyle::GetStyleSetName())
{
	SpawnVertexCommand = MakeShareable(new FUICommandList());
}

FButtonCommand::~FButtonCommand()
{
	if (SpawnVertexCommand.IsValid())
		SpawnVertexCommand.Reset();
}

void FButtonCommand::RegisterCommands()
{
#define LOCTEXT_NAMESPACE ""
	UI_COMMAND(SpawnVertexButton, "SpawnVertexButton", "Spawn Vertex Button", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenViewerButton, "OpenViewerButton", "Open Viewer Button", EUserInterfaceActionType::Button, FInputChord());
#undef LOCTEXT_NAMESPACE

	SpawnVertexCommand->MapAction(SpawnVertexButton, FExecuteAction::CreateRaw(this, &FButtonCommand::SpawnVertexButton_Pressed));
	SpawnVertexCommand->MapAction(OpenViewerButton, FExecuteAction::CreateRaw(this, &FButtonCommand::OpenViewerButton_Pressed));
}


void FButtonCommand::SpawnVertexButton_Pressed()
{
	TArray<const FSlateBrush*> brushes;
	FEditorStyle::GetResources(brushes);
	
	for (const auto& brush : brushes)
		UE_LOG(LogTemp, Warning, TEXT("%s"), *brush->GetResourceName().ToString())
}

void FButtonCommand::OpenViewerButton_Pressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Viewer Button is Pressed"));
}