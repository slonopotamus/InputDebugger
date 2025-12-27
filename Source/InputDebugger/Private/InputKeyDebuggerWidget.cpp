#include "InputKeyDebuggerWidget.h"

UInputKeyDebuggerWidget::UInputKeyDebuggerWidget()
{
	SetVisibilityInternal(ESlateVisibility::Collapsed);
}

TSharedRef<SWidget> UInputKeyDebuggerWidget::RebuildWidget()
{
	if (!bRegistered)
	{
		FInputDebuggerModule::GetModule().RegisterKeyListener(Key, this);
		bRegistered = true;
	}

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
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInputKeyDebuggerWidget::OnKeyDown_Implementation(const FKey& InKey)
{
	SetVisibility(ESlateVisibility::Visible);
}
