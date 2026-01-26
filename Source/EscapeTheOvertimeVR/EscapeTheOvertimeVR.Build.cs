// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EscapeTheOvertimeVR : ModuleRules
{
	public EscapeTheOvertimeVR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"NavigationSystem",
			"GeometryCollectionEngine",
			"HeadMountedDisplay", // 구형 VR API 지원
			"XRBase"             // 최신 OpenXR 표준 지원 (필수)
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"EscapeTheOvertimeVR",
			"EscapeTheOvertimeVR/MainClass",
            "EscapeTheOvertimeVR/MainClass/Objects",
            "EscapeTheOvertimeVR/MainClass/Objects/Doors",
            "EscapeTheOvertimeVR/MainClass/Objects/Doors/CabinetDoor",
            "EscapeTheOvertimeVR/MainClass/Objects/Doors/ElevatorDoors",
            "EscapeTheOvertimeVR/MainClass/Objects/Doors/MainDoors",
            "EscapeTheOvertimeVR/MainClass/Objects/Doors/RestroomDoors",
            "EscapeTheOvertimeVR/MainClass/Objects/Doors/LockerDoors",
            "EscapeTheOvertimeVR/MainClass/Objects/Doors/SinkDoors",
            "EscapeTheOvertimeVR/MainClass/Objects/Items",
            "EscapeTheOvertimeVR/MainClass/Objects/Items/Keycard",
            "EscapeTheOvertimeVR/MainClass/Objects/Items/Distraction",
            "EscapeTheOvertimeVR/MainClass/Objects/Items/Muffle",
            "EscapeTheOvertimeVR/MainClass/Objects/Items/SpeedUp",
            "EscapeTheOvertimeVR/MainClass/Objects/Items/Heal",
            "EscapeTheOvertimeVR/MainClass/Objects/Props",
            "EscapeTheOvertimeVR/MainClass/Objects/Props/KeycardReader",
            "EscapeTheOvertimeVR/MainClass/Objects/WorldObjects",
            "EscapeTheOvertimeVR/MainClass/Characters",
            "EscapeTheOvertimeVR/MainClass/Characters/Boss",
            "EscapeTheOvertimeVR/MainClass/Characters/Dancer",
            "EscapeTheOvertimeVR/MainClass/Game",
            "EscapeTheOvertimeVR/MainClass/Game/AnimNotifies",
            "EscapeTheOvertimeVR/Variant_Horror",
			"EscapeTheOvertimeVR/Variant_Horror/UI",
			"EscapeTheOvertimeVR/Variant_Shooter",
			"EscapeTheOvertimeVR/Variant_Shooter/AI",
			"EscapeTheOvertimeVR/Variant_Shooter/UI",
			"EscapeTheOvertimeVR/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
