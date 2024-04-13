static int f_midi_do_transmit(struct f_midi *midi, struct usb_ep *ep)
{
	struct usb_request *req = NULL;
	unsigned int len, i;
	bool active = false;
	int err;

	/*
	 * We peek the request in order to reuse it if it fails to enqueue on
	 * its endpoint
	 */
	len = kfifo_peek(&midi->in_req_fifo, &req);
	if (len != 1) {
		ERROR(midi, "%s: Couldn't get usb request\n", __func__);
		return -1;
	}

	/*
	 * If buffer overrun, then we ignore this transmission.
	 * IMPORTANT: This will cause the user-space rawmidi device to block
	 * until a) usb requests have been completed or b) snd_rawmidi_write()
	 * times out.
	 */
	if (req->length > 0)
		return 0;

	for (i = midi->in_last_port; i < midi->in_ports; ++i) {
		struct gmidi_in_port *port = midi->in_ports_array + i;
		struct snd_rawmidi_substream *substream = port->substream;

		if (!port->active || !substream)
			continue;

		while (req->length + 3 < midi->buflen) {
			uint8_t b;

			if (snd_rawmidi_transmit(substream, &b, 1) != 1) {
				port->active = 0;
				break;
			}
			f_midi_transmit_byte(req, port, b);
		}

		active = !!port->active;
		if (active)
			break;
	}
	midi->in_last_port = active ? i : 0;

	if (req->length <= 0)
		goto done;

	err = usb_ep_queue(ep, req, GFP_ATOMIC);
	if (err < 0) {
		ERROR(midi, "%s failed to queue req: %d\n",
		      midi->in_ep->name, err);
		req->length = 0; /* Re-use request next time. */
	} else {
		/* Upon success, put request at the back of the queue. */
		kfifo_skip(&midi->in_req_fifo);
		kfifo_put(&midi->in_req_fifo, req);
	}

done:
	return active;
}
