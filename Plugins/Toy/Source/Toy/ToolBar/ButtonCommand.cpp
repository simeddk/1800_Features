#include "ButtonCommand.h"
#include "EditorStyleSet.h"
#include "Interfaces/IMainFrameModule.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Serialization/BufferArchive.h"
#include "DetailPanel/DetailsButton.h"

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
	IMainFrameModule& mainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	void* handle = mainFrame.GetParentWindow()->GetNativeWindow()->GetOSWindowHandle();

	FString path;
	TArray<FString> fileNames;
	IDesktopPlatform* platform = FDesktopPlatformModule::Get();
	platform->OpenFileDialog(handle, "Load", path, "", "Binary File(*.bin)|*.bin", EFileDialogFlags::None, fileNames);
	if (fileNames.Num() < 1) return;

	FBufferArchive buffer;
	FFileHelper::LoadFileToArray(buffer, *fileNames[0]);
	FMemoryReader reader = FMemoryReader(buffer, true);
	buffer.Seek(0);

	FVertexData data;
	reader << data;
	reader.FlushCache();
	reader.Close();

	GLog->Logf(TEXT("Vertex Count : %d"), data.Positions.Num());
	GLog->Logf(TEXT("Index Count : %d"), data.Indices.Num());

}

void FButtonCommand::OpenViewerButton_Pressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Viewer Button is Pressed"));
}