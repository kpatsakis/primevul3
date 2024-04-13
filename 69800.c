NTSTATUS NTAPI KeSaveExtendedProcessorState (
    __in ULONG64 Mask,
    PXSTATE_SAVE XStateSave
    )
{
	if (KeSaveExtendedProcessorStatePtr)
	{
		return (KeSaveExtendedProcessorStatePtr) (Mask, XStateSave);
	}
	else
	{
		return STATUS_SUCCESS;
	}
}
