#pragma once

#include "Modules/ModuleManager.h"
#include "InputDebuggerModule.generated.h"

struct FInputDebuggerInputProcessor;

UINTERFACE(BlueprintType)
class UDebugKeyListener : public UInterface
{
	GENERATED_BODY()
};

class INPUTDEBUGGER_API IDebugKeyListener
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnKeyUp(const FKey& Key);

	UFUNCTION(BlueprintNativeEvent)
	void OnKeyDown(const FKey& Key);
};

class INPUTDEBUGGER_API FInputDebuggerModule final : public FDefaultModuleImpl
{
	using Super = FDefaultModuleImpl;

	friend struct FInputDebuggerInputProcessor;

	TSharedPtr<FInputDebuggerInputProcessor> InputProcessor;

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	TMap<FKey, TArray<TScriptInterface<UDebugKeyListener>>> Listeners;

public:
	static FInputDebuggerModule& GetModule();

	void RegisterListener(const FKey& Key, const TScriptInterface<UDebugKeyListener>& Listener);

	void UnregisterListener(const FKey& Key, const TScriptInterface<UDebugKeyListener>& Listener);
};
