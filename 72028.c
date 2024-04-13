static void f_midi_drop_out_substreams(struct f_midi *midi)
{
	unsigned int i;

	for (i = 0; i < midi->in_ports; i++) {
		struct gmidi_in_port *port = midi->in_ports_array + i;
		struct snd_rawmidi_substream *substream = port->substream;

		if (port->active && substream)
			snd_rawmidi_drop_output(substream);
	}
}
