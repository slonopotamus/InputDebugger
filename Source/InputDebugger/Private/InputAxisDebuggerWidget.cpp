#include "InputAxisDebuggerWidget.h"
#include "Components/OverlaySlot.h"

TSharedRef<SWidget> UInputAxisDebuggerWidget::RebuildWidget()
{
	if (!bRegistered)
	{
		FInputDebuggerModule::GetModule().RegisterAxisListener(Axis, this);
		bRegistered = true;
	}

	MoveChildren(0.5f);

	return Super::RebuildWidget();
}

void UInputAxisDebuggerWidget::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	if (bRegistered)
	{
		FInputDebuggerModule::GetModule().UnregisterAxisListener(Axis, this);
		bRegistered = false;
	}
}

void UInputAxisDebuggerWidget::OnAxisValue_Implementation(const FKey& Key, const float Value)
{
	MoveChildren(Value);
}

void UInputAxisDebuggerWidget::MoveChildren(const float Value)
{
	for (auto* MySlot : GetSlots())
	{
		if (auto* ChildSlot = Cast<UOverlaySlot>(MySlot); ensure(MySlot))
		{
			// TODO: Unhack this
			constexpr auto Size = 50;

			if (bHorizontal)
			{
				ChildSlot->SetPadding(FMargin{(Value + 1) * 0.5 * Size, 0, 0, 0});
			}
			else
			{
				ChildSlot->SetPadding(FMargin{0, (-Value + 1) * 0.5 * Size, 0, 0});
			}
		}
	}
}
