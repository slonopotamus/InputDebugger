#include "InputDebugger/Public/InputKeyDebuggerWidget.h"

TSharedRef<SWidget> UInputKeyDebuggerWidget::RebuildWidget()
{
	if (!bRegistered)
	{
		FInputDebuggerModule::GetModule().RegisterListener(Key, this);
	}

	BackgroundDelegate.BindDynamic(this, &ThisClass::GetBrush);

	return Super::RebuildWidget();
}

void UInputKeyDebuggerWidget::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	if (bRegistered)
	{
		FInputDebuggerModule::GetModule().UnregisterListener(Key, this);
		bRegistered = false;
	}
}

void UInputKeyDebuggerWidget::OnKeyUp_Implementation(const FKey& InKey)
{
	bKeyPressed = false;
}

void UInputKeyDebuggerWidget::OnKeyDown_Implementation(const FKey& InKey)
{
	bKeyPressed = true;
}

FSlateBrush UInputKeyDebuggerWidget::GetBrush()
{
	return bKeyPressed ? KeyPressedBrush : Background;
}
