// Copyright Epic Games, Inc. All Rights Reserved.

#include "SublimeSourceCodeAccessor.h"
#include "SublimeSourceCodeAccessModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/Paths.h"
#include "Misc/UProjectInfo.h"
#include "Misc/App.h"

#if PLATFORM_WINDOWS
#include "Internationalization/Regex.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(LogSublimeAccessor, Log, All);

#define LOCTEXT_NAMESPACE "SublimeSourceCodeAccessor"

namespace
{
	static const TCHAR* GSublimeExtension = TEXT(".sublime-project");
}

static FString MakePath(const FString& InPath)
{
	return TEXT("\"") + InPath + TEXT("\"");
}

FString FSublimeSourceCodeAccessor::GetSolutionPath() const
{
	FScopeLock Lock(&CachedSolutionPathCriticalSection);

	if (IsInGameThread())
	{
		CachedSolutionPath = FPaths::ProjectDir();

		if (!FUProjectDictionary::GetDefault().IsForeignProject(CachedSolutionPath))
		{
			CachedSolutionPath = FPaths::Combine(FPaths::RootDir(), FString("UE5") + GSublimeExtension);
		}
		else
		{
			FString BaseName = FApp::HasProjectName() ? FApp::GetProjectName() : FPaths::GetBaseFilename(CachedSolutionPath);
			CachedSolutionPath = FPaths::Combine(CachedSolutionPath, BaseName + GSublimeExtension);
		}
	}

	return CachedSolutionPath;
}

static void OnModuleCompileStarted(bool bIsAsyncCompile)
{
	FSublimeSourceCodeAccessModule& SublimeSourceCodeAccessModule = FModuleManager::LoadModuleChecked<FSublimeSourceCodeAccessModule>(TEXT("SublimeSourceCodeAccess"));
	SublimeSourceCodeAccessModule.GetAccessor().SaveAllOpenDocuments();
}

void FSublimeSourceCodeAccessor::Startup()
{
	FString SolutionDir = GetSolutionPath();
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!PlatformFile.FileExists(*SolutionDir))
	{
		FString SolutionFile = "{\"folders\":[{\"path\": \".\"}]}"; 
		FFileHelper::SaveStringToFile(*SolutionFile, *SolutionDir);
	}

	RefreshAvailability();
}

void FSublimeSourceCodeAccessor::RefreshAvailability()
{

#if PLATFORM_WINDOWS
	Location.URL = "C:\\Program Files\\Sublime Text\\subl.exe";
#elif PLATFORM_LINUX
	Location.URL = "HERE LINUX";
#elif PLATFORM_MAC
	Location.URL = "HERE MAC";
#endif
}

void FSublimeSourceCodeAccessor::Shutdown()
{
}

bool FSublimeSourceCodeAccessor::OpenSourceFiles(const TArray<FString>& AbsoluteSourcePaths)
{
	if (Location.IsValid())
	{
		FString SolutionDir = GetSolutionPath();
		TArray<FString> Args;
		Args.Add(MakePath(SolutionDir));

		for (const FString& SourcePath : AbsoluteSourcePaths)
		{
			Args.Add(MakePath(SourcePath));
		}

		return Launch(Args);
	}

	return false;
}

bool FSublimeSourceCodeAccessor::AddSourceFiles(const TArray<FString>& AbsoluteSourcePaths, const TArray<FString>& AvailableModules)
{
	return true;
}

bool FSublimeSourceCodeAccessor::OpenFileAtLine(const FString& FullPath, int32 LineNumber, int32 ColumnNumber)
{
	if (Location.IsValid())
	{
		LineNumber = LineNumber > 0 ? LineNumber : 1;
		ColumnNumber = ColumnNumber > 0 ? ColumnNumber : 1;

		FString SolutionDir = GetSolutionPath();
		TArray<FString> Args;
		Args.Add(MakePath(SolutionDir));
		Args.Add(MakePath(FullPath) + FString::Printf(TEXT(":%d:%d"), LineNumber, ColumnNumber));
		return Launch(Args);
	}

	return false;
}

bool FSublimeSourceCodeAccessor::CanAccessSourceCode() const
{
	return Location.IsValid();
}

FName FSublimeSourceCodeAccessor::GetFName() const
{
	return FName("Sublime");
}

FText FSublimeSourceCodeAccessor::GetNameText() const
{
	return LOCTEXT("SublimeDisplayName", "Sublime");
}

FText FSublimeSourceCodeAccessor::GetDescriptionText() const
{
	return LOCTEXT("SublimeDisplayDesc", "Open source code files in Sublime");
}

void FSublimeSourceCodeAccessor::Tick(const float DeltaTime)
{
}

bool FSublimeSourceCodeAccessor::OpenSolution()
{
	if (Location.IsValid())
	{
		return OpenSolutionAtPath(GetSolutionPath());
	}

	return false;
}

bool FSublimeSourceCodeAccessor::OpenSolutionAtPath(const FString& InSolutionPath)
{
	if (Location.IsValid())
	{
		FString SolutionPath = InSolutionPath;

		if (!SolutionPath.EndsWith(GSublimeExtension))
		{
			SolutionPath = SolutionPath + GSublimeExtension;
		}

		TArray<FString> Args;
		Args.Add(MakePath(SolutionPath));
		return Launch(Args);
	}

	return false;
}

bool FSublimeSourceCodeAccessor::DoesSolutionExist() const
{
	return FPaths::FileExists(GetSolutionPath());
}

bool FSublimeSourceCodeAccessor::SaveAllOpenDocuments() const
{
	return true;
}

bool FSublimeSourceCodeAccessor::Launch(const TArray<FString>& InArgs)
{
	if (Location.IsValid())
	{
		FString ArgsString;
		for (const FString& Arg : InArgs)
		{
			ArgsString.Append(Arg);
			ArgsString.Append(TEXT(" "));
		}

		uint32 ProcessID;
		FProcHandle hProcess = FPlatformProcess::CreateProc(*Location.URL, *ArgsString, true, false, false, &ProcessID, 0, nullptr, nullptr, nullptr);
		return hProcess.IsValid();
	}

	return false;
}

#undef LOCTEXT_NAMESPACE
