void convert_file_info(char *inName, u32 trackID)
{
	GF_Err e;
	u32 i;
	Bool found;
	GF_MediaImporter import;
	memset(&import, 0, sizeof(GF_MediaImporter));
	import.trackID = trackID;
	import.in_name = inName;
	import.flags = GF_IMPORT_PROBE_ONLY;
	e = gf_media_import(&import);
	if (e) {
		fprintf(stderr, "Error probing file %s: %s\n", inName, gf_error_to_string(e));
		return;
	}
	if (trackID) {
		fprintf(stderr, "Import probing results for track %s#%d:\n", inName, trackID);
	} else {
		fprintf(stderr, "Import probing results for %s:\n", inName);
		if (!import.nb_tracks) {
			fprintf(stderr, "File has no selectable tracks\n");
			return;
		}
		fprintf(stderr, "File has %d tracks\n", import.nb_tracks);
	}
	if (import.probe_duration) {
		fprintf(stderr, "Duration: %g s\n", (Double) (import.probe_duration/1000.0));
	}
	found = 0;
	for (i=0; i<import.nb_tracks; i++) {
		if (trackID && (trackID != import.tk_info[i].track_num)) continue;
		if (!trackID) fprintf(stderr, "\tTrack %d type: ", import.tk_info[i].track_num);
		else fprintf(stderr, "Track type: ");

		switch (import.tk_info[i].type) {
		case GF_ISOM_MEDIA_VISUAL:
			fprintf(stderr, "Video (%s)", gf_4cc_to_str(import.tk_info[i].media_type));
			if (import.tk_info[i].video_info.temporal_enhancement) fprintf(stderr, " Temporal Enhancement");
			break;
        case GF_ISOM_MEDIA_AUXV:
            fprintf(stderr, "Auxiliary Video (%s)", gf_4cc_to_str(import.tk_info[i].media_type));
            break;
        case GF_ISOM_MEDIA_PICT:
            fprintf(stderr, "Picture Sequence (%s)", gf_4cc_to_str(import.tk_info[i].media_type));
            break;
		case GF_ISOM_MEDIA_AUDIO:
			fprintf(stderr, "Audio (%s)", gf_4cc_to_str(import.tk_info[i].media_type));
			break;
		case GF_ISOM_MEDIA_TEXT:
			fprintf(stderr, "Text (%s)", gf_4cc_to_str(import.tk_info[i].media_type));
			break;
		case GF_ISOM_MEDIA_SCENE:
			fprintf(stderr, "Scene (%s)", gf_4cc_to_str(import.tk_info[i].media_type));
			break;
		case GF_ISOM_MEDIA_OD:
			fprintf(stderr, "OD (%s)", gf_4cc_to_str(import.tk_info[i].media_type));
			break;
		case GF_ISOM_MEDIA_META:
			fprintf(stderr, "Metadata (%s)", gf_4cc_to_str(import.tk_info[i].media_type));
			break;
		default:
			fprintf(stderr, "Other (4CC: %s)", gf_4cc_to_str(import.tk_info[i].type));
			break;
		}

		if (import.tk_info[i].lang) fprintf(stderr, " - lang %s", gf_4cc_to_str(import.tk_info[i].lang));

		if (import.tk_info[i].mpeg4_es_id) fprintf(stderr, " - MPEG-4 ESID %d", import.tk_info[i].mpeg4_es_id);

		if (import.tk_info[i].prog_num) {
			if (!import.nb_progs) {
				fprintf(stderr, " - Program %d", import.tk_info[i].prog_num);
			} else {
				u32 j;
				for (j=0; j<import.nb_progs; j++) {
					if (import.tk_info[i].prog_num != import.pg_info[j].number) continue;
					fprintf(stderr, " - Program %s", import.pg_info[j].name);
					break;
				}
			}
		}
		fprintf(stderr, "\n");
		if (!trackID) continue;


		if (gf_isom_is_video_subtype(import.tk_info[i].type) && import.tk_info[i].video_info.width && import.tk_info[i].video_info.height
		   ) {
			fprintf(stderr, "Source: %s %dx%d", gf_4cc_to_str(import.tk_info[i].media_type), import.tk_info[i].video_info.width, import.tk_info[i].video_info.height);
			if (import.tk_info[i].video_info.FPS) fprintf(stderr, " @ %g FPS", import.tk_info[i].video_info.FPS);
			if (import.tk_info[i].video_info.par) fprintf(stderr, " PAR: %d:%d", import.tk_info[i].video_info.par >> 16, import.tk_info[i].video_info.par & 0xFFFF);
			fprintf(stderr, "\n");
		}
		else if ((import.tk_info[i].type==GF_ISOM_MEDIA_AUDIO) && import.tk_info[i].audio_info.sample_rate) {
			fprintf(stderr, "Source: %s - SampleRate %d - %d channels\n", gf_4cc_to_str(import.tk_info[i].media_type), import.tk_info[i].audio_info.sample_rate, import.tk_info[i].audio_info.nb_channels);
		} else {
			fprintf(stderr, "Source: %s\n", gf_4cc_to_str(import.tk_info[i].media_type));
		}


		fprintf(stderr, "\nImport Capabilities:\n");
		if (import.tk_info[i].flags & GF_IMPORT_USE_DATAREF) fprintf(stderr, "\tCan use data referencing\n");
		if (import.tk_info[i].flags & GF_IMPORT_NO_FRAME_DROP) fprintf(stderr, "\tCan use fixed FPS import\n");
		if (import.tk_info[i].flags & GF_IMPORT_FORCE_PACKED) fprintf(stderr, "\tCan force packed bitstream import\n");
		if (import.tk_info[i].flags & GF_IMPORT_OVERRIDE_FPS) fprintf(stderr, "\tCan override source frame rate\n");
		if (import.tk_info[i].flags & (GF_IMPORT_SBR_IMPLICIT|GF_IMPORT_SBR_EXPLICIT)) fprintf(stderr, "\tCan use AAC-SBR signaling\n");
		if (import.tk_info[i].flags & (GF_IMPORT_PS_IMPLICIT|GF_IMPORT_PS_EXPLICIT)) fprintf(stderr, "\tCan use AAC-PS signaling\n");
		if (import.tk_info[i].flags & GF_IMPORT_FORCE_MPEG4) fprintf(stderr, "\tCan force MPEG-4 Systems signaling\n");
		if (import.tk_info[i].flags & GF_IMPORT_3GPP_AGGREGATION) fprintf(stderr, "\tCan use 3GPP frame aggregation\n");
		if (import.tk_info[i].flags & GF_IMPORT_NO_DURATION) fprintf(stderr, "\tCannot use duration-based import\n");

		found = 1;
		break;
	}
	fprintf(stderr, "\n");
	if (!found && trackID) fprintf(stderr, "Cannot find track %d in file\n", trackID);
}
