// Plugins/UnrealMCP/Source/UnrealMCP/UnrealMCP.Build.cs
// UE 5.6 – konsistent mit IWYU & .Build.cs-Best-Practices
using UnrealBuildTool;

public class UnrealMCP : ModuleRules
{
    public UnrealMCP(ReadOnlyTargetRules Target) : base(Target)
    {
        IWYUSupport = IWYUSupport.Full;        // ersetzt bEnforceIWYU :contentReference[oaicite:1]{index=1}
        PCHUsage    = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core","CoreUObject","Engine","InputCore",
            "Networking","Sockets","HTTP","Json","JsonUtilities",
            "DeveloperSettings"
        });

        PrivateDependencyModuleNames.AddRange(new[]
        {
            // Editor-spezifisch
            "UnrealEd","EditorScriptingUtilities","EditorSubsystem",
            // UI
            "Slate","SlateCore","UMG","UMGEditor",
            // Blueprint / Kismet
            "Kismet","KismetCompiler","BlueprintGraph",
            // Asset / Project
            "Projects","AssetRegistry","ToolMenus","PropertyEditor"
        });

        // DynamicallyLoadedModuleNames … leer lassen – nichts wird zur Laufzeit nachgeladen
    }
}
