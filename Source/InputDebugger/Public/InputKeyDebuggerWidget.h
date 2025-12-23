#pragma once

#include "Components/Border.h"
#include "InputDebuggerModule.h"
#include "InputKeyDebuggerWidget.generated.h"

UCLASS()
class INPUTDEBUGGER_API UInputKeyDebuggerWidget : public UBorder, public IDebugKeyListener
{
	GENERATED_BODY()

	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
	virtual void OnKeyUp_Implementation(const FKey& InKey) override;
	
	virtual void OnKeyDown_Implementation(const FKey& InKey) override;
	
	UFUNCTION()
	FSlateBrush GetBrush();

	bool bRegistered = false;
	
	bool bKeyPressed = false;

	UPROPERTY(EditAnywhere)
	FKey Key;
	
	UPROPERTY(EditAnywhere, Category = "Appearance")
	FSlateBrush KeyPressedBrush;
};
