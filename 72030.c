static void f_midi_handle_out_data(struct usb_ep *ep, struct usb_request *req)
{
	unsigned int i;
	u8 *buf = req->buf;

	for (i = 0; i + 3 < req->actual; i += 4)
		if (buf[i] != 0) {
			int cable = buf[i] >> 4;
			int length = f_midi_cin_length[buf[i] & 0x0f];
			f_midi_read_data(ep, cable, &buf[i + 1], length);
		}
}
