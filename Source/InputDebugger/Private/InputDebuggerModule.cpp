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
		if (auto* ListenersPtr = InputDebugger.Listeners.Find(InKeyEvent.GetKey()))
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
		if (auto* ListenersPtr = InputDebugger.Listeners.Find(InKeyEvent.GetKey()))
		{
			for (auto& Listener : *ListenersPtr)
			{
				IDebugKeyListener::Execute_OnKeyDown(Listener.GetObject(), InKeyEvent.GetKey());
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

void FInputDebuggerModule::RegisterListener(const FKey& Key, const TScriptInterface<UDebugKeyListener>& Listener)
{
	Listeners.FindOrAdd(Key).Add(Listener);
}

void FInputDebuggerModule::UnregisterListener(const FKey& Key, const TScriptInterface<UDebugKeyListener>& Listener)
{
	if (auto* ListenersPtr = Listeners.Find(Key))
	{
		ListenersPtr->Remove(Listener);

		if (ListenersPtr->IsEmpty())
		{
			Listeners.Remove(Key);
		}
	}
}

IMPLEMENT_MODULE(FInputDebuggerModule, InputDebugger)
