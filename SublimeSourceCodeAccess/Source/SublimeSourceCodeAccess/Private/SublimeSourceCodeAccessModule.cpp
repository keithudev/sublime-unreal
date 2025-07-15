// Copyright Epic Games, Inc. All Rights Reserved. 

#include "SublimeSourceCodeAccessModule.h"
#include "Features/IModularFeatures.h"
#include "Modules/ModuleManager.h"
#include "HAL/LowLevelMemTracker.h"
#include "SublimeSourceCodeAccessor.h"

LLM_DEFINE_TAG(SublimeSourceCodeAccess);

IMPLEMENT_MODULE(FSublimeSourceCodeAccessModule, SublimeSourceCodeAccess);

#define LOCTEXT_NAMESPACE "SublimeSourceCodeAccessor"

FSublimeSourceCodeAccessModule::FSublimeSourceCodeAccessModule()
	: SublimeSourceCodeAccessor(MakeShareable(new FSublimeSourceCodeAccessor()))
{
}

void FSublimeSourceCodeAccessModule::StartupModule()
{
	LLM_SCOPE_BYTAG(SublimeSourceCodeAccess);

	SublimeSourceCodeAccessor->Startup();

	IModularFeatures::Get().RegisterModularFeature(TEXT("SourceCodeAccessor"), &SublimeSourceCodeAccessor.Get());
}

void FSublimeSourceCodeAccessModule::ShutdownModule()
{
	IModularFeatures::Get().UnregisterModularFeature(TEXT("SourceCodeAccessor"), &SublimeSourceCodeAccessor.Get());

	SublimeSourceCodeAccessor->Shutdown();
}

FSublimeSourceCodeAccessor& FSublimeSourceCodeAccessModule::GetAccessor()
{
	return SublimeSourceCodeAccessor.Get();
}

#undef LOCTEXT_NAMESPACE
