static void set_chapter_track(GF_ISOFile *file, u32 track, u32 chapter_ref_trak)
{
	u64 ref_duration, chap_duration;
	Double scale;

	gf_isom_set_track_reference(file, chapter_ref_trak, GF_ISOM_REF_CHAP, gf_isom_get_track_id(file, track) );
	gf_isom_set_track_enabled(file, track, 0);

	ref_duration = gf_isom_get_media_duration(file, chapter_ref_trak);
	chap_duration = gf_isom_get_media_duration(file, track);
	scale = (Double) (s64) gf_isom_get_media_timescale(file, track);
	scale /= gf_isom_get_media_timescale(file, chapter_ref_trak);
	ref_duration = (u64) (ref_duration * scale);

	if (chap_duration < ref_duration) {
		chap_duration -= gf_isom_get_sample_duration(file, track, gf_isom_get_sample_count(file, track));
		chap_duration = ref_duration - chap_duration;
		gf_isom_set_last_sample_duration(file, track, (u32) chap_duration);
	}
}
