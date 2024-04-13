static int f_midi_out_close(struct snd_rawmidi_substream *substream)
{
	struct f_midi *midi = substream->rmidi->private_data;

	VDBG(midi, "%s()\n", __func__);
	return 0;
}
