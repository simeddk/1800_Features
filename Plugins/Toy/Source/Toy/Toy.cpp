#include "Toy.h"
#include "LevelEditor.h"
#include "ToolBar/ButtonCommand.h"

#define LOCTEXT_NAMESPACE "FToyModule"

void FToyModule::StartupModule()
{
	UE_LOG(LogTemp, Error, TEXT("Startup Toy Module"));

	FButtonCommand::Register();

	Extender = MakeShareable(new FExtender());
	FToolBarExtensionDelegate toolBarExtensionDelegate = FToolBarExtensionDelegate::CreateRaw(this, &FToyModule::AddToolBar);
	Extender->AddToolBarExtension("Compile", EExtensionHook::Before, FButtonCommand::Get().SpawnVertexCommand, toolBarExtensionDelegate);

	FLevelEditorModule& levelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	levelEditor.GetToolBarExtensibilityManager()->AddExtender(Extender);
}

void FToyModule::ShutdownModule()
{
	UE_LOG(LogTemp, Error, TEXT("Shutdown Toy Module"));
}

void FToyModule::AddToolBar(FToolBarBuilder& InToolBarBuilder)
{
	FSlateIcon icon = FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.SelectMode");

	InToolBarBuilder.AddSeparator();
	InToolBarBuilder.AddToolBarButton
	(
		FButtonCommand::Get().SpawnVertexButton,
		NAME_None,
		FText::FromString("Spawn Vertex"),
		FText::FromString("Spawn Vertex from Binary File"),
		icon
	);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FToyModule, Toy)