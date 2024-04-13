static void f_midi_out_trigger(struct snd_rawmidi_substream *substream, int up)
{
	struct f_midi *midi = substream->rmidi->private_data;

	VDBG(midi, "%s()\n", __func__);

	if (up)
		set_bit(substream->number, &midi->out_triggered);
	else
		clear_bit(substream->number, &midi->out_triggered);
}
