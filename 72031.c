static int f_midi_in_open(struct snd_rawmidi_substream *substream)
{
	struct f_midi *midi = substream->rmidi->private_data;
	struct gmidi_in_port *port;

	if (substream->number >= midi->in_ports)
		return -EINVAL;

	VDBG(midi, "%s()\n", __func__);
	port = midi->in_ports_array + substream->number;
	port->substream = substream;
	port->state = STATE_INITIAL;
	return 0;
}
