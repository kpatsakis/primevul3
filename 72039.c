static void f_midi_rmidi_free(struct snd_rawmidi *rmidi)
{
	f_midi_free(rmidi->private_data);
}
