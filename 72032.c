static void f_midi_in_tasklet(unsigned long data)
{
	struct f_midi *midi = (struct f_midi *) data;
	f_midi_transmit(midi);
}
