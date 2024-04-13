static void f_midi_transmit(struct f_midi *midi)
{
	struct usb_ep *ep = midi->in_ep;
	int ret;
	unsigned long flags;

	/* We only care about USB requests if IN endpoint is enabled */
	if (!ep || !ep->enabled)
		goto drop_out;

	spin_lock_irqsave(&midi->transmit_lock, flags);

	do {
		ret = f_midi_do_transmit(midi, ep);
		if (ret < 0) {
			spin_unlock_irqrestore(&midi->transmit_lock, flags);
			goto drop_out;
		}
	} while (ret);

	spin_unlock_irqrestore(&midi->transmit_lock, flags);

	return;

drop_out:
	f_midi_drop_out_substreams(midi);
}
