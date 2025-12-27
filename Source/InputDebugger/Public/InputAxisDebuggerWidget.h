#pragma once

#include "Components/Overlay.h"
#include "InputDebuggerModule.h"
#include "InputAxisDebuggerWidget.generated.h"

UCLASS()
class INPUTDEBUGGER_API UInputAxisDebuggerWidget : public UOverlay, public IDebugAxisListener
{
	GENERATED_BODY()

	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual void OnAxisValue_Implementation(const FKey& Key, float Value) override;

	bool bRegistered = false;

	UPROPERTY(EditAnywhere)
	bool bHorizontal = true;

	UPROPERTY(EditAnywhere)
	FKey Axis;
	
	void MoveChildren(const float Value);
};
