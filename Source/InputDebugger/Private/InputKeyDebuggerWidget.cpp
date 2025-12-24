#include "InputDebugger/Public/InputKeyDebuggerWidget.h"

UInputKeyDebuggerWidget::UInputKeyDebuggerWidget()
{
	KeyUpBrushColor = FLinearColor::Black;
	KeyDownBrushColor = FLinearColor::Red;
}

TSharedRef<SWidget> UInputKeyDebuggerWidget::RebuildWidget()
{
	if (!bRegistered)
	{
		FInputDebuggerModule::GetModule().RegisterKeyListener(Key, this);
		bRegistered = true;
	}

	BrushColorDelegate.BindDynamic(this, &ThisClass::GetBrushColor);

	return Super::RebuildWidget();
}

void UInputKeyDebuggerWidget::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	if (bRegistered)
	{
		FInputDebuggerModule::GetModule().UnregisterKeyListener(Key, this);
		bRegistered = false;
	}
}

void UInputKeyDebuggerWidget::OnKeyUp_Implementation(const FKey& InKey)
{
	bKeyDown = false;
}

void UInputKeyDebuggerWidget::OnKeyDown_Implementation(const FKey& InKey)
{
	bKeyDown = true;
}

FLinearColor UInputKeyDebuggerWidget::GetBrushColor()
{
	return bKeyDown ? KeyDownBrushColor : KeyUpBrushColor;
}
