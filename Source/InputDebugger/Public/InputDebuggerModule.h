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

UINTERFACE(BlueprintType)
class UDebugAxisListener : public UInterface
{
	GENERATED_BODY()
};

class INPUTDEBUGGER_API IDebugAxisListener
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnAxisValue(const FKey& Key, float Value);
};

class INPUTDEBUGGER_API FInputDebuggerModule final : public FDefaultModuleImpl
{
	using Super = FDefaultModuleImpl;

	friend struct FInputDebuggerInputProcessor;

	TSharedPtr<FInputDebuggerInputProcessor> InputProcessor;

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	TMap<FKey, TArray<TScriptInterface<IDebugKeyListener>>> KeyListeners;

	TMap<FKey, TArray<TScriptInterface<IDebugAxisListener>>> AxisListeners;

public:
	static FInputDebuggerModule& GetModule();

	void RegisterKeyListener(const FKey& Key, const TScriptInterface<IDebugKeyListener>& Listener);

	void UnregisterKeyListener(const FKey& Key, const TScriptInterface<IDebugKeyListener>& Listener);

	void RegisterAxisListener(const FKey& Key, const TScriptInterface<IDebugAxisListener>& Listener);

	void UnregisterAxisListener(const FKey& Key, const TScriptInterface<IDebugAxisListener>& Listener);
};
