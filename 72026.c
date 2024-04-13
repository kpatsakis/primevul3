f_midi_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct f_midi *midi = ep->driver_data;
	struct usb_composite_dev *cdev = midi->func.config->cdev;
	int status = req->status;

	switch (status) {
	case 0:			 /* normal completion */
		if (ep == midi->out_ep) {
			/* We received stuff. req is queued again, below */
			f_midi_handle_out_data(ep, req);
		} else if (ep == midi->in_ep) {
			/* Our transmit completed. See if there's more to go.
			 * f_midi_transmit eats req, don't queue it again. */
			req->length = 0;
			f_midi_transmit(midi);
			return;
		}
		break;

	/* this endpoint is normally active while we're configured */
	case -ECONNABORTED:	/* hardware forced ep reset */
	case -ECONNRESET:	/* request dequeued */
	case -ESHUTDOWN:	/* disconnect from host */
		VDBG(cdev, "%s gone (%d), %d/%d\n", ep->name, status,
				req->actual, req->length);
		if (ep == midi->out_ep) {
			f_midi_handle_out_data(ep, req);
			/* We don't need to free IN requests because it's handled
			 * by the midi->in_req_fifo. */
			free_ep_req(ep, req);
		}
		return;

	case -EOVERFLOW:	/* buffer overrun on read means that
				 * we didn't provide a big enough buffer.
				 */
	default:
		DBG(cdev, "%s complete --> %d, %d/%d\n", ep->name,
				status, req->actual, req->length);
		break;
	case -EREMOTEIO:	/* short read */
		break;
	}

	status = usb_ep_queue(ep, req, GFP_ATOMIC);
	if (status) {
		ERROR(cdev, "kill %s:  resubmit %d bytes --> %d\n",
				ep->name, req->length, status);
		usb_ep_set_halt(ep);
		/* FIXME recover later ... somehow */
	}
}
