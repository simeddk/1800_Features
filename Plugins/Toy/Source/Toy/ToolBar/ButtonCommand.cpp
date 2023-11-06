#include "ButtonCommand.h"
#include "EditorStyleSet.h"
#include "Interfaces/IMainFrameModule.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Serialization/BufferArchive.h"
#include "DetailPanel/DetailsButton.h"
#include "LevelEditorViewport.h"
#include "Misc/FileHelper.h"
#include "Viewer/MeshViewer.h"
#include "RHI/CLoadMesh.h"
#include "Assets/CDetailAsset.h"

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
	//----------------------------------------------------------------------------
	// @@ Open File Dialog
	//----------------------------------------------------------------------------
	IMainFrameModule& mainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	void* handle = mainFrame.GetParentWindow()->GetNativeWindow()->GetOSWindowHandle();

	FString path;
	TArray<FString> fileNames;
	IDesktopPlatform* platform = FDesktopPlatformModule::Get();
	platform->OpenFileDialog(handle, "Load", path, "", "Binary File(*.bin)|*.bin", EFileDialogFlags::None, fileNames);
	if (fileNames.Num() < 1) return;

	// -> File Archive
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

#ifdef TEST_CSV
	// -> Save File(*_opened.csv)
	FString planeText;
	for (int32 i = 0; i < data.Positions.Num(); i++)
	{
		planeText.Append(data.Positions[i].ToString() + ",");
		planeText.Append(data.Normals[i].ToString() + ",");
		planeText.Append(data.UVs[i].ToString() + "\n");
	}

	FString planeTextPath = FPaths::GetBaseFilename(fileNames[0], false);
	planeTextPath.Append("_opened.csv");
	FFileHelper::SaveStringToFile(planeText, *planeTextPath);
#endif

	//Spawn LoadMesh from Binary File
	FLevelEditorViewportClient* viewportClient = (FLevelEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
	FVector start = viewportClient->GetViewLocation();
	FVector end = start + viewportClient->GetViewRotation().RotateVector(FVector(1e+4f, 0, 0));


	UWorld* world = GEditor->GetEditorWorldContext().World();
	FHitResult hitresult;
	world->LineTraceSingleByChannel(hitresult, start, end, ECollisionChannel::ECC_Visibility);
	
	if (hitresult.bBlockingHit == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Out Of Range!!"));
		return;
	}

	FTransform transform;
	transform.SetLocation(hitresult.ImpactPoint);
	FVector direction = (hitresult.TraceEnd - hitresult.TraceStart).GetSafeNormal();
	FRotator rotation = FRotator(0, direction.Rotation().Yaw, 0);
	transform.SetRotation(FQuat(rotation));

	ACLoadMesh* spawnedActor = world->SpawnActorDeferred<ACLoadMesh>
	(
		ACLoadMesh::StaticClass(),
		transform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	spawnedActor->SetPositions(data.Positions);
	spawnedActor->SetIndices(data.Indices);
	spawnedActor->SetNormals(data.Normals);
	spawnedActor->SetUVs(data.UVs);
	spawnedActor->SetColors(data.Colors);

	spawnedActor->FinishSpawning(transform);
}

void FButtonCommand::OpenViewerButton_Pressed()
{
	FMeshViewer::OpenWindow(NewObject<UCDetailAsset>());
}