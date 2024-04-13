static int ff_url_read(void *h, unsigned char *buf, int size)
{
	u32 retry = 10;
	u32 read;
	int full_size;
	FFDemux *ffd = (FFDemux *)h;

	full_size = 0;
	if (ffd->buffer_used) {
		if (ffd->buffer_used >= (u32) size) {
			ffd->buffer_used-=size;
			memcpy(ffd->buffer, ffd->buffer+size, sizeof(char)*ffd->buffer_used);
#ifdef FFMPEG_DUMP_REMOTE
			if (ffd->outdbg) gf_fwrite(buf, size, 1, ffd->outdbg);
#endif
			return size;
		}
		full_size += ffd->buffer_used;
		buf += ffd->buffer_used;
		size -= ffd->buffer_used;
		ffd->buffer_used = 0;
	}

	while (size) {
		GF_Err e = gf_dm_sess_fetch_data(ffd->dnload, buf, size, &read);
		if (e==GF_EOS) break;
		/*we're sync!!*/
		if (e==GF_IP_NETWORK_EMPTY) {
			if (!retry) {
				GF_LOG(GF_LOG_ERROR, GF_LOG_CONTAINER, ("[FFMPEG Demuxer] timeout fetching bytes from network\n") );
				return -1;
			}
			retry --;
			gf_sleep(100);
			continue;
		}
		if (e) {
			GF_LOG(GF_LOG_ERROR, GF_LOG_CONTAINER, ("[FFMPEG Demuxer] error fetching bytes from network: %s\n", gf_error_to_string(e) ) );
			return -1;
		}
		full_size += read;
		if (read==size) break;
		size -= read;
		buf += read;
	}
#ifdef FFMPEG_DUMP_REMOTE
	if (ffd->outdbg) gf_fwrite(ffd->buffer, full_size, 1, ffd->outdbg);
#endif
	return full_size ? (int) full_size : -1;
}
