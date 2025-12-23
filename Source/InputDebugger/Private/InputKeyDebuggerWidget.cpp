#include "InputDebugger/Public/InputKeyDebuggerWidget.h"

UInputKeyDebuggerWidget::UInputKeyDebuggerWidget()
{
	KeyUpBrush.TintColor = FLinearColor::Black;
	KeyDownBrush.TintColor = FLinearColor::Red;
}

TSharedRef<SWidget> UInputKeyDebuggerWidget::RebuildWidget()
{
	if (!bRegistered)
	{
		FInputDebuggerModule::GetModule().RegisterListener(Key, this);
		bRegistered = true;
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
	bKeyDown = false;
}

void UInputKeyDebuggerWidget::OnKeyDown_Implementation(const FKey& InKey)
{
	bKeyDown = true;
}

FSlateBrush UInputKeyDebuggerWidget::GetBrush()
{
	return bKeyDown ? KeyDownBrush : KeyUpBrush;
}
