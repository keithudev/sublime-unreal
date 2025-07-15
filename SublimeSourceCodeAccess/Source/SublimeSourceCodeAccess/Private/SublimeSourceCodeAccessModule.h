// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"
#include "Templates/SharedPointer.h"

class FSublimeSourceCodeAccessor;

class FSublimeSourceCodeAccessModule : public IModuleInterface
{
public:
	FSublimeSourceCodeAccessModule();

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	FSublimeSourceCodeAccessor& GetAccessor();

private:

	TSharedRef<FSublimeSourceCodeAccessor> SublimeSourceCodeAccessor;
};
