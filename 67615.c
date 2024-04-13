static BOOL update_begin_paint(rdpContext* context)
{
	wStream* s;
	rdpUpdate* update = context->update;

	if (update->us)
		update->EndPaint(context);

	s = fastpath_update_pdu_init_new(context->rdp->fastpath);

	if (!s)
		return FALSE;

	Stream_SealLength(s);
	Stream_Seek(s, 2); /* numberOrders (2 bytes) */
	update->combineUpdates = TRUE;
	update->numberOrders = 0;
	update->us = s;
	return TRUE;
}
