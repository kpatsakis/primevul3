static void f_midi_free(struct usb_function *f)
{
	struct f_midi *midi;
	struct f_midi_opts *opts;

	midi = func_to_midi(f);
	opts = container_of(f->fi, struct f_midi_opts, func_inst);
	mutex_lock(&opts->lock);
	if (!--midi->free_ref) {
		kfree(midi->id);
		kfifo_free(&midi->in_req_fifo);
		kfree(midi);
		--opts->refcnt;
	}
	mutex_unlock(&opts->lock);
}
