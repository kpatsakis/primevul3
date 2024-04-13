GF_Err import_file(GF_ISOFile *dest, char *inName, u32 import_flags, Double force_fps, u32 frames_per_sample)
{
	u32 track_id, i, j, timescale, track, stype, profile, level, new_timescale, rescale, svc_mode, txt_flags, split_tile_mode, temporal_mode;
	s32 par_d, par_n, prog_id, delay, force_rate;
	s32 tw, th, tx, ty, txtw, txth, txtx, txty;
	Bool do_audio, do_video, do_auxv,do_pict, do_all, disable, track_layout, text_layout, chap_ref, is_chap, is_chap_file, keep_handler, negative_cts_offset, rap_only, refs_only;
	u32 group, handler, rvc_predefined, check_track_for_svc, check_track_for_lhvc, check_track_for_hevc;
	const char *szLan;
	GF_Err e;
	GF_MediaImporter import;
	char *ext, szName[1000], *handler_name, *rvc_config, *chapter_name;
	GF_List *kinds;
	GF_TextFlagsMode txt_mode = GF_ISOM_TEXT_FLAGS_OVERWRITE;
	u8 max_layer_id_plus_one, max_temporal_id_plus_one;

	rvc_predefined = 0;
	chapter_name = NULL;
	new_timescale = 1;
	rescale = 0;
	text_layout = 0;
	/*0: merge all
	  1: split base and all SVC in two tracks
	  2: split all base and SVC layers in dedicated tracks
	 */
	svc_mode = 0;

	memset(&import, 0, sizeof(GF_MediaImporter));

	strcpy(szName, inName);
#ifdef WIN32
	/*dirty hack for msys&mingw: when we use import options, the ':' separator used prevents msys from translating the path
	we do this for regular cases where the path starts with the drive letter. If the path start with anything else (/home , /opt, ...) we're screwed :( */
	if ( (szName[0]=='/') && (szName[2]=='/')) {
		szName[0] = szName[1];
		szName[1] = ':';
	}
#endif

	is_chap_file = 0;
	handler = 0;
	disable = 0;
	chap_ref = 0;
	is_chap = 0;
	kinds = gf_list_new();
	track_layout = 0;
	szLan = NULL;
	delay = 0;
	group = 0;
	stype = 0;
	profile = level = 0;
	negative_cts_offset = 0;
	split_tile_mode = 0;
	temporal_mode = 0;
	rap_only = 0;
	refs_only = 0;
	txt_flags = 0;
	max_layer_id_plus_one = max_temporal_id_plus_one = 0;
	force_rate = -1;

	tw = th = tx = ty = txtw = txth = txtx = txty = 0;
	par_d = par_n = -2;
	/*use ':' as separator, but beware DOS paths...*/
	ext = strchr(szName, ':');
	if (ext && ext[1]=='\\') ext = strchr(szName+2, ':');

	handler_name = NULL;
	rvc_config = NULL;
	while (ext) {
		char *ext2 = strchr(ext+1, ':');

		if (ext2 && !strncmp(ext2, "://", 3) ) {
			ext2[0] = ':';
			ext2 = strchr(ext2+1, ':');
		}

		if (ext2 && ( !strncmp(ext2, ":\\", 2) || !strncmp(ext2, ":/", 2) ) ) {
			ext2[0] = ':';
			ext2 = strchr(ext2+1, ':');
		}

		if (ext2) ext2[0] = 0;

		/*all extensions for track-based importing*/
		if (!strnicmp(ext+1, "dur=", 4)) import.duration = (u32)( (atof(ext+5) * 1000) + 0.5 );
		else if (!strnicmp(ext+1, "lang=", 5)) {
			/* prevent leak if param is set twice */
			if (szLan)
				gf_free((char*) szLan);

			szLan = gf_strdup(ext+6);
		}
		else if (!strnicmp(ext+1, "delay=", 6)) delay = atoi(ext+7);
		else if (!strnicmp(ext+1, "par=", 4)) {
			if (!stricmp(ext+5, "none")) {
				par_n = par_d = -1;
			} else {
				if (ext2) ext2[0] = ':';
				if (ext2) ext2 = strchr(ext2+1, ':');
				if (ext2) ext2[0] = 0;
				sscanf(ext+5, "%d:%d", &par_n, &par_d);
			}
		}
		else if (!strnicmp(ext+1, "name=", 5)) {
			handler_name = gf_strdup(ext+6);
		}
		else if (!strnicmp(ext+1, "ext=", 4)) {
			/*extensions begin with '.'*/
			if (*(ext+5) == '.')
				import.force_ext = gf_strdup(ext+5);
			else {
				import.force_ext = gf_calloc(1+strlen(ext+5)+1, 1);
				import.force_ext[0] = '.';
				strcat(import.force_ext+1, ext+5);
			}
		}
		else if (!strnicmp(ext+1, "hdlr=", 5)) handler = GF_4CC(ext[6], ext[7], ext[8], ext[9]);
		else if (!strnicmp(ext+1, "disable", 7)) disable = 1;
		else if (!strnicmp(ext+1, "group=", 6)) {
			group = atoi(ext+7);
			if (!group) group = gf_isom_get_next_alternate_group_id(dest);
		}
		else if (!strnicmp(ext+1, "fps=", 4)) {
			if (!strcmp(ext+5, "auto")) force_fps = GF_IMPORT_AUTO_FPS;
			else if (strchr(ext+5, '-')) {
				u32 ticks, dts_inc;
				sscanf(ext+5, "%u-%u", &ticks, &dts_inc);
				if (!dts_inc) dts_inc=1;
				force_fps = ticks;
				force_fps /= dts_inc;
			}
			else force_fps = atof(ext+5);
		}
		else if (!stricmp(ext+1, "rap")) rap_only = 1;
		else if (!stricmp(ext+1, "refs")) refs_only = 1;
		else if (!stricmp(ext+1, "trailing")) import_flags |= GF_IMPORT_KEEP_TRAILING;
		else if (!strnicmp(ext+1, "agg=", 4)) frames_per_sample = atoi(ext+5);
		else if (!stricmp(ext+1, "dref")) import_flags |= GF_IMPORT_USE_DATAREF;
		else if (!stricmp(ext+1, "keep_refs")) import_flags |= GF_IMPORT_KEEP_REFS;
		else if (!stricmp(ext+1, "nodrop")) import_flags |= GF_IMPORT_NO_FRAME_DROP;
		else if (!stricmp(ext+1, "packed")) import_flags |= GF_IMPORT_FORCE_PACKED;
		else if (!stricmp(ext+1, "sbr")) import_flags |= GF_IMPORT_SBR_IMPLICIT;
		else if (!stricmp(ext+1, "sbrx")) import_flags |= GF_IMPORT_SBR_EXPLICIT;
		else if (!stricmp(ext+1, "ovsbr")) import_flags |= GF_IMPORT_OVSBR;
		else if (!stricmp(ext+1, "ps")) import_flags |= GF_IMPORT_PS_IMPLICIT;
		else if (!stricmp(ext+1, "psx")) import_flags |= GF_IMPORT_PS_EXPLICIT;
		else if (!stricmp(ext+1, "mpeg4")) import_flags |= GF_IMPORT_FORCE_MPEG4;
		else if (!stricmp(ext+1, "nosei")) import_flags |= GF_IMPORT_NO_SEI;
		else if (!stricmp(ext+1, "svc") || !stricmp(ext+1, "lhvc") ) import_flags |= GF_IMPORT_SVC_EXPLICIT;
		else if (!stricmp(ext+1, "nosvc") || !stricmp(ext+1, "nolhvc")) import_flags |= GF_IMPORT_SVC_NONE;

		/*split SVC layers*/
		else if (!strnicmp(ext+1, "svcmode=", 8) || !strnicmp(ext+1, "lhvcmode=", 9)) {
			char *mode = ext+9;
			if (mode[0]=='=') mode = ext+10;

			if (!stricmp(mode, "splitnox"))
				svc_mode = 3;
			else if (!stricmp(mode, "splitnoxib"))
				svc_mode = 4;
			else if (!stricmp(mode, "splitall") || !stricmp(mode, "split"))
				svc_mode = 2;
			else if (!stricmp(mode, "splitbase"))
				svc_mode = 1;
			else if (!stricmp(mode, "merged"))
				svc_mode = 0;
		}
		/*split SVC layers*/
		else if (!strnicmp(ext+1, "temporal=", 9)) {
			char *mode = ext+10;
			if (!stricmp(mode, "split"))
				temporal_mode = 2;
			else if (!stricmp(mode, "splitnox"))
				temporal_mode = 3;
			else if (!stricmp(mode, "splitbase"))
				temporal_mode = 1;
			else {
				fprintf(stderr, "Unrecognized temporal mode %s, ignoring\n", mode);
				temporal_mode = 0;
			}
		}
		else if (!stricmp(ext+1, "subsamples")) import_flags |= GF_IMPORT_SET_SUBSAMPLES;
		else if (!stricmp(ext+1, "deps")) import_flags |= GF_IMPORT_SAMPLE_DEPS;
		else if (!stricmp(ext+1, "forcesync")) import_flags |= GF_IMPORT_FORCE_SYNC;
		else if (!stricmp(ext+1, "xps_inband")) import_flags |= GF_IMPORT_FORCE_XPS_INBAND;
		else if (!strnicmp(ext+1, "max_lid=", 8) || !strnicmp(ext+1, "max_tid=", 8)) {
			s32 val = atoi(ext+9);
			if (val < 0) {
				fprintf(stderr, "Warning: request max layer/temporal id is negative - ignoring\n");
			} else {
				if (!strnicmp(ext+1, "max_lid=", 8))
					max_layer_id_plus_one = 1 + (u8) val;
				else
					max_temporal_id_plus_one = 1 + (u8) val;
			}
		}
		else if (!stricmp(ext+1, "tiles")) split_tile_mode = 2;
		else if (!stricmp(ext+1, "tiles_rle")) split_tile_mode = 3;
		else if (!stricmp(ext+1, "split_tiles")) split_tile_mode = 1;

		/*force all composition offsets to be positive*/
		else if (!strnicmp(ext+1, "negctts", 7)) negative_cts_offset = 1;
		else if (!strnicmp(ext+1, "stype=", 6)) {
			stype = GF_4CC(ext[7], ext[8], ext[9], ext[10]);
		}
		else if (!stricmp(ext+1, "chap")) is_chap = 1;
		else if (!strnicmp(ext+1, "chapter=", 8)) {
			chapter_name = gf_strdup(ext+9);
		}
		else if (!strnicmp(ext+1, "chapfile=", 9)) {
			chapter_name = gf_strdup(ext+10);
			is_chap_file=1;
		}
		else if (!strnicmp(ext+1, "layout=", 7)) {
			if ( sscanf(ext+8, "%dx%dx%dx%d", &tw, &th, &tx, &ty)==4) {
				track_layout = 1;
			} else if ( sscanf(ext+8, "%dx%d", &tw, &th)==2) {
				track_layout = 1;
				tx = ty = 0;
			}
		}
		else if (!strnicmp(ext+1, "rescale=", 8)) {
			rescale = atoi(ext+9);
		}
		else if (!strnicmp(ext+1, "timescale=", 10)) {
			new_timescale = atoi(ext+11);
		}
		else if (!stricmp(ext+1, "noedit")) import_flags |= GF_IMPORT_NO_EDIT_LIST;


		else if (!strnicmp(ext+1, "rvc=", 4)) {
			if (sscanf(ext+5, "%d", &rvc_predefined) != 1) {
				rvc_config = gf_strdup(ext+5);
			}
		}
		else if (!strnicmp(ext+1, "fmt=", 4)) import.streamFormat = gf_strdup(ext+5);
		else if (!strnicmp(ext+1, "profile=", 8)) profile = atoi(ext+9);
		else if (!strnicmp(ext+1, "level=", 6)) level = atoi(ext+7);
		else if (!strnicmp(ext+1, "novpsext", 8)) import_flags |= GF_IMPORT_NO_VPS_EXTENSIONS;
		else if (!strnicmp(ext+1, "keepav1t", 8)) import_flags |= GF_IMPORT_KEEP_AV1_TEMPORAL_OBU;

		else if (!strnicmp(ext+1, "font=", 5)) import.fontName = gf_strdup(ext+6);
		else if (!strnicmp(ext+1, "size=", 5)) import.fontSize = atoi(ext+6);
		else if (!strnicmp(ext+1, "text_layout=", 12)) {
			if ( sscanf(ext+13, "%dx%dx%dx%d", &txtw, &txth, &txtx, &txty)==4) {
				text_layout = 1;
			} else if ( sscanf(ext+8, "%dx%d", &txtw, &txth)==2) {
				track_layout = 1;
				txtx = txty = 0;
			}
		}

#ifndef GPAC_DISABLE_SWF_IMPORT
		else if (!stricmp(ext+1, "swf-global")) import.swf_flags |= GF_SM_SWF_STATIC_DICT;
		else if (!stricmp(ext+1, "swf-no-ctrl")) import.swf_flags &= ~GF_SM_SWF_SPLIT_TIMELINE;
		else if (!stricmp(ext+1, "swf-no-text")) import.swf_flags |= GF_SM_SWF_NO_TEXT;
		else if (!stricmp(ext+1, "swf-no-font")) import.swf_flags |= GF_SM_SWF_NO_FONT;
		else if (!stricmp(ext+1, "swf-no-line")) import.swf_flags |= GF_SM_SWF_NO_LINE;
		else if (!stricmp(ext+1, "swf-no-grad")) import.swf_flags |= GF_SM_SWF_NO_GRADIENT;
		else if (!stricmp(ext+1, "swf-quad")) import.swf_flags |= GF_SM_SWF_QUAD_CURVE;
		else if (!stricmp(ext+1, "swf-xlp")) import.swf_flags |= GF_SM_SWF_SCALABLE_LINE;
		else if (!stricmp(ext+1, "swf-ic2d")) import.swf_flags |= GF_SM_SWF_USE_IC2D;
		else if (!stricmp(ext+1, "swf-same-app")) import.swf_flags |= GF_SM_SWF_REUSE_APPEARANCE;
		else if (!strnicmp(ext+1, "swf-flatten=", 12)) import.swf_flatten_angle = (Float) atof(ext+13);
#endif

		else if (!strnicmp(ext+1, "kind=", 5)) {
			char *kind_scheme, *kind_value;
			char *kind_data = ext+6;
			char *sep = strchr(kind_data, '=');
			if (sep) {
				*sep = 0;
			}
			kind_scheme = gf_strdup(kind_data);
			if (sep) {
				*sep = '=';
				kind_value = gf_strdup(sep+1);
			} else {
				kind_value = NULL;
			}
			gf_list_add(kinds, kind_scheme);
			gf_list_add(kinds, kind_value);
		}
		else if (!strnicmp(ext+1, "txtflags", 8)) {
			if (!strnicmp(ext+1, "txtflags=", 9)) {
				sscanf(ext+10, "%x", &txt_flags);
			}
			else if (!strnicmp(ext+1, "txtflags+=", 10)) {
				sscanf(ext+11, "%x", &txt_flags);
				txt_mode = GF_ISOM_TEXT_FLAGS_TOGGLE;
			}
			else if (!strnicmp(ext+1, "txtflags-=", 10)) {
				sscanf(ext+11, "%x", &txt_flags);
				txt_mode = GF_ISOM_TEXT_FLAGS_UNTOGGLE;
			}
		}
		else if (!strnicmp(ext+1, "rate=", 5)) {
			force_rate = atoi(ext+6);
		}

		else if (!strnicmp(ext+1, "asemode=", 8)){
			char *mode = ext+9;
			if (!stricmp(mode, "v0-bs"))
				import.asemode = GF_IMPORT_AUDIO_SAMPLE_ENTRY_v0_BS;
			else if (!stricmp(mode, "v0-2"))
				import.asemode = GF_IMPORT_AUDIO_SAMPLE_ENTRY_v0_2;
			else if (!stricmp(mode, "v1"))
				import.asemode = GF_IMPORT_AUDIO_SAMPLE_ENTRY_v1_MPEG;
			else if (!stricmp(mode, "v1-qt"))
				import.asemode = GF_IMPORT_AUDIO_SAMPLE_ENTRY_v1_QTFF;
		}

		else if (!strnicmp(ext+1, "audio_roll=", 11)) {
			import.audio_roll_change = GF_TRUE;
			import.audio_roll = atoi(ext+12);
		}

		/*unrecognized, assume name has colon in it*/
		else {
			fprintf(stderr, "Unrecognized import option %s, ignoring\n", ext+1);
			ext = ext2;
			continue;
		}

		if (ext2) ext2[0] = ':';

		ext[0] = 0;

		/* restart from where we stopped
		 * if we didn't stop (ext2 null) then the end has been reached
		 * so we can stop the whole thing */
		ext = ext2;
	}

	/*check duration import (old syntax)*/
	ext = strrchr(szName, '%');
	if (ext) {
		import.duration = (u32) (atof(ext+1) * 1000);
		ext[0] = 0;
	}

	/*select switches for av containers import*/
	do_audio = do_video = do_auxv = do_pict = 0;
	track_id = prog_id = 0;
	do_all = 1;
	ext = strrchr(szName, '#');
	if (ext) ext[0] = 0;

	keep_handler = gf_isom_probe_file(szName);

	import.in_name = szName;
	import.flags = GF_IMPORT_PROBE_ONLY;
	e = gf_media_import(&import);
	if (e) goto exit;

	if (ext) {
		ext++;
		if (!strnicmp(ext, "audio", 5)) do_audio = 1;
		else if (!strnicmp(ext, "video", 5)) do_video = 1;
        else if (!strnicmp(ext, "auxv", 4)) do_auxv = 1;
        else if (!strnicmp(ext, "pict", 4)) do_pict = 1;
		else if (!strnicmp(ext, "trackID=", 8)) track_id = atoi(&ext[8]);
		else if (!strnicmp(ext, "PID=", 4)) track_id = atoi(&ext[4]);
		else if (!strnicmp(ext, "program=", 8)) {
			for (i=0; i<import.nb_progs; i++) {
				if (!stricmp(import.pg_info[i].name, ext+8)) {
					prog_id = import.pg_info[i].number;
					do_all = 0;
					break;
				}
			}
		}
		else if (!strnicmp(ext, "prog_id=", 8)) {
			prog_id = atoi(ext+8);
			do_all = 0;
		}
		else track_id = atoi(ext);
	}
	if (do_audio || do_video || do_auxv || do_pict || track_id) do_all = 0;

	if (track_layout || is_chap) {
		u32 w, h, sw, sh, fw, fh, i;
		w = h = sw = sh = fw = fh = 0;
		chap_ref = 0;
		for (i=0; i<gf_isom_get_track_count(dest); i++) {
			switch (gf_isom_get_media_type(dest, i+1)) {
			case GF_ISOM_MEDIA_SCENE:
			case GF_ISOM_MEDIA_VISUAL:
            case GF_ISOM_MEDIA_AUXV:
            case GF_ISOM_MEDIA_PICT:
				if (!chap_ref && gf_isom_is_track_enabled(dest, i+1) ) chap_ref = i+1;

				gf_isom_get_visual_info(dest, i+1, 1, &sw, &sh);
				gf_isom_get_track_layout_info(dest, i+1, &fw, &fh, NULL, NULL, NULL);
				if (w<sw) w = sw;
				if (w<fw) w = fw;
				if (h<sh) h = sh;
				if (h<fh) h = fh;
				break;
			case GF_ISOM_MEDIA_AUDIO:
				if (!chap_ref && gf_isom_is_track_enabled(dest, i+1) ) chap_ref = i+1;
				break;
			}
		}
		if (track_layout) {
			if (!tw) tw = w;
			if (!th) th = h;
			if (ty==-1) ty = (h>(u32)th) ? h-th : 0;
			import.text_width = tw;
			import.text_height = th;
		}
		if (is_chap && chap_ref) import_flags |= GF_IMPORT_NO_TEXT_FLUSH;
	}
	if (text_layout && txtw && txth) {
		import.text_track_width = import.text_width ? import.text_width : txtw;
		import.text_track_height = import.text_height ? import.text_height : txth;
		import.text_width = txtw;
		import.text_height = txth;
		import.text_x = txtx;
		import.text_y = txty;
	}

	check_track_for_svc = check_track_for_lhvc = check_track_for_hevc = 0;

	import.dest = dest;
	import.video_fps = force_fps;
	import.frames_per_sample = frames_per_sample;
	import.flags = import_flags;

	if (!import.nb_tracks) {
		u32 count, o_count;
		o_count = gf_isom_get_track_count(import.dest);
		e = gf_media_import(&import);
		if (e) return e;
		count = gf_isom_get_track_count(import.dest);
		timescale = gf_isom_get_timescale(dest);
		for (i=o_count; i<count; i++) {
			if (szLan) gf_isom_set_media_language(import.dest, i+1, (char *) szLan);
			if (delay) {
				u64 tk_dur;
				gf_isom_remove_edit_segments(import.dest, i+1);
				tk_dur = gf_isom_get_track_duration(import.dest, i+1);
				if (delay>0) {
					gf_isom_append_edit_segment(import.dest, i+1, (timescale*delay)/1000, 0, GF_ISOM_EDIT_EMPTY);
					gf_isom_append_edit_segment(import.dest, i+1, tk_dur, 0, GF_ISOM_EDIT_NORMAL);
				} else if (delay<0) {
					u64 to_skip = (timescale*(-delay))/1000;
					if (to_skip<tk_dur) {
						gf_isom_append_edit_segment(import.dest, i+1, tk_dur-to_skip, to_skip, GF_ISOM_EDIT_NORMAL);
					} else {
						fprintf(stderr, "Warning: request negative delay longer than track duration - ignoring\n");
					}
				}
			}
			if ((par_n>=0) && (par_d>=0)) {
				e = gf_media_change_par(import.dest, i+1, par_n, par_d);
			}

			if (rap_only || refs_only) {
				e = gf_media_remove_non_rap(import.dest, i+1, refs_only);
			}

			if (handler_name) gf_isom_set_handler_name(import.dest, i+1, handler_name);
			else if (!keep_handler) {
				char szHName[1024];
				const char *fName = gf_url_get_resource_name((const  char *)inName);
				fName = strchr(fName, '.');
				if (fName) fName += 1;
				else fName = "?";

				sprintf(szHName, "*%s@GPAC%s", fName, GPAC_FULL_VERSION);
				gf_isom_set_handler_name(import.dest, i+1, szHName);
			}
			if (handler) gf_isom_set_media_type(import.dest, i+1, handler);
			if (disable) gf_isom_set_track_enabled(import.dest, i+1, 0);

			if (group) {
				gf_isom_set_alternate_group_id(import.dest, i+1, group);
			}
			if (track_layout) {
				gf_isom_set_track_layout_info(import.dest, i+1, tw<<16, th<<16, tx<<16, ty<<16, 0);
			}
			if (stype)
				gf_isom_set_media_subtype(import.dest, i+1, 1, stype);

			if (is_chap && chap_ref) {
				set_chapter_track(import.dest, i+1, chap_ref);
			}

			for (j = 0; j < gf_list_count(kinds); j+=2) {
				char *kind_scheme = (char *)gf_list_get(kinds, j);
				char *kind_value = (char *)gf_list_get(kinds, j+1);
				gf_isom_add_track_kind(import.dest, i+1, kind_scheme, kind_value);
			}

			if (profile || level)
				gf_media_change_pl(import.dest, i+1, profile, level);

			if (gf_isom_get_media_subtype(import.dest, i+1, 1)== GF_ISOM_BOX_TYPE_MP4S)
				keep_sys_tracks = 1;

			gf_isom_set_composition_offset_mode(import.dest, i+1, negative_cts_offset);

			if (gf_isom_get_avc_svc_type(import.dest, i+1, 1)>=GF_ISOM_AVCTYPE_AVC_SVC)
				check_track_for_svc = i+1;

			switch (gf_isom_get_hevc_lhvc_type(import.dest, i+1, 1)) {
			case GF_ISOM_HEVCTYPE_HEVC_LHVC:
			case GF_ISOM_HEVCTYPE_LHVC_ONLY:
				check_track_for_lhvc = i+1;
				break;
			case GF_ISOM_HEVCTYPE_HEVC_ONLY:
				check_track_for_hevc=1;
				break;
			}

			if (txt_flags) {
				gf_isom_text_set_display_flags(import.dest, i+1, 0, txt_flags, txt_mode);
			}

			if (force_rate>=0) {
				gf_isom_update_bitrate(import.dest, i+1, 1, force_rate, force_rate, 0);
			}

			if (split_tile_mode) {
				switch (gf_isom_get_media_subtype(import.dest, i+1, 1)) {
				case GF_ISOM_SUBTYPE_HVC1:
				case GF_ISOM_SUBTYPE_HEV1:
				case GF_ISOM_SUBTYPE_HVC2:
				case GF_ISOM_SUBTYPE_HEV2:
					break;
				default:
					split_tile_mode = 0;
					break;
				}
			}
		}
	} else {
		if (do_all)
			import.flags |= GF_IMPORT_KEEP_REFS;

		for (i=0; i<import.nb_tracks; i++) {
			import.trackID = import.tk_info[i].track_num;
			if (prog_id) {
				if (import.tk_info[i].prog_num!=prog_id) continue;
				e = gf_media_import(&import);
			}
			else if (do_all) e = gf_media_import(&import);
			else if (track_id && (track_id==import.trackID)) {
				track_id = 0;
				e = gf_media_import(&import);
			}
			else if (do_audio && (import.tk_info[i].type==GF_ISOM_MEDIA_AUDIO)) {
				do_audio = 0;
				e = gf_media_import(&import);
			}
			else if (do_video && (import.tk_info[i].type==GF_ISOM_MEDIA_VISUAL)) {
				do_video = 0;
				e = gf_media_import(&import);
			}
            else if (do_auxv && (import.tk_info[i].type==GF_ISOM_MEDIA_AUXV)) {
                do_auxv = 0;
                e = gf_media_import(&import);
            }
            else if (do_pict && (import.tk_info[i].type==GF_ISOM_MEDIA_PICT)) {
                do_pict = 0;
                e = gf_media_import(&import);
            }
			else continue;
			if (e) goto exit;

			timescale = gf_isom_get_timescale(dest);
			track = gf_isom_get_track_by_id(import.dest, import.final_trackID);
			if (szLan) gf_isom_set_media_language(import.dest, track, (char *) szLan);
			if (disable) gf_isom_set_track_enabled(import.dest, track, 0);

			if (delay) {
				u64 tk_dur;
				gf_isom_remove_edit_segments(import.dest, track);
				tk_dur = gf_isom_get_track_duration(import.dest, track);
				if (delay>0) {
					gf_isom_append_edit_segment(import.dest, track, (timescale*delay)/1000, 0, GF_ISOM_EDIT_EMPTY);
					gf_isom_append_edit_segment(import.dest, track, tk_dur, 0, GF_ISOM_EDIT_NORMAL);
				} else {
					u64 to_skip = (timescale*(-delay))/1000;
					if (to_skip<tk_dur) {
						u64 media_time = (-delay)*gf_isom_get_media_timescale(import.dest, track) / 1000;
						gf_isom_append_edit_segment(import.dest, track, tk_dur-to_skip, media_time, GF_ISOM_EDIT_NORMAL);
					} else {
						fprintf(stderr, "Warning: request negative delay longer than track duration - ignoring\n");
					}
				}
			}
			if (gf_isom_is_video_subtype(import.tk_info[i].type) && (par_n>=-1) && (par_d>=-1)) {
				e = gf_media_change_par(import.dest, track, par_n, par_d);
			}
			if (rap_only || refs_only) {
				e = gf_media_remove_non_rap(import.dest, track, refs_only);
			}
			if (handler_name) gf_isom_set_handler_name(import.dest, track, handler_name);
			else if (!keep_handler) {
				char szHName[1024];
				const char *fName = gf_url_get_resource_name((const  char *)inName);
				fName = strchr(fName, '.');
				if (fName) fName += 1;
				else fName = "?";

				sprintf(szHName, "%s@GPAC%s", fName, GPAC_FULL_VERSION);
				gf_isom_set_handler_name(import.dest, track, szHName);
			}
			if (handler) gf_isom_set_media_type(import.dest, track, handler);

			if (group) {
				gf_isom_set_alternate_group_id(import.dest, track, group);
			}

			if (track_layout) {
				gf_isom_set_track_layout_info(import.dest, track, tw<<16, th<<16, tx<<16, ty<<16, 0);
			}
			if (stype)
				gf_isom_set_media_subtype(import.dest, track, 1, stype);

			if (is_chap && chap_ref) {
				set_chapter_track(import.dest, track, chap_ref);
			}

			for (j = 0; j < gf_list_count(kinds); j+=2) {
				char *kind_scheme = (char *)gf_list_get(kinds, j);
				char *kind_value = (char *)gf_list_get(kinds, j+1);
				gf_isom_add_track_kind(import.dest, i+1, kind_scheme, kind_value);
			}

			if (profile || level)
				gf_media_change_pl(import.dest, track, profile, level);

			if (gf_isom_get_mpeg4_subtype(import.dest, track, 1))
				keep_sys_tracks = 1;

			if (new_timescale>1) {
				gf_isom_set_media_timescale(import.dest, track, new_timescale, 0);
			}

			if (rescale>1) {
				switch (gf_isom_get_media_type(import.dest, track)) {
				case GF_ISOM_MEDIA_AUDIO:
					fprintf(stderr, "Cannot force media timescale for audio media types - ignoring\n");
					break;
				default:
					gf_isom_set_media_timescale(import.dest, track, rescale, 1);
					break;
				}
			}

			if (rvc_config) {
				FILE *f = gf_fopen(rvc_config, "rb");
				if (f) {
					char *data;
					u32 size;
					size_t read;
					gf_fseek(f, 0, SEEK_END);
					size = (u32) gf_ftell(f);
					gf_fseek(f, 0, SEEK_SET);
					data = gf_malloc(sizeof(char)*size);
					read = fread(data, 1, size, f);
					gf_fclose(f);
					if (read != size) {
						fprintf(stderr, "Error: could not read rvc config from %s\n", rvc_config);
						e = GF_IO_ERR;
						goto exit;
					}
#ifdef GPAC_DISABLE_ZLIB
					fprintf(stderr, "Error: no zlib support - RVC not available\n");
					e = GF_NOT_SUPPORTED;
					goto exit;
#else
					gf_gz_compress_payload(&data, size, &size);
#endif
					gf_isom_set_rvc_config(import.dest, track, 1, 0, "application/rvc-config+xml+gz", data, size);
					gf_free(data);
				}
			} else if (rvc_predefined>0) {
				gf_isom_set_rvc_config(import.dest, track, 1, rvc_predefined, NULL, NULL, 0);
			}

			gf_isom_set_composition_offset_mode(import.dest, track, negative_cts_offset);

			if (gf_isom_get_avc_svc_type(import.dest, track, 1)>=GF_ISOM_AVCTYPE_AVC_SVC)
				check_track_for_svc = track;

			switch (gf_isom_get_hevc_lhvc_type(import.dest, track, 1)) {
			case GF_ISOM_HEVCTYPE_HEVC_LHVC:
			case GF_ISOM_HEVCTYPE_LHVC_ONLY:
				check_track_for_lhvc = i+1;
				break;
			case GF_ISOM_HEVCTYPE_HEVC_ONLY:
				check_track_for_hevc=1;
				break;
			}

			if (txt_flags) {
				gf_isom_text_set_display_flags(import.dest, track, 0, txt_flags, txt_mode);
			}
			if (force_rate>=0) {
				gf_isom_update_bitrate(import.dest, i+1, 1, force_rate, force_rate, 0);
			}

			if (split_tile_mode) {
				switch (gf_isom_get_media_subtype(import.dest, track, 1)) {
				case GF_ISOM_SUBTYPE_HVC1:
				case GF_ISOM_SUBTYPE_HEV1:
				case GF_ISOM_SUBTYPE_HVC2:
				case GF_ISOM_SUBTYPE_HEV2:
					break;
				default:
					split_tile_mode = 0;
					break;
				}
			}
		}
		if (track_id) fprintf(stderr, "WARNING: Track ID %d not found in file\n", track_id);
		else if (do_video) fprintf(stderr, "WARNING: Video track not found\n");
        else if (do_auxv) fprintf(stderr, "WARNING: Auxiliary Video track not found\n");
        else if (do_pict) fprintf(stderr, "WARNING: Picture sequence track not found\n");
		else if (do_audio) fprintf(stderr, "WARNING: Audio track not found\n");
	}

	if (chapter_name) {
		if (is_chap_file) {
			e = gf_media_import_chapters(import.dest, chapter_name, 0);
		} else {
			e = gf_isom_add_chapter(import.dest, 0, 0, chapter_name);
		}
	}

	/*force to rewrite all dependencies*/
	for (i = 1; i <= gf_isom_get_track_count(import.dest); i++)
	{
		e = gf_isom_rewrite_track_dependencies(import.dest, i);
		if (e) {
			GF_LOG(GF_LOG_WARNING, GF_LOG_CONTAINER, ("Warning: track ID %d has references to a track not imported\n", gf_isom_get_track_id(import.dest, i) ));
			e = GF_OK;
		}
	}

	if (max_layer_id_plus_one || max_temporal_id_plus_one) {
		for (i = 1; i <= gf_isom_get_track_count(import.dest); i++)
		{
			e = gf_media_filter_hevc(import.dest, i, max_temporal_id_plus_one, max_layer_id_plus_one);
			if (e) {
				fprintf(stderr, "Warning: track ID %d: error while filtering LHVC layers\n", gf_isom_get_track_id(import.dest, i));
				e = GF_OK;
			}
		}
	}

	if (check_track_for_svc) {
		if (svc_mode) {
			e = gf_media_split_svc(import.dest, check_track_for_svc, (svc_mode==2) ? 1 : 0);
			if (e) goto exit;
		} else {
			e = gf_media_merge_svc(import.dest, check_track_for_svc, 1);
			if (e) goto exit;
		}
	}
#ifndef GPAC_DISABLE_HEVC
	if (check_track_for_lhvc) {
		if (svc_mode) {
			GF_LHVCExtractoreMode xmode = GF_LHVC_EXTRACTORS_ON;
			if (svc_mode==3) xmode = GF_LHVC_EXTRACTORS_OFF;
			else if (svc_mode==4) xmode = GF_LHVC_EXTRACTORS_OFF_FORCE_INBAND;
			e = gf_media_split_lhvc(import.dest, check_track_for_lhvc, GF_FALSE, (svc_mode==1) ? 0 : 1, xmode );
			if (e) goto exit;
		} else {
		}
	}
	if (check_track_for_hevc) {
		if (split_tile_mode) {
			e = gf_media_split_hevc_tiles(import.dest, split_tile_mode - 1);
			if (e) goto exit;
		}
		if (temporal_mode) {
			GF_LHVCExtractoreMode xmode = (temporal_mode==3) ? GF_LHVC_EXTRACTORS_OFF : GF_LHVC_EXTRACTORS_ON;
			e = gf_media_split_lhvc(import.dest, check_track_for_hevc, GF_TRUE, (temporal_mode==1) ? GF_FALSE : GF_TRUE, xmode );
			if (e) goto exit;
		}
	}

#endif /*GPAC_DISABLE_HEVC*/

exit:
	while (gf_list_count(kinds)) {
		char *kind = (char *)gf_list_get(kinds, 0);
		gf_list_rem(kinds, 0);
		if (kind) gf_free(kind);
	}
	gf_list_del(kinds);
	if (handler_name) gf_free(handler_name);
	if (chapter_name ) gf_free(chapter_name);
	if (import.fontName) gf_free(import.fontName);
	if (import.streamFormat) gf_free(import.streamFormat);
	if (import.force_ext) gf_free(import.force_ext);
	if (rvc_config) gf_free(rvc_config);
	if (szLan) gf_free((char *)szLan);
	return e;
}
