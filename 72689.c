GF_Err cat_isomedia_file(GF_ISOFile *dest, char *fileName, u32 import_flags, Double force_fps, u32 frames_per_sample, char *tmp_dir, Bool force_cat, Bool align_timelines, Bool allow_add_in_command)
{
	u32 i, j, count, nb_tracks, nb_samp, nb_done;
	GF_ISOFile *orig;
	GF_Err e;
	char *opts, *multi_cat;
	Double ts_scale;
	Double dest_orig_dur;
	u32 dst_tk, tk_id, mtype;
	u64 insert_dts;
	Bool is_isom;
	GF_ISOSample *samp;
	Double aligned_to_DTS = 0;

	if (strchr(fileName, '*')) return cat_multiple_files(dest, fileName, import_flags, force_fps, frames_per_sample, tmp_dir, force_cat, align_timelines, allow_add_in_command);

	multi_cat = allow_add_in_command ? strchr(fileName, '+') : NULL;
	if (multi_cat) {
		multi_cat[0] = 0;
		multi_cat = &multi_cat[1];
	}
	opts = strchr(fileName, ':');
	if (opts && (opts[1]=='\\'))
		opts = strchr(fileName, ':');

	e = GF_OK;

	/*if options are specified, reimport the file*/
	is_isom = opts ? 0 : gf_isom_probe_file(fileName);

	if (!is_isom || opts) {
		orig = gf_isom_open("temp", GF_ISOM_WRITE_EDIT, tmp_dir);
		e = import_file(orig, fileName, import_flags, force_fps, frames_per_sample);
		if (e) return e;
	} else {
		/*we open the original file in edit mode since we may have to rewrite AVC samples*/
		orig = gf_isom_open(fileName, GF_ISOM_OPEN_EDIT, tmp_dir);
	}

	while (multi_cat) {
		char *sep = strchr(multi_cat, '+');
		if (sep) sep[0] = 0;

		e = import_file(orig, multi_cat, import_flags, force_fps, frames_per_sample);
		if (e) {
			gf_isom_delete(orig);
			return e;
		}
		if (!sep) break;
		sep[0]=':';
		multi_cat = sep+1;
	}

	nb_samp = 0;
	nb_tracks = gf_isom_get_track_count(orig);
	for (i=0; i<nb_tracks; i++) {
		u32 mtype = gf_isom_get_media_type(orig, i+1);
		switch (mtype) {
		case GF_ISOM_MEDIA_HINT:
		case GF_ISOM_MEDIA_OD:
		case GF_ISOM_MEDIA_FLASH:
			fprintf(stderr, "WARNING: Track ID %d (type %s) not handled by concatenation - removing from destination\n", gf_isom_get_track_id(orig, i+1), gf_4cc_to_str(mtype));
			continue;
		case GF_ISOM_MEDIA_AUDIO:
		case GF_ISOM_MEDIA_TEXT:
		case GF_ISOM_MEDIA_SUBT:
		case GF_ISOM_MEDIA_MPEG_SUBT:
		case GF_ISOM_MEDIA_VISUAL:
        case GF_ISOM_MEDIA_AUXV:
        case GF_ISOM_MEDIA_PICT:
		case GF_ISOM_MEDIA_SCENE:
		case GF_ISOM_MEDIA_OCR:
		case GF_ISOM_MEDIA_OCI:
		case GF_ISOM_MEDIA_IPMP:
		case GF_ISOM_MEDIA_MPEGJ:
		case GF_ISOM_MEDIA_MPEG7:
		default:
			/*only cat self-contained files*/
			if (gf_isom_is_self_contained(orig, i+1, 1)) {
				nb_samp+= gf_isom_get_sample_count(orig, i+1);
				break;
			}
			break;
		}
	}
	if (!nb_samp) {
		fprintf(stderr, "No suitable media tracks to cat in %s - skipping\n", fileName);
		goto err_exit;
	}

	dest_orig_dur = (Double) (s64) gf_isom_get_duration(dest);
	if (!gf_isom_get_timescale(dest)) {
		gf_isom_set_timescale(dest, gf_isom_get_timescale(orig));
	}
	dest_orig_dur /= gf_isom_get_timescale(dest);

	aligned_to_DTS = 0;
	for (i=0; i<gf_isom_get_track_count(dest); i++) {
		Double track_dur = (Double) gf_isom_get_media_duration(dest, i+1);
		track_dur /= gf_isom_get_media_timescale(dest, i+1);
		if (aligned_to_DTS < track_dur) {
			aligned_to_DTS = track_dur;
		}
	}

	fprintf(stderr, "Appending file %s\n", fileName);
	nb_done = 0;
	for (i=0; i<nb_tracks; i++) {
		u64 last_DTS, dest_track_dur_before_cat;
		u32 nb_edits = 0;
		Bool skip_lang_test = 1;
		Bool use_ts_dur = 1;
		Bool merge_edits = 0;
		Bool new_track = 0;
		mtype = gf_isom_get_media_type(orig, i+1);
		switch (mtype) {
		case GF_ISOM_MEDIA_HINT:
		case GF_ISOM_MEDIA_OD:
		case GF_ISOM_MEDIA_FLASH:
			continue;
		case GF_ISOM_MEDIA_TEXT:
		case GF_ISOM_MEDIA_SUBT:
		case GF_ISOM_MEDIA_MPEG_SUBT:
		case GF_ISOM_MEDIA_SCENE:
			use_ts_dur = 0;
		case GF_ISOM_MEDIA_AUDIO:
		case GF_ISOM_MEDIA_VISUAL:
        case GF_ISOM_MEDIA_PICT:
		case GF_ISOM_MEDIA_OCR:
		case GF_ISOM_MEDIA_OCI:
		case GF_ISOM_MEDIA_IPMP:
		case GF_ISOM_MEDIA_MPEGJ:
		case GF_ISOM_MEDIA_MPEG7:
		default:
			if (!gf_isom_is_self_contained(orig, i+1, 1)) continue;
			break;
		}

		dst_tk = 0;
		/*if we had a temporary import of the file, check if the original track ID matches the dst one. If so, skip all language detection code*/
		tk_id = gf_isom_get_track_original_id(orig, i+1);
		if (!tk_id) {
			tk_id = gf_isom_get_track_id(orig, i+1);
			skip_lang_test = 0;
		}
		dst_tk = gf_isom_get_track_by_id(dest, tk_id);


		if (dst_tk) {
			if (mtype != gf_isom_get_media_type(dest, dst_tk))
				dst_tk = 0;
			else if (gf_isom_get_media_subtype(dest, dst_tk, 1) != gf_isom_get_media_subtype(orig, i+1, 1))
				dst_tk = 0;
		}

		if (!dst_tk) {
			for (j=0; j<gf_isom_get_track_count(dest); j++) {
				if (mtype != gf_isom_get_media_type(dest, j+1)) continue;
				if (gf_isom_is_same_sample_description(orig, i+1, 0, dest, j+1, 0)) {
					if (gf_isom_is_video_subtype(mtype) ) {
						u32 w, h, ow, oh;
						gf_isom_get_visual_info(orig, i+1, 1, &ow, &oh);
						gf_isom_get_visual_info(dest, j+1, 1, &w, &h);
						if ((ow==w) && (oh==h)) {
							dst_tk = j+1;
							break;
						}
					}
					/*check language code*/
					else if (!skip_lang_test && (mtype==GF_ISOM_MEDIA_AUDIO)) {
						u32 lang_src, lang_dst;
						char *lang = NULL;
						gf_isom_get_media_language(orig, i+1, &lang);
						if (lang) {
							lang_src = GF_4CC(lang[0], lang[1], lang[2], lang[3]);
							gf_free(lang);
						} else {
							lang_src = 0;
						}
						gf_isom_get_media_language(dest, j+1, &lang);
						if (lang) {
							lang_dst = GF_4CC(lang[0], lang[1], lang[2], lang[3]);
							gf_free(lang);
						} else {
							lang_dst = 0;
						}
						if (lang_dst==lang_src) {
							dst_tk = j+1;
							break;
						}
					} else {
						dst_tk = j+1;
						break;
					}
				}
			}
		}

		if (dst_tk) {
			u32 found_dst_tk = dst_tk;
			u32 stype = gf_isom_get_media_subtype(dest, dst_tk, 1);
			/*we MUST have the same codec*/
			if (gf_isom_get_media_subtype(orig, i+1, 1) != stype) dst_tk = 0;
			/*we only support cat with the same number of sample descriptions*/
			if (gf_isom_get_sample_description_count(orig, i+1) != gf_isom_get_sample_description_count(dest, dst_tk)) dst_tk = 0;
			/*if not forcing cat, check the media codec config is the same*/
			if (!gf_isom_is_same_sample_description(orig, i+1, 0, dest, dst_tk, 0)) {
				dst_tk = 0;
			}
			/*we force the same visual resolution*/
			else if (gf_isom_is_video_subtype(mtype) ) {
				u32 w, h, ow, oh;
				gf_isom_get_visual_info(orig, i+1, 1, &ow, &oh);
				gf_isom_get_visual_info(dest, dst_tk, 1, &w, &h);
				if ((ow!=w) || (oh!=h)) {
					dst_tk = 0;
				}
			}

			if (!dst_tk) {
				/*merge AVC config if possible*/
				if ((stype == GF_ISOM_SUBTYPE_AVC_H264)
				        || (stype == GF_ISOM_SUBTYPE_AVC2_H264)
				        || (stype == GF_ISOM_SUBTYPE_AVC3_H264)
				        || (stype == GF_ISOM_SUBTYPE_AVC4_H264) ) {
					dst_tk = merge_avc_config(dest, tk_id, orig, i+1, force_cat);
				}
#ifndef GPAC_DISABLE_HEVC
				/*merge HEVC config if possible*/
				else if ((stype == GF_ISOM_SUBTYPE_HVC1)
				         || (stype == GF_ISOM_SUBTYPE_HEV1)
				         || (stype == GF_ISOM_SUBTYPE_HVC2)
				         || (stype == GF_ISOM_SUBTYPE_HEV2)) {
					dst_tk = merge_hevc_config(dest, tk_id, orig, i+1, force_cat);
				}
#endif /*GPAC_DISABLE_HEVC*/
				else if (force_cat) {
					dst_tk = found_dst_tk;
				}
			}
		}

		/*looks like a new track*/
		if (!dst_tk) {
			fprintf(stderr, "No suitable destination track found - creating new one (type %s)\n", gf_4cc_to_str(mtype));
			e = gf_isom_clone_track(orig, i+1, dest, GF_FALSE, &dst_tk);
			if (e) goto err_exit;
			gf_isom_clone_pl_indications(orig, dest);
			new_track = 1;

			if (align_timelines) {
				u32 max_timescale = 0;
				u32 idx;
				for (idx=0; idx<nb_tracks; idx++) {
					if (max_timescale < gf_isom_get_media_timescale(orig, idx+1))
						max_timescale = gf_isom_get_media_timescale(orig, idx+1);
				}
#if 0
				if (dst_timescale < max_timescale) {
					dst_timescale = gf_isom_get_media_timescale(dest, dst_tk);
					idx = max_timescale / dst_timescale;
					if (dst_timescale * idx < max_timescale) idx ++;
					dst_timescale *= idx;

					gf_isom_set_media_timescale(dest, dst_tk, max_timescale, 0);
				}
#else
				gf_isom_set_media_timescale(dest, dst_tk, max_timescale, 0);
#endif
			}

			/*remove cloned edit list, as it will be rewritten after import*/
			gf_isom_remove_edit_segments(dest, dst_tk);
		} else {
			nb_edits = gf_isom_get_edit_segment_count(orig, i+1);
		}

		dest_track_dur_before_cat = gf_isom_get_media_duration(dest, dst_tk);
		count = gf_isom_get_sample_count(dest, dst_tk);

		if (align_timelines) {
			insert_dts = (u64) (aligned_to_DTS * gf_isom_get_media_timescale(dest, dst_tk));
		} else if (use_ts_dur && (count>1)) {
			insert_dts = 2*gf_isom_get_sample_dts(dest, dst_tk, count) - gf_isom_get_sample_dts(dest, dst_tk, count-1);
		} else {
			insert_dts = dest_track_dur_before_cat;
			if (!count) insert_dts = 0;
		}

		ts_scale = gf_isom_get_media_timescale(dest, dst_tk);
		ts_scale /= gf_isom_get_media_timescale(orig, i+1);

		/*if not a new track, see if we can merge the edit list - this is a crude test that only checks
		we have the same edit types*/
		if (nb_edits && (nb_edits == gf_isom_get_edit_segment_count(dest, dst_tk)) ) {
			u64 editTime, segmentDuration, mediaTime, dst_editTime, dst_segmentDuration, dst_mediaTime;
			u8 dst_editMode, editMode;
			u32 j;
			merge_edits = 1;
			for (j=0; j<nb_edits; j++) {
				gf_isom_get_edit_segment(orig, i+1, j+1, &editTime, &segmentDuration, &mediaTime, &editMode);
				gf_isom_get_edit_segment(dest, dst_tk, j+1, &dst_editTime, &dst_segmentDuration, &dst_mediaTime, &dst_editMode);

				if (dst_editMode!=editMode) {
					merge_edits=0;
					break;
				}
			}
		}

		last_DTS = 0;
		count = gf_isom_get_sample_count(orig, i+1);
		for (j=0; j<count; j++) {
			u32 di;
			samp = gf_isom_get_sample(orig, i+1, j+1, &di);
			last_DTS = samp->DTS;
			samp->DTS =  (u64) (ts_scale * samp->DTS + (new_track ? 0 : insert_dts));
			samp->CTS_Offset =  (u32) (samp->CTS_Offset * ts_scale);

			if (gf_isom_is_self_contained(orig, i+1, di)) {
				e = gf_isom_add_sample(dest, dst_tk, di, samp);
			} else {
				u64 offset;
				GF_ISOSample *s = gf_isom_get_sample_info(orig, i+1, j+1, &di, &offset);
				e = gf_isom_add_sample_reference(dest, dst_tk, di, samp, offset);
				gf_isom_sample_del(&s);
			}
			gf_isom_sample_del(&samp);
			if (e) goto err_exit;

			e = gf_isom_copy_sample_info(dest, dst_tk, orig, i+1, j+1);
			if (e) goto err_exit;

			gf_set_progress("Appending", nb_done, nb_samp);
			nb_done++;
		}
		/*scene description and text: compute last sample duration based on original media duration*/
		if (!use_ts_dur) {
			insert_dts = gf_isom_get_media_duration(orig, i+1) - last_DTS;
			gf_isom_set_last_sample_duration(dest, dst_tk, (u32) insert_dts);
		}

		if (new_track && insert_dts) {
			u64 media_dur = gf_isom_get_media_duration(orig, i+1);
			/*convert from media time to track time*/
			Double rescale = (Float) gf_isom_get_timescale(dest);
			rescale /= (Float) gf_isom_get_media_timescale(dest, dst_tk);
			/*convert from orig to dst time scale*/
			rescale *= ts_scale;

			gf_isom_set_edit_segment(dest, dst_tk, 0, (u64) (s64) (insert_dts*rescale), 0, GF_ISOM_EDIT_EMPTY);
			gf_isom_set_edit_segment(dest, dst_tk, (u64) (s64) (insert_dts*rescale), (u64) (s64) (media_dur*rescale), 0, GF_ISOM_EDIT_NORMAL);
		} else if (merge_edits) {
			/*convert from media time to track time*/
			Double rescale = (Float) gf_isom_get_timescale(dest);
			rescale /= (Float) gf_isom_get_media_timescale(dest, dst_tk);
			/*convert from orig to dst time scale*/
			rescale *= ts_scale;

			/*get the first edit normal mode and add the new track dur*/
			for (j=nb_edits; j>0; j--) {
				u64 editTime, segmentDuration, mediaTime;
				u8 editMode;
				gf_isom_get_edit_segment(dest, dst_tk, j, &editTime, &segmentDuration, &mediaTime, &editMode);

				if (editMode==GF_ISOM_EDIT_NORMAL) {
					Double prev_dur = (Double) (s64) dest_track_dur_before_cat;
					Double dur = (Double) (s64) gf_isom_get_media_duration(orig, i+1);

					dur *= rescale;
					prev_dur *= rescale;

					/*safety test: some files have broken edit lists. If no more than 2 entries, check that the segment duration
					is less or equal to the movie duration*/
					if (prev_dur < segmentDuration) {
						fprintf(stderr, "Warning: suspicious edit list entry found: duration %g sec but longest track duration before cat is %g - fixing it\n", (Double) (s64) segmentDuration/1000.0, prev_dur/1000);
						segmentDuration = (u64) (s64) ( (Double) (s64) (dest_track_dur_before_cat - mediaTime) * rescale );
					}

					segmentDuration += (u64) (s64) dur;
					gf_isom_modify_edit_segment(dest, dst_tk, j, segmentDuration, mediaTime, editMode);
					break;
				}
			}
		} else {
			u64 editTime, segmentDuration, mediaTime, edit_offset;
			Double t;
			u8 editMode;
			u32 j, count;

			count = gf_isom_get_edit_segment_count(dest, dst_tk);
			if (count) {
				e = gf_isom_get_edit_segment(dest, dst_tk, count, &editTime, &segmentDuration, &mediaTime, &editMode);
				if (e) {
					fprintf(stderr, "Error: edit segment error on destination track %u could not be retrieved.\n", dst_tk);
					goto err_exit;
				}
			} else if (gf_isom_get_edit_segment_count(orig, i+1)) {
				/*fake empty edit segment*/
				/*convert from media time to track time*/
				Double rescale = (Float) gf_isom_get_timescale(dest);
				rescale /= (Float) gf_isom_get_media_timescale(dest, dst_tk);
				segmentDuration = (u64) (dest_track_dur_before_cat * rescale);
				editTime = 0;
				mediaTime = 0;
				gf_isom_set_edit_segment(dest, dst_tk, editTime, segmentDuration, mediaTime, GF_ISOM_EDIT_NORMAL);
			} else {
				editTime = 0;
				segmentDuration = 0;
			}

			/*convert to dst time scale*/
			ts_scale = (Float) gf_isom_get_timescale(dest);
			ts_scale /= (Float) gf_isom_get_timescale(orig);

			edit_offset = editTime + segmentDuration;
			count = gf_isom_get_edit_segment_count(orig, i+1);
			for (j=0; j<count; j++) {
				gf_isom_get_edit_segment(orig, i+1, j+1, &editTime, &segmentDuration, &mediaTime, &editMode);
				t = (Double) (s64) editTime;
				t *= ts_scale;
				t += (s64) edit_offset;
				editTime = (s64) t;
				t = (Double) (s64) segmentDuration;
				t *= ts_scale;
				segmentDuration = (s64) t;
				t = (Double) (s64) mediaTime;
				t *= ts_scale;
				t+= (s64) dest_track_dur_before_cat;
				mediaTime = (s64) t;
				if ((editMode == GF_ISOM_EDIT_EMPTY) && (mediaTime > 0)) {
					editMode = GF_ISOM_EDIT_NORMAL;
				}
				gf_isom_set_edit_segment(dest, dst_tk, editTime, segmentDuration, mediaTime, editMode);
			}
		}

	}
	gf_set_progress("Appending", nb_samp, nb_samp);

	/*check chapters*/
	for (i=0; i<gf_isom_get_chapter_count(orig, 0); i++) {
		char *name;
		Double c_time;
		u64 chap_time;
		gf_isom_get_chapter(orig, 0, i+1, &chap_time, (const char **) &name);
		c_time = (Double) (s64) chap_time;
		c_time /= 1000;
		c_time += dest_orig_dur;

		/*check last file chapter*/
		if (!i && gf_isom_get_chapter_count(dest, 0)) {
			const char *last_name;
			u64 last_chap_time;
			gf_isom_get_chapter(dest, 0, gf_isom_get_chapter_count(dest, 0), &last_chap_time, &last_name);
			/*last and first chapters are the same, don't duplicate*/
			if (last_name && name && !stricmp(last_name, name)) continue;
		}

		chap_time = (u64) (c_time*1000);
		gf_isom_add_chapter(dest, 0, chap_time, name);
	}


err_exit:
	gf_isom_delete(orig);
	return e;
}
