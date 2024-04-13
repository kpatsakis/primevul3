GF_AUContext *gf_sm_stream_au_new(GF_StreamContext *stream, u64 timing, Double time_sec, Bool isRap)
{
	u32 i;
	GF_AUContext *tmp;
	u64 tmp_timing;

	tmp_timing = timing ? timing : (u64) (time_sec*1000);
	if (stream->imp_exp_time >= tmp_timing) {
		/*look for existing AU*/
		i=0;
		while ((tmp = (GF_AUContext *)gf_list_enum(stream->AUs, &i))) {
			if (timing && (tmp->timing==timing)) return tmp;
			else if (time_sec && (tmp->timing_sec == time_sec)) return tmp;
			else if (!time_sec && !timing && !tmp->timing && !tmp->timing_sec) return tmp;
			/*insert AU*/
			else if ((time_sec && time_sec<tmp->timing_sec) || (timing && timing<tmp->timing)) {
				GF_SAFEALLOC(tmp, GF_AUContext);
				if (!tmp) return NULL;
				tmp->commands = gf_list_new();
				if (isRap) tmp->flags = GF_SM_AU_RAP;
				tmp->timing = timing;
				tmp->timing_sec = time_sec;
				tmp->owner = stream;
				gf_list_insert(stream->AUs, tmp, i-1);
				return tmp;
			}
		}
	}
	GF_SAFEALLOC(tmp, GF_AUContext);
	if (!tmp) return NULL;
	tmp->commands = gf_list_new();
	if (isRap) tmp->flags = GF_SM_AU_RAP;
	tmp->timing = timing;
	tmp->timing_sec = time_sec;
	tmp->owner = stream;
	if (stream->disable_aggregation) tmp->flags |= GF_SM_AU_NOT_AGGREGATED;
	gf_list_add(stream->AUs, tmp);
	stream->imp_exp_time = tmp_timing;
	return tmp;
}
