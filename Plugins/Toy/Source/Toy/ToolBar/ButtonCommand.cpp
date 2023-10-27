#include "ButtonCommand.h"

FButtonCommand::FButtonCommand()
	: TCommands("ContextName", FText::FromString("ContextDesc"), NAME_None, FEditorStyle::GetStyleSetName())
{
}

FButtonCommand::~FButtonCommand()
{
}

void FButtonCommand::RegisterCommands()
{
//#define
	//UI_COMMAND(CommandInfo, "LoadMesh", "Load Mesh Data", EUserInterfaceActionType::Button, FInputChord());
//undef
}
