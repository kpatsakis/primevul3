process_update_pdu(STREAM s)
{
	uint16 update_type, count;

	in_uint16_le(s, update_type);

	ui_begin_update();
	switch (update_type)
	{
		case RDP_UPDATE_ORDERS:
			logger(Protocol, Debug, "%s(), RDP_UPDATE_ORDERS", __func__);

			in_uint8s(s, 2);	/* pad */
			in_uint16_le(s, count);
			in_uint8s(s, 2);	/* pad */
			process_orders(s, count);
			break;

		case RDP_UPDATE_BITMAP:
			logger(Protocol, Debug, "%s(), RDP_UPDATE_BITMAP", __func__);
			process_bitmap_updates(s);
			break;

		case RDP_UPDATE_PALETTE:
			logger(Protocol, Debug, "%s(), RDP_UPDATE_PALETTE", __func__);
			process_palette(s);
			break;

		case RDP_UPDATE_SYNCHRONIZE:
			logger(Protocol, Debug, "%s(), RDP_UPDATE_SYNCHRONIZE", __func__);
			break;

		default:
			logger(Protocol, Warning, "process_update_pdu(), unhandled update type %d",
			       update_type);
	}
	ui_end_update();
}
