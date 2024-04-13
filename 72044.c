static inline void f_midi_unregister_card(struct f_midi *midi)
{
	if (midi->card) {
		snd_card_free(midi->card);
		midi->card = NULL;
	}
}
