void HTMLButtonElement::accessKeyAction(bool sendMouseEvents)
{
    focus();

    dispatchSimulatedClick(0, sendMouseEvents ? SendMouseUpDownEvents : SendNoEvents);
}
