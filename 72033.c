static void f_midi_in_trigger(struct snd_rawmidi_substream *substream, int up)
{
	struct f_midi *midi = substream->rmidi->private_data;

	if (substream->number >= midi->in_ports)
		return;

	VDBG(midi, "%s() %d\n", __func__, up);
	midi->in_ports_array[substream->number].active = up;
	if (up)
		tasklet_hi_schedule(&midi->tasklet);
}
