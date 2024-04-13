static GF_Err FFD_CloseService(GF_InputService *plug)
{
	FFDemux *ffd = (FFDemux*)plug->priv;

	ffd->is_running = 0;

#if FF_API_CLOSE_INPUT_FILE
	if (ffd->ctx) av_close_input_file(ffd->ctx);
#else
	if (ffd->ctx) avformat_close_input(&ffd->ctx);
#endif

	ffd->ctx = NULL;
	ffd->audio_ch = ffd->video_ch = NULL;
	ffd->audio_run = ffd->video_run = GF_FALSE;

	if (ffd->dnload) {
		if (ffd->is_running) {
			while (!ffd->is_running) gf_sleep(1);
			ffd->is_running = 0;
		}
		gf_service_download_del(ffd->dnload);
		ffd->dnload = NULL;
	}
	if (ffd->buffer) gf_free(ffd->buffer);
	ffd->buffer = NULL;

	gf_service_disconnect_ack(ffd->service, NULL, GF_OK);
#ifdef FFMPEG_DUMP_REMOTE
	if (ffd->outdbg) gf_fclose(ffd->outdbg);
#endif
	return GF_OK;
}
