static void f_midi_unbind(struct usb_configuration *c, struct usb_function *f)
{
	struct usb_composite_dev *cdev = f->config->cdev;
	struct f_midi *midi = func_to_midi(f);
	struct snd_card *card;

	DBG(cdev, "unbind\n");

	/* just to be sure */
	f_midi_disable(f);

	card = midi->card;
	midi->card = NULL;
	if (card)
		snd_card_free_when_closed(card);

	usb_free_all_descriptors(f);
}
