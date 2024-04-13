static u32 FFDemux_Run(void *par)
{
	AVPacket pkt;
	s64 seek_to;
	GF_NetworkCommand com;
	GF_NetworkCommand map;
	GF_SLHeader slh;
	FFDemux *ffd = (FFDemux *) par;

	memset(&map, 0, sizeof(GF_NetworkCommand));
	map.command_type = GF_NET_CHAN_MAP_TIME;

	memset(&com, 0, sizeof(GF_NetworkCommand));
	com.command_type = GF_NET_BUFFER_QUERY;

	memset(&slh, 0, sizeof(GF_SLHeader));

	slh.compositionTimeStampFlag = slh.decodingTimeStampFlag = 1;

	while (ffd->is_running) {
		if (!ffd->video_ch && !ffd->audio_ch) {
			gf_sleep(100);
			continue;
		}

		if ((ffd->seek_time>=0) && ffd->seekable) {
			seek_to = (s64) (AV_TIME_BASE*ffd->seek_time);
			av_seek_frame(ffd->ctx, -1, seek_to, AVSEEK_FLAG_BACKWARD);
			ffd->seek_time = -1;
		}
		pkt.stream_index = -1;
		/*EOF*/
		if (av_read_frame(ffd->ctx, &pkt) <0) break;
		if (pkt.pts == AV_NOPTS_VALUE) pkt.pts = pkt.dts;
		if (!pkt.dts) pkt.dts = pkt.pts;

		slh.compositionTimeStamp = pkt.pts;
		slh.decodingTimeStamp = pkt.dts;

		gf_mx_p(ffd->mx);
		/*blindly send audio as soon as video is init*/
		if (ffd->audio_ch && (pkt.stream_index == ffd->audio_st) ) {
			slh.compositionTimeStamp *= ffd->audio_tscale.num;
			slh.decodingTimeStamp *= ffd->audio_tscale.num;

			gf_service_send_packet(ffd->service, ffd->audio_ch, (char *) pkt.data, pkt.size, &slh, GF_OK);
		}
		else if (ffd->video_ch && (pkt.stream_index == ffd->video_st)) {
			slh.compositionTimeStamp *= ffd->video_tscale.num;
			slh.decodingTimeStamp *= ffd->video_tscale.num;
			slh.randomAccessPointFlag = pkt.flags&AV_PKT_FLAG_KEY ? 1 : 0;
			gf_service_send_packet(ffd->service, ffd->video_ch, (char *) pkt.data, pkt.size, &slh, GF_OK);
		}
		gf_mx_v(ffd->mx);
		av_free_packet(&pkt);

		/*sleep untill the buffer occupancy is too low - note that this work because all streams in this
		demuxer are synchronized*/
		while (ffd->audio_run || ffd->video_run) {
			gf_service_command(ffd->service, &com, GF_OK);
			if (com.buffer.occupancy < com.buffer.max)
				break;

			gf_sleep(1);
		}

		if (!ffd->audio_run && !ffd->video_run) break;
	}
	/*signal EOS*/
	if (ffd->audio_ch) gf_service_send_packet(ffd->service, ffd->audio_ch, NULL, 0, NULL, GF_EOS);
	if (ffd->video_ch) gf_service_send_packet(ffd->service, ffd->video_ch, NULL, 0, NULL, GF_EOS);
	ffd->is_running = 2;

	return 0;
}
