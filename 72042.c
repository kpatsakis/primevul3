static void f_midi_transmit_byte(struct usb_request *req,
				 struct gmidi_in_port *port, uint8_t b)
{
	uint8_t p[4] = { port->cable << 4, 0, 0, 0 };
	uint8_t next_state = STATE_INITIAL;

	switch (b) {
	case 0xf8 ... 0xff:
		/* System Real-Time Messages */
		p[0] |= 0x0f;
		p[1] = b;
		next_state = port->state;
		port->state = STATE_REAL_TIME;
		break;

	case 0xf7:
		/* End of SysEx */
		switch (port->state) {
		case STATE_SYSEX_0:
			p[0] |= 0x05;
			p[1] = 0xf7;
			next_state = STATE_FINISHED;
			break;
		case STATE_SYSEX_1:
			p[0] |= 0x06;
			p[1] = port->data[0];
			p[2] = 0xf7;
			next_state = STATE_FINISHED;
			break;
		case STATE_SYSEX_2:
			p[0] |= 0x07;
			p[1] = port->data[0];
			p[2] = port->data[1];
			p[3] = 0xf7;
			next_state = STATE_FINISHED;
			break;
		default:
			/* Ignore byte */
			next_state = port->state;
			port->state = STATE_INITIAL;
		}
		break;

	case 0xf0 ... 0xf6:
		/* System Common Messages */
		port->data[0] = port->data[1] = 0;
		port->state = STATE_INITIAL;
		switch (b) {
		case 0xf0:
			port->data[0] = b;
			port->data[1] = 0;
			next_state = STATE_SYSEX_1;
			break;
		case 0xf1:
		case 0xf3:
			port->data[0] = b;
			next_state = STATE_1PARAM;
			break;
		case 0xf2:
			port->data[0] = b;
			next_state = STATE_2PARAM_1;
			break;
		case 0xf4:
		case 0xf5:
			next_state = STATE_INITIAL;
			break;
		case 0xf6:
			p[0] |= 0x05;
			p[1] = 0xf6;
			next_state = STATE_FINISHED;
			break;
		}
		break;

	case 0x80 ... 0xef:
		/*
		 * Channel Voice Messages, Channel Mode Messages
		 * and Control Change Messages.
		 */
		port->data[0] = b;
		port->data[1] = 0;
		port->state = STATE_INITIAL;
		if (b >= 0xc0 && b <= 0xdf)
			next_state = STATE_1PARAM;
		else
			next_state = STATE_2PARAM_1;
		break;

	case 0x00 ... 0x7f:
		/* Message parameters */
		switch (port->state) {
		case STATE_1PARAM:
			if (port->data[0] < 0xf0)
				p[0] |= port->data[0] >> 4;
			else
				p[0] |= 0x02;

			p[1] = port->data[0];
			p[2] = b;
			/* This is to allow Running State Messages */
			next_state = STATE_1PARAM;
			break;
		case STATE_2PARAM_1:
			port->data[1] = b;
			next_state = STATE_2PARAM_2;
			break;
		case STATE_2PARAM_2:
			if (port->data[0] < 0xf0)
				p[0] |= port->data[0] >> 4;
			else
				p[0] |= 0x03;

			p[1] = port->data[0];
			p[2] = port->data[1];
			p[3] = b;
			/* This is to allow Running State Messages */
			next_state = STATE_2PARAM_1;
			break;
		case STATE_SYSEX_0:
			port->data[0] = b;
			next_state = STATE_SYSEX_1;
			break;
		case STATE_SYSEX_1:
			port->data[1] = b;
			next_state = STATE_SYSEX_2;
			break;
		case STATE_SYSEX_2:
			p[0] |= 0x04;
			p[1] = port->data[0];
			p[2] = port->data[1];
			p[3] = b;
			next_state = STATE_SYSEX_0;
			break;
		}
		break;
	}

	/* States where we have to write into the USB request */
	if (next_state == STATE_FINISHED ||
	    port->state == STATE_SYSEX_2 ||
	    port->state == STATE_1PARAM ||
	    port->state == STATE_2PARAM_2 ||
	    port->state == STATE_REAL_TIME) {

		unsigned int length = req->length;
		u8 *buf = (u8 *)req->buf + length;

		memcpy(buf, p, sizeof(p));
		req->length = length + sizeof(p);

		if (next_state == STATE_FINISHED) {
			next_state = STATE_INITIAL;
			port->data[0] = port->data[1] = 0;
		}
	}

	port->state = next_state;
}
