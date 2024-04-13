rdpsnd_process_packet(uint8 opcode, STREAM s)
{
	uint16 vol_left, vol_right;
	static uint16 tick, format;
	static uint8 packet_index;

	switch (opcode)
	{
		case SNDC_WAVE:
			in_uint16_le(s, tick);
			in_uint16_le(s, format);
			in_uint8(s, packet_index);
			in_uint8s(s, 3);
			logger(Sound, Debug,
			       "rdpsnd_process_packet(), RDPSND_WRITE(tick: %u, format: %u, index: %u, data: %u bytes)\n",
			       (unsigned) tick, (unsigned) format, (unsigned) packet_index,
			       (unsigned) s->size - 8);

			if (format >= MAX_FORMATS)
			{
				logger(Sound, Error,
				       "rdpsnd_process_packet(), invalid format index");
				break;
			}

			if (!device_open || (format != current_format))
			{
				/*
				 * If we haven't selected a device by now, then either
				 * we've failed to find a working device, or the server
				 * is sending bogus SNDC_WAVE.
				 */
				if (!current_driver)
				{
					rdpsnd_send_waveconfirm(tick, packet_index);
					break;
				}
				if (!device_open && !current_driver->wave_out_open())
				{
					rdpsnd_send_waveconfirm(tick, packet_index);
					break;
				}
				if (!current_driver->wave_out_set_format(&formats[format]))
				{
					rdpsnd_send_waveconfirm(tick, packet_index);
					current_driver->wave_out_close();
					device_open = False;
					break;
				}
				device_open = True;
				current_format = format;
			}

			rdpsnd_queue_write(rdpsnd_dsp_process
					   (s->p, s->end - s->p, current_driver,
					    &formats[current_format]), tick, packet_index);
			return;
			break;
		case SNDC_CLOSE:
			logger(Sound, Debug, "rdpsnd_process_packet(), SNDC_CLOSE()");
			if (device_open)
				current_driver->wave_out_close();
			device_open = False;
			break;
		case SNDC_FORMATS:
			rdpsnd_process_negotiate(s);
			break;
		case SNDC_TRAINING:
			rdpsnd_process_training(s);
			break;
		case SNDC_SETVOLUME:
			in_uint16_le(s, vol_left);
			in_uint16_le(s, vol_right);
			logger(Sound, Debug,
			       "rdpsnd_process_packet(), SNDC_SETVOLUME(left: 0x%04x (%u %%), right: 0x%04x (%u %%))",
			       (unsigned) vol_left, (unsigned) vol_left / 655, (unsigned) vol_right,
			       (unsigned) vol_right / 655);
			if (device_open)
				current_driver->wave_out_volume(vol_left, vol_right);
			break;
		default:
			logger(Sound, Warning, "rdpsnd_process_packet(), Unhandled opcode 0x%x",
			       opcode);
			break;
	}
}
