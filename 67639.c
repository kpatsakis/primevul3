BOOL update_read_suppress_output(rdpUpdate* update, wStream* s)
{
	BYTE allowDisplayUpdates;

	if (Stream_GetRemainingLength(s) < 4)
		return FALSE;

	Stream_Read_UINT8(s, allowDisplayUpdates);
	Stream_Seek(s, 3); /* pad3Octects */

	if (allowDisplayUpdates > 0 && Stream_GetRemainingLength(s) < 8)
		return FALSE;

	if (update->context->settings->SuppressOutput)
		IFCALL(update->SuppressOutput, update->context, allowDisplayUpdates,
		       allowDisplayUpdates > 0 ? (RECTANGLE_16*) Stream_Pointer(s) : NULL);
	else
		WLog_Print(update->log, WLOG_WARN,
		           "ignoring suppress output request from client");

	return TRUE;
}
