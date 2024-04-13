static void f_midi_read_data(struct usb_ep *ep, int cable,
			     uint8_t *data, int length)
{
	struct f_midi *midi = ep->driver_data;
	struct snd_rawmidi_substream *substream = midi->out_substream[cable];

	if (!substream)
		/* Nobody is listening - throw it on the floor. */
		return;

	if (!test_bit(cable, &midi->out_triggered))
		return;

	snd_rawmidi_receive(substream, data, length);
}
