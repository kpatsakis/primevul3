GF_Err split_isomedia_file(GF_ISOFile *mp4, Double split_dur, u64 split_size_kb, char *inName, Double InterleavingTime, Double chunk_start_time, Bool adjust_split_end, char *outName, const char *tmpdir)
{
	u32 i, count, nb_tk, needs_rap_sync, cur_file, conv_type, nb_tk_done, nb_samp, nb_done, di;
	Double max_dur, cur_file_time;
	Bool do_add, all_duplicatable, size_exceeded, chunk_extraction, rap_split, split_until_end;
	GF_ISOFile *dest;
	GF_ISOSample *samp;
	GF_Err e;
	TKInfo *tks, *tki;
	char *ext, szName[1000], szFile[1000];
	Double chunk_start = (Double) chunk_start_time;

	chunk_extraction = (chunk_start>=0) ? 1 : 0;
	split_until_end = 0;
	rap_split = 0;
	if (split_size_kb == (u64)-1) rap_split = 1;
	if (split_dur == -1) rap_split = 1;
	else if (split_dur <= -2) {
		split_size_kb = 0;
		split_until_end = 1;
	}

	if (rap_split) {
		split_size_kb = 0;
		split_dur = (double) GF_MAX_FLOAT;
	}


	ext = strrchr(inName, '/');
	if (!ext) ext = strrchr(inName, '\\');
	strcpy(szName, ext ? ext+1 : inName);
	ext = strrchr(szName, '.');
	if (ext) ext[0] = 0;
	ext = strrchr(inName, '.');

	dest = NULL;

	conv_type = 0;
	switch (gf_isom_guess_specification(mp4)) {
	case GF_ISOM_BRAND_ISMA:
		conv_type = 1;
		break;
	case GF_ISOM_BRAND_3GP4:
	case GF_ISOM_BRAND_3GP5:
	case GF_ISOM_BRAND_3GP6:
	case GF_ISOM_BRAND_3GG6:
	case GF_ISOM_BRAND_3G2A:
		conv_type = 2;
		break;
	}
	if (!stricmp(ext, ".3gp") || !stricmp(ext, ".3g2")) conv_type = 2;

	count = gf_isom_get_track_count(mp4);
	tks = (TKInfo *)gf_malloc(sizeof(TKInfo)*count);
	memset(tks, 0, sizeof(TKInfo)*count);

	e = GF_OK;
	max_dur = 0;
	nb_tk = 0;
	all_duplicatable = 1;
	needs_rap_sync = 0;
	nb_samp = 0;
	for (i=0; i<count; i++) {
		u32 mtype;
		Double dur;
		tks[nb_tk].tk = i+1;
		tks[nb_tk].can_duplicate = 0;

		mtype = gf_isom_get_media_type(mp4, i+1);
		switch (mtype) {
		/*we duplicate text samples at boundaries*/
		case GF_ISOM_MEDIA_TEXT:
		case GF_ISOM_MEDIA_SUBT:
		case GF_ISOM_MEDIA_MPEG_SUBT:
			tks[nb_tk].can_duplicate = 1;
		case GF_ISOM_MEDIA_AUDIO:
			break;
		case GF_ISOM_MEDIA_VISUAL:
        case GF_ISOM_MEDIA_AUXV:
        case GF_ISOM_MEDIA_PICT:
			if (gf_isom_get_sample_count(mp4, i+1)>1) {
				break;
			}
			continue;
		case GF_ISOM_MEDIA_HINT:
		case GF_ISOM_MEDIA_SCENE:
		case GF_ISOM_MEDIA_OCR:
		case GF_ISOM_MEDIA_OD:
		case GF_ISOM_MEDIA_OCI:
		case GF_ISOM_MEDIA_IPMP:
		case GF_ISOM_MEDIA_MPEGJ:
		case GF_ISOM_MEDIA_MPEG7:
		case GF_ISOM_MEDIA_FLASH:
			fprintf(stderr, "WARNING: Track ID %d (type %s) not handled by splitter - skipping\n", gf_isom_get_track_id(mp4, i+1), gf_4cc_to_str(mtype));
			continue;
		default:
			/*for all other track types, only split if more than one sample*/
			if (gf_isom_get_sample_count(mp4, i+1)==1) {
				fprintf(stderr, "WARNING: Track ID %d (type %s) not handled by splitter - skipping\n", gf_isom_get_track_id(mp4, i+1), gf_4cc_to_str(mtype));
				continue;
			}
			tks[nb_tk].can_duplicate = 1;
		}

		tks[nb_tk].sample_count = gf_isom_get_sample_count(mp4, i+1);
		nb_samp += tks[nb_tk].sample_count;
		tks[nb_tk].last_sample = 0;
		tks[nb_tk].firstDTS = 0;
		tks[nb_tk].time_scale = gf_isom_get_media_timescale(mp4, i+1);
		tks[nb_tk].has_non_raps = gf_isom_has_sync_points(mp4, i+1);
		/*seen that on some 3gp files from nokia ...*/
		if (mtype==GF_ISOM_MEDIA_AUDIO) tks[nb_tk].has_non_raps = 0;

		dur = (Double) (s64) gf_isom_get_media_duration(mp4, i+1);
		dur /= tks[nb_tk].time_scale;
		if (max_dur<dur) max_dur=dur;

		if (tks[nb_tk].has_non_raps) {
			/*we don't support that*/
			if (needs_rap_sync) {
				fprintf(stderr, "More than one track has non-sync points - cannot split file\n");
				gf_free(tks);
				return GF_NOT_SUPPORTED;
			}
			needs_rap_sync = nb_tk+1;
		}
		if (!tks[nb_tk].can_duplicate) all_duplicatable = 0;
		nb_tk++;
	}
	if (!nb_tk) {
		fprintf(stderr, "No suitable tracks found for splitting file\n");
		gf_free(tks);
		return GF_NOT_SUPPORTED;
	}
	if (chunk_start>=max_dur) {
		fprintf(stderr, "Input file (%f) shorter than requested split start offset (%f)\n", max_dur, chunk_start);
		gf_free(tks);
		return GF_NOT_SUPPORTED;
	}
	if (split_until_end) {
		if (split_dur < -2) {
			split_dur = - (split_dur + 2 - chunk_start);
			if (max_dur < split_dur) {
				fprintf(stderr, "Split duration till end %lf longer than track duration %lf\n", split_dur, max_dur);
				gf_free(tks);
				return GF_NOT_SUPPORTED;
			} else {
				split_dur = max_dur - split_dur;
			}
		} else {
			split_dur = max_dur;
		}
	} else if (!rap_split && (max_dur<=split_dur)) {
		fprintf(stderr, "Input file (%f) shorter than requested split duration (%f)\n", max_dur, split_dur);
		gf_free(tks);
		return GF_NOT_SUPPORTED;
	}
	if (needs_rap_sync) {
		Bool has_enough_sync = GF_FALSE;
		tki = &tks[needs_rap_sync-1];

		if (chunk_start == 0.0f)
			has_enough_sync = GF_TRUE;
		else if (gf_isom_get_sync_point_count(mp4, tki->tk) > 1)
			has_enough_sync = GF_TRUE;
		else if (gf_isom_get_sample_group_info(mp4, tki->tk, 1, GF_ISOM_SAMPLE_GROUP_RAP, NULL, NULL, NULL))
			has_enough_sync = GF_TRUE;
		else if (gf_isom_get_sample_group_info(mp4, tki->tk, 1, GF_ISOM_SAMPLE_GROUP_SYNC, NULL, NULL, NULL))
			has_enough_sync = GF_TRUE;

		if (!has_enough_sync) {
			fprintf(stderr, "Not enough Random Access points in input file - cannot split\n");
			gf_free(tks);
			return GF_NOT_SUPPORTED;
		}
	}
	split_size_kb *= 1024;
	cur_file_time = 0;

	if (chunk_start>0) {
		if (needs_rap_sync) {
			u32 sample_num;
			Double start;
			tki = &tks[needs_rap_sync-1];

			start = (Double) (s64) gf_isom_get_sample_dts(mp4, tki->tk, tki->sample_count);
			start /= tki->time_scale;
			if (start<chunk_start) {
				tki->stop_state = 2;
			} else  {
				e = gf_isom_get_sample_for_media_time(mp4, tki->tk, (u64) (chunk_start*tki->time_scale), &di, GF_ISOM_SEARCH_SYNC_BACKWARD, &samp, &sample_num);
				if (e!=GF_OK) {
					fprintf(stderr, "Cannot locate RAP in track ID %d for chunk extraction from %02.2f sec\n", gf_isom_get_track_id(mp4, tki->tk), chunk_start);
					gf_free(tks);
					return GF_NOT_SUPPORTED;
				}
				start = (Double) (s64) samp->DTS;
				start /= tki->time_scale;
				gf_isom_sample_del(&samp);
				fprintf(stderr, "Adjusting chunk start time to previous random access at %02.2f sec\n", start);
				split_dur += (chunk_start - start);
				chunk_start = start;
			}
		}
		/*sync all tracks*/
		for (i=0; i<nb_tk; i++) {
			tki = &tks[i];
			while (tki->last_sample<tki->sample_count) {
				Double time;
				u64 dts;
				dts = gf_isom_get_sample_dts(mp4, tki->tk, tki->last_sample+1);
				time = (Double) (s64) dts;
				time /= tki->time_scale;
				if (time>=chunk_start) {
					/*rewind one sample (text tracks & co)*/
					if (tki->can_duplicate && tki->last_sample) {
						tki->last_sample--;
						tki->firstDTS = (u64) (chunk_start*tki->time_scale);
					} else {
						tki->firstDTS = dts;
					}
					break;
				}
				tki->last_sample++;
			}
		}
		cur_file_time = chunk_start;
	} else {
		chunk_start = 0;
	}

	dest = NULL;
	nb_done = 0;
	nb_tk_done = 0;
	cur_file = 0;
	while (nb_tk_done<nb_tk) {
		Double last_rap_sample_time, max_dts, file_split_dur;
		Bool is_last_rap;
		Bool all_av_done = GF_FALSE;

		if (chunk_extraction) {
			sprintf(szFile, "%s_%d_%d%s", szName, (u32) chunk_start, (u32) (chunk_start+split_dur), ext);
			if (outName) strcpy(szFile, outName);
		} else {
			sprintf(szFile, "%s_%03d%s", szName, cur_file+1, ext);
			if (outName) {
				char *the_file = gf_url_concatenate(outName, szFile);
				if (the_file) {
					strcpy(szFile, the_file);
					gf_free(the_file);
				}
			}
		}
		dest = gf_isom_open(szFile, GF_ISOM_WRITE_EDIT, tmpdir);
		/*clone all tracks*/
		for (i=0; i<nb_tk; i++) {
			tki = &tks[i];
			/*track done - we remove the track from destination, an empty video track could cause pbs to some players*/
			if (tki->stop_state==2) continue;

			e = gf_isom_clone_track(mp4, tki->tk, dest, GF_FALSE, &tki->dst_tk);
			if (e) {
				fprintf(stderr, "Error cloning track %d\n", tki->tk);
				goto err_exit;
			}
			/*use non-packet CTS offsets (faster add/remove)*/
			if (gf_isom_has_time_offset(mp4, tki->tk)) {
				gf_isom_set_cts_packing(dest, tki->dst_tk, GF_TRUE);
			}
			gf_isom_remove_edit_segments(dest, tki->dst_tk);
		}
		do_add = 1;
		is_last_rap = 0;
		last_rap_sample_time = 0;
		file_split_dur = split_dur;

		size_exceeded = 0;
		max_dts = 0;
		while (do_add) {
			Bool is_rap;
			Double time;
			u32 nb_over, nb_av = 0;
			/*perfom basic de-interleaving to make sure we're not importing too much of a given track*/
			u32 nb_add = 0;
			/*add one sample of each track*/
			for (i=0; i<nb_tk; i++) {
				Double t;
				u64 dts;
				tki = &tks[i];

				if (!tki->can_duplicate) nb_av++;

				if (tki->stop_state)
					continue;
				if (tki->last_sample==tki->sample_count)
					continue;

				/*get sample info, see if we need to check it (basic de-interleaver)*/
				dts = gf_isom_get_sample_dts(mp4, tki->tk, tki->last_sample+1);

				/*reinsertion (timed text)*/
				if (dts < tki->firstDTS) {
					samp = gf_isom_get_sample(mp4, tki->tk, tki->last_sample+1, &di);
					samp->DTS = 0;
					e = gf_isom_add_sample(dest, tki->dst_tk, di, samp);
					if (!e) {
						e = gf_isom_copy_sample_info(dest, tki->dst_tk, mp4, tki->tk, tki->last_sample+1);
					}

					gf_isom_sample_del(&samp);
					tki->last_sample += 1;
					dts = gf_isom_get_sample_dts(mp4, tki->tk, tki->last_sample+1);
				}
				dts -= tki->firstDTS;


				t = (Double) (s64) dts;
				t /= tki->time_scale;
				if (tki->first_sample_done) {
					if (!all_av_done && (t>max_dts)) continue;
				} else {
					/*here's the trick: only take care of a/v media for splitting, and add other media
					only if thir dts is less than the max AV dts found. Otherwise with some text streams we will end up importing
					too much video and corrupting the last sync point indication*/
					if (!tki->can_duplicate && (t>max_dts)) max_dts = t;
					tki->first_sample_done = 1;
				}
				samp = gf_isom_get_sample(mp4, tki->tk, tki->last_sample+1, &di);
				samp->DTS -= tki->firstDTS;

				nb_add += 1;

				is_rap = GF_FALSE;
				if (samp->IsRAP) {
					is_rap = GF_TRUE;
				} else {
					Bool has_roll;
					gf_isom_get_sample_rap_roll_info(mp4, tki->tk, tki->last_sample+1, &is_rap, &has_roll, NULL);
				}


				if (tki->has_non_raps && is_rap) {
					GF_ISOSample *next_rap;
					u32 next_rap_num, sdi;
					last_rap_sample_time = (Double) (s64) samp->DTS;
					last_rap_sample_time /= tki->time_scale;
					e = gf_isom_get_sample_for_media_time(mp4, tki->tk, samp->DTS+tki->firstDTS+2, &sdi, GF_ISOM_SEARCH_SYNC_FORWARD, &next_rap, &next_rap_num);
					if (e==GF_EOS)
						is_last_rap = 1;
					if (next_rap) {
						if (!next_rap->IsRAP)
							is_last_rap = 1;
						gf_isom_sample_del(&next_rap);
					}
				}
				tki->lastDTS = samp->DTS;
				e = gf_isom_add_sample(dest, tki->dst_tk, di, samp);
				gf_isom_sample_del(&samp);

				if (!e) {
					e = gf_isom_copy_sample_info(dest, tki->dst_tk, mp4, tki->tk, tki->last_sample+1);
				}
				tki->last_sample += 1;
				gf_set_progress("Splitting", nb_done, nb_samp);
				nb_done++;
				if (e) {
					fprintf(stderr, "Error cloning track %d sample %d\n", tki->tk, tki->last_sample);
					goto err_exit;
				}

				tki->next_sample_is_rap = 0;
				if (rap_split && tki->has_non_raps) {
					if ( gf_isom_get_sample_sync(mp4, tki->tk, tki->last_sample+1))
						tki->next_sample_is_rap = 1;
				}
			}

			/*test by size/duration*/
			nb_over = 0;

			/*test by file size: same as duration test, only dynamically increment import duration*/
			if (split_size_kb) {
				u64 est_size = gf_isom_estimate_size(dest);
				/*while below desired size keep importing*/
				if (est_size<split_size_kb)
					file_split_dur = (Double) GF_MAX_FLOAT;
				else {
					size_exceeded = 1;
				}
			}

			for (i=0; i<nb_tk; i++) {
				tki = &tks[i];
				if (tki->stop_state) {
					nb_over++;
					if (!tki->can_duplicate && (tki->last_sample==tki->sample_count) )
						nb_av--;
					continue;
				}
				time = (Double) (s64) tki->lastDTS;
				time /= tki->time_scale;
				if (size_exceeded
				        || (tki->last_sample==tki->sample_count)
				        || (!tki->can_duplicate && (time>file_split_dur))
				        || (rap_split && tki->has_non_raps && tki->next_sample_is_rap)
				   ) {
					nb_over++;
					tki->stop_state = 1;
					if (tki->last_sample<tki->sample_count)
						is_last_rap = 0;
					else if (tki->first_sample_done)
						is_last_rap = 0;

					if (rap_split && tki->next_sample_is_rap) {
						file_split_dur = (Double) ( gf_isom_get_sample_dts(mp4, tki->tk, tki->last_sample+1) - tki->firstDTS);
						file_split_dur /= tki->time_scale;
					}
				}
				/*special tracks (not audio, not video)*/
				else if (tki->can_duplicate) {
					u64 dts = gf_isom_get_sample_dts(mp4, tki->tk, tki->last_sample+1);
					time = (Double) (s64) (dts - tki->firstDTS);
					time /= tki->time_scale;
					if (time>file_split_dur) {
						nb_over++;
						tki->stop_state = 1;
					}
				}
				if (!nb_add && (!max_dts || (tki->lastDTS <= 1 + (u64) (tki->time_scale*max_dts) )))
					tki->first_sample_done = 0;
			}
			if (nb_over==nb_tk) do_add = 0;

			if (!nb_av)
				all_av_done = GF_TRUE;
		}

		/*remove samples - first figure out smallest duration*/
		file_split_dur = (Double) GF_MAX_FLOAT;
		for (i=0; i<nb_tk; i++) {
			Double time;
			tki = &tks[i];
			/*track done*/
			if ((tki->stop_state==2) || (!is_last_rap && (tki->sample_count == tki->last_sample)) ) {
				if (tki->has_non_raps) last_rap_sample_time = 0;
				time = (Double) (s64) ( gf_isom_get_sample_dts(mp4, tki->tk, tki->last_sample+1) - tki->firstDTS);
				time /= tki->time_scale;
				if (file_split_dur==(Double)GF_MAX_FLOAT || file_split_dur<time) file_split_dur = time;
				continue;
			}

			{
				time = (Double) (s64) ( gf_isom_get_sample_dts(mp4, tki->tk, tki->last_sample+1) - tki->firstDTS);
				time /= tki->time_scale;
				if ((!tki->can_duplicate || all_duplicatable) && time<file_split_dur) file_split_dur = time;
				else if (rap_split && tki->next_sample_is_rap) file_split_dur = time;
			}
		}
		if (file_split_dur == (Double) GF_MAX_FLOAT) {
			fprintf(stderr, "Cannot split file (duration too small or size too small)\n");
			goto err_exit;
		}
		if (chunk_extraction) {
			if (adjust_split_end) {
				fprintf(stderr, "Adjusting chunk end time to previous random access at %02.2f sec\n", chunk_start + last_rap_sample_time);
				file_split_dur = last_rap_sample_time;
				if (outName) strcpy(szFile, outName);
				else sprintf(szFile, "%s_%d_%d%s", szName, (u32) chunk_start, (u32) (chunk_start+file_split_dur), ext);
				gf_isom_set_final_name(dest, szFile);
			}
			else file_split_dur = split_dur;
		}

		/*don't split if eq to copy...*/
		if (is_last_rap && !cur_file && !chunk_start) {
			fprintf(stderr, "Cannot split file (Not enough sync samples, duration too large or size too big)\n");
			goto err_exit;
		}


		/*if not last chunk and longer duration adjust to previous RAP point*/
		if ( (size_exceeded || !split_size_kb) && (file_split_dur>split_dur) && !chunk_start) {
			/*if larger than last RAP, rewind till it*/
			if (last_rap_sample_time && (last_rap_sample_time<file_split_dur) ) {
				file_split_dur = last_rap_sample_time;
				is_last_rap = 0;
			}
		}

		nb_tk_done = 0;
		if (!is_last_rap || chunk_extraction) {
			for (i=0; i<nb_tk; i++) {
				Double time = 0;
				u32 last_samp;
				tki = &tks[i];
				while (1) {
					last_samp = gf_isom_get_sample_count(dest, tki->dst_tk);

					time = (Double) (s64) gf_isom_get_media_duration(dest, tki->dst_tk);
					time -= (Double) (s64) gf_isom_get_sample_duration(dest, tki->dst_tk, tki->last_sample) / 4;
					time /= tki->time_scale;

					if (last_samp<=1) break;

					/*done*/
					if (tki->last_sample==tki->sample_count) {
						if (!chunk_extraction && !tki->can_duplicate) {
							tki->stop_state=2;
							break;
						}
					}

					if (time <= file_split_dur) break;

					gf_isom_remove_sample(dest, tki->dst_tk, last_samp);
					tki->last_sample--;
					assert(tki->last_sample);
					nb_done--;
					gf_set_progress("Splitting", nb_done, nb_samp);
				}
				if (tki->last_sample<tki->sample_count) {
					u64 dts;
					tki->stop_state = 0;
					dts = gf_isom_get_sample_dts(mp4, tki->tk, tki->last_sample+1);
					time = (Double) (s64) (dts - tki->firstDTS);
					time /= tki->time_scale;
					/*re-insert prev sample*/
					if (tki->can_duplicate && (time>file_split_dur) ) {
						Bool was_insert = GF_FALSE;
						tki->last_sample--;
						dts = gf_isom_get_sample_dts(mp4, tki->tk, tki->last_sample+1);
						if (dts < tki->firstDTS) was_insert = GF_TRUE;
						tki->firstDTS += (u64) (file_split_dur*tki->time_scale);
						if (was_insert) {
							gf_isom_set_last_sample_duration(dest, tki->dst_tk, (u32) (file_split_dur*tki->time_scale));
						} else {
							gf_isom_set_last_sample_duration(dest, tki->dst_tk, (u32) (tki->firstDTS - dts) );
						}
					} else {
						tki->firstDTS = dts;
					}
					tki->first_sample_done = 0;
				} else {
					nb_tk_done++;
				}

			}
		}

		if (chunk_extraction) {
			fprintf(stderr, "Extracting chunk %s - duration %02.2fs (%02.2fs->%02.2fs)\n", szFile, file_split_dur, chunk_start, (chunk_start+split_dur));
		} else {
			fprintf(stderr, "Storing split-file %s - duration %02.2f seconds\n", szFile, file_split_dur);
		}

		/*repack CTSs*/
		for (i=0; i<nb_tk; i++) {
			u32 j;
			u64 new_track_dur;
			tki = &tks[i];
			if (tki->stop_state == 2) continue;
			if (!gf_isom_get_sample_count(dest, tki->dst_tk)) {
				gf_isom_remove_track(dest, tki->dst_tk);
				continue;
			}
			if (gf_isom_has_time_offset(mp4, tki->tk)) {
				gf_isom_set_cts_packing(dest, tki->dst_tk, GF_FALSE);
			}
			if (is_last_rap && tki->can_duplicate) {
				gf_isom_set_last_sample_duration(dest, tki->dst_tk, gf_isom_get_sample_duration(mp4, tki->tk, tki->sample_count));
			}

			/*rewrite edit list*/
			new_track_dur = gf_isom_get_track_duration(dest, tki->dst_tk);
			count = gf_isom_get_edit_segment_count(mp4, tki->tk);
			if (count>2) {
				fprintf(stderr, "Warning: %d edit segments - not supported while splitting (max 2) - ignoring extra\n", count);
				count=2;
			}
			for (j=0; j<count; j++) {
				u64 editTime, segDur, MediaTime;
				u8 mode;

				gf_isom_get_edit_segment(mp4, tki->tk, j+1, &editTime, &segDur, &MediaTime, &mode);
				if (!j && (mode!=GF_ISOM_EDIT_EMPTY) ) {
					fprintf(stderr, "Warning: Edit list doesn't look like a track delay scheme - ignoring\n");
					break;
				}
				if (mode==GF_ISOM_EDIT_NORMAL) {
					segDur = new_track_dur;
				}
				gf_isom_set_edit_segment(dest, tki->dst_tk, editTime, segDur, MediaTime, mode);
			}
		}
		/*check chapters*/
		do_add = 1;
		for (i=0; i<gf_isom_get_chapter_count(mp4, 0); i++) {
			char *name;
			u64 chap_time;
			gf_isom_get_chapter(mp4, 0, i+1, &chap_time, (const char **) &name);
			max_dts = (Double) (s64) chap_time;
			max_dts /= 1000;
			if (max_dts<cur_file_time) continue;
			if (max_dts>cur_file_time+file_split_dur) break;
			max_dts-=cur_file_time;
			chap_time = (u64) (max_dts*1000);
			gf_isom_add_chapter(dest, 0, chap_time, name);
			/*add prev*/
			if (do_add && i) {
				gf_isom_get_chapter(mp4, 0, i, &chap_time, (const char **) &name);
				gf_isom_add_chapter(dest, 0, 0, name);
				do_add = 0;
			}
		}
		cur_file_time += file_split_dur;

		if (conv_type==1) gf_media_make_isma(dest, 1, 0, 0);
		else if (conv_type==2) gf_media_make_3gpp(dest);
		if (InterleavingTime) {
			gf_isom_make_interleave(dest, InterleavingTime);
		} else {
			gf_isom_set_storage_mode(dest, GF_ISOM_STORE_STREAMABLE);
		}

		gf_isom_clone_pl_indications(mp4, dest);
		e = gf_isom_close(dest);
		dest = NULL;
		if (e) fprintf(stderr, "Error storing file %s\n", gf_error_to_string(e));
		if (is_last_rap || chunk_extraction) break;
		cur_file++;
	}
	gf_set_progress("Splitting", nb_samp, nb_samp);
err_exit:
	if (dest) gf_isom_delete(dest);
	gf_free(tks);
	return e;
}
