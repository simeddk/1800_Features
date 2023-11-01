using UnrealBuildTool;

public class Toy : ModuleRules
{
	public Toy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.Add("Core");

		PrivateDependencyModuleNames.AddRange(
		new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"EditorStyle",
			"LevelEditor",
			"Projects",
			"GameplayDebugger",
			"PropertyEditor"
		});
		
	}
}
