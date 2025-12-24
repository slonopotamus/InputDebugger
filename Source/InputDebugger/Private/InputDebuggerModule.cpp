#include "InputDebuggerModule.h"
#include "Framework/Application/IInputProcessor.h"

struct FInputDebuggerInputProcessor final : IInputProcessor
{
	explicit FInputDebuggerInputProcessor(FInputDebuggerModule& InputDebugger)
	    : InputDebugger{InputDebugger}
	{}

	FInputDebuggerModule& InputDebugger;

	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {}

	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		if (auto* ListenersPtr = InputDebugger.KeyListeners.Find(InKeyEvent.GetKey()))
		{
			for (auto& Listener : *ListenersPtr)
			{
				IDebugKeyListener::Execute_OnKeyUp(Listener.GetObject(), InKeyEvent.GetKey());
			}
		}

		return false;
	}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		if (auto* ListenersPtr = InputDebugger.KeyListeners.Find(InKeyEvent.GetKey()))
		{
			for (auto& Listener : *ListenersPtr)
			{
				IDebugKeyListener::Execute_OnKeyDown(Listener.GetObject(), InKeyEvent.GetKey());
			}
		}

		return false;
	}

	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override
	{
		if (auto* ListenersPtr = InputDebugger.AxisListeners.Find(InAnalogInputEvent.GetKey()))
		{
			for (auto& Listener : *ListenersPtr)
			{
				IDebugAxisListener::Execute_OnAxisValue(Listener.GetObject(), InAnalogInputEvent.GetKey(), InAnalogInputEvent.GetAnalogValue());
			}
		}

		return false;
	}
};

void FInputDebuggerModule::StartupModule()
{
	Super::StartupModule();

	if (FSlateApplication::IsInitialized())
	{
		InputProcessor = MakeShared<FInputDebuggerInputProcessor>(*this);
		// TODO: Priority?
		FSlateApplication::Get().RegisterInputPreProcessor(InputProcessor);
	}
}

void FInputDebuggerModule::ShutdownModule()
{
	if (FSlateApplication::IsInitialized() && InputProcessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);
	}

	Super::ShutdownModule();
}

FInputDebuggerModule& FInputDebuggerModule::GetModule()
{
	static const FName ModuleName{TEXT("InputDebugger")};
	return FModuleManager::LoadModuleChecked<FInputDebuggerModule>(ModuleName);
}

void FInputDebuggerModule::RegisterKeyListener(const FKey& Key, const TScriptInterface<UDebugKeyListener>& Listener)
{
	if (auto& Arr = KeyListeners.FindOrAdd(Key); ensure(!Arr.Contains(Listener)))
	{
		Arr.AddUnique(Listener);
	}
}

void FInputDebuggerModule::UnregisterKeyListener(const FKey& Key, const TScriptInterface<UDebugKeyListener>& Listener)
{
	if (auto* ListenersPtr = KeyListeners.Find(Key); ensure(ListenersPtr) && ensure(ListenersPtr->Remove(Listener)) && ListenersPtr->IsEmpty())
	{
		KeyListeners.Remove(Key);
	}
}

void FInputDebuggerModule::RegisterAxisListener(const FKey& Key, const TScriptInterface<UDebugAxisListener>& Listener)
{
	if (auto& Arr = AxisListeners.FindOrAdd(Key); ensure(!Arr.Contains(Listener)))
	{
		Arr.AddUnique(Listener);
	}
}

void FInputDebuggerModule::UnregisterAxisListener(const FKey& Key, const TScriptInterface<UDebugAxisListener>& Listener)
{
	if (auto* ListenersPtr = AxisListeners.Find(Key); ensure(ListenersPtr) && ensure(ListenersPtr->Remove(Listener)) && ListenersPtr->IsEmpty())
	{
		KeyListeners.Remove(Key);
	}
}

IMPLEMENT_MODULE(FInputDebuggerModule, InputDebugger)
