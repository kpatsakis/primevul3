void PrintAVInfo(Bool final)
{
	GF_MediaInfo a_odi, v_odi, s_odi;
	Double avg_dec_time=0;
	u32 tot_time=0;
	Bool print_codecs = final;

	if (scene_odm) {
		GF_ObjectManager *root_odm = gf_term_get_root_object(term);
		u32 count = gf_term_get_object_count(term, root_odm);
		if (last_odm_count != count) {
			last_odm_count = count;
			scene_odm = NULL;
		}
	}
	if (!video_odm && !audio_odm && !scene_odm) {
		u32 count, i;
		GF_ObjectManager *root_odm = root_odm = gf_term_get_root_object(term);
		if (!root_odm) return;

		if (gf_term_get_object_info(term, root_odm, &v_odi)==GF_OK) {
			if (!scene_odm  && (v_odi.generated_scene== 0)) {
				scene_odm = root_odm;
			}
		}

		count = gf_term_get_object_count(term, root_odm);
		for (i=0; i<count; i++) {
			GF_ObjectManager *odm = gf_term_get_object(term, root_odm, i);
			if (!odm) break;
			if (gf_term_get_object_info(term, odm, &v_odi) == GF_OK) {
				if (!video_odm && (v_odi.od_type == GF_STREAM_VISUAL) && (v_odi.raw_media || (v_odi.cb_max_count>1) || v_odi.direct_video_memory || (bench_mode == 3) )) {
					video_odm = odm;
				}
				else if (!audio_odm && (v_odi.od_type == GF_STREAM_AUDIO)) {
					audio_odm = odm;
				}
				else if (!scene_odm && (v_odi.od_type == GF_STREAM_SCENE)) {
					scene_odm = odm;
				}
			}
		}
	}

	if (0 && bench_buffer) {
		fprintf(stderr, "Buffering %d %% ", bench_buffer-1);
		return;
	}

	if (video_odm) {
		if (gf_term_get_object_info(term, video_odm, &v_odi)!= GF_OK) {
			video_odm = NULL;
			return;
		}
	} else {
		memset(&v_odi, 0, sizeof(v_odi));
	}
	if (print_codecs && audio_odm) {
		gf_term_get_object_info(term, audio_odm, &a_odi);
	} else {
		memset(&a_odi, 0, sizeof(a_odi));
	}
	if ((print_codecs || !video_odm) && scene_odm) {
		gf_term_get_object_info(term, scene_odm, &s_odi);
	} else {
		memset(&s_odi, 0, sizeof(s_odi));
	}

	if (final) {
		tot_time = gf_sys_clock() - bench_mode_start;
		fprintf(stderr, "                                                                                     \r");
		fprintf(stderr, "************** Bench Mode Done in %d ms ********************\n", tot_time);
		if (bench_mode==3) fprintf(stderr, "** Systems layer only (no decoding) **\n");

		if (!video_odm) {
			u32 nb_frames_drawn;
			Double FPS = gf_term_get_simulation_frame_rate(term, &nb_frames_drawn);
			fprintf(stderr, "Drawn %d frames FPS %.2f (simulation FPS %.2f) - duration %d ms\n", nb_frames_drawn, ((Float)nb_frames_drawn*1000)/tot_time,(Float) FPS, gf_term_get_time_in_ms(term)  );
		}
	}
	if (print_codecs) {
		if (video_odm) {
			fprintf(stderr, "%s %dx%d sar=%d:%d duration %.2fs\n", v_odi.codec_name, v_odi.width, v_odi.height, v_odi.par ? (v_odi.par>>16)&0xFF : 1, v_odi.par ? (v_odi.par)&0xFF : 1, v_odi.duration);
			if (final) {
				u32 dec_run_time = v_odi.last_frame_time - v_odi.first_frame_time;
				if (!dec_run_time) dec_run_time = 1;
				if (v_odi.duration) fprintf(stderr, "%d%% ", (u32) (100*v_odi.current_time / v_odi.duration ) );
				fprintf(stderr, "%d frames FPS %.2f (max %d us/f) rate avg %d max %d", v_odi.nb_dec_frames, ((Float)v_odi.nb_dec_frames*1000) / dec_run_time, v_odi.max_dec_time, (u32) v_odi.avg_bitrate/1000, (u32) v_odi.max_bitrate/1000);
				if (v_odi.nb_dropped) {
					fprintf(stderr, " (Error during bench: %d frames drop)", v_odi.nb_dropped);
				}
				fprintf(stderr, "\n");
			}
		}
		if (audio_odm) {
			fprintf(stderr, "%s SR %d num channels %d bpp %d duration %.2fs\n", a_odi.codec_name, a_odi.sample_rate, a_odi.num_channels, a_odi.bits_per_sample, a_odi.duration);
			if (final) {
				u32 dec_run_time = a_odi.last_frame_time - a_odi.first_frame_time;
				if (!dec_run_time) dec_run_time = 1;
				if (a_odi.duration) fprintf(stderr, "%d%% ", (u32) (100*a_odi.current_time / a_odi.duration ) );
				fprintf(stderr, "%d frames (ms/f %.2f avg %.2f max) rate avg %d max %d", a_odi.nb_dec_frames, ((Float)dec_run_time)/a_odi.nb_dec_frames, a_odi.max_dec_time/1000.0, (u32) a_odi.avg_bitrate/1000, (u32) a_odi.max_bitrate/1000);
				if (a_odi.nb_dropped) {
					fprintf(stderr, " (Error during bench: %d frames drop)", a_odi.nb_dropped);
				}
				fprintf(stderr, "\n");
			}
		}
		if (scene_odm) {
			u32 w, h;
			gf_term_get_visual_output_size(term, &w, &h);
			fprintf(stderr, "%s scene size %dx%d rastered to %dx%d duration %.2fs\n", s_odi.codec_name ? s_odi.codec_name : "", s_odi.width, s_odi.height, w, h, s_odi.duration);
			if (final) {
				if (s_odi.nb_dec_frames>2 && s_odi.total_dec_time) {
					u32 dec_run_time = s_odi.last_frame_time - s_odi.first_frame_time;
					if (!dec_run_time) dec_run_time = 1;
					fprintf(stderr, "%d frames FPS %.2f (max %d us/f) rate avg %d max %d", s_odi.nb_dec_frames, ((Float)s_odi.nb_dec_frames*1000) / dec_run_time, s_odi.max_dec_time, (u32) s_odi.avg_bitrate/1000, (u32) s_odi.max_bitrate/1000);
					fprintf(stderr, "\n");
				} else {
					u32 nb_frames_drawn;
					Double FPS;
					gf_term_get_simulation_frame_rate(term, &nb_frames_drawn);
					tot_time = gf_sys_clock() - bench_mode_start;
					FPS = gf_term_get_framerate(term, 0);
					fprintf(stderr, "%d frames FPS %.2f (abs %.2f)\n", nb_frames_drawn, (1000.0*nb_frames_drawn / tot_time), FPS);
				}
			}
		}
		if (final) {
			fprintf(stderr, "**********************************************************\n\n");
			return;
		}
	}

	if (video_odm) {
		tot_time = v_odi.last_frame_time - v_odi.first_frame_time;
		if (!tot_time) tot_time=1;
		if (v_odi.duration) fprintf(stderr, "%d%% ", (u32) (100*v_odi.current_time / v_odi.duration ) );
		fprintf(stderr, "%d f FPS %.2f (%.2f ms max) rate %d ", v_odi.nb_dec_frames, ((Float)v_odi.nb_dec_frames*1000) / tot_time, v_odi.max_dec_time/1000.0, (u32) v_odi.instant_bitrate/1000);
	}
	else if (scene_odm) {

		if (s_odi.nb_dec_frames>2 && s_odi.total_dec_time) {
			avg_dec_time = (Float) 1000000 * s_odi.nb_dec_frames;
			avg_dec_time /= s_odi.total_dec_time;
			if (s_odi.duration) fprintf(stderr, "%d%% ", (u32) (100*s_odi.current_time / s_odi.duration ) );
			fprintf(stderr, "%d f %.2f (%d us max) - rate %d ", s_odi.nb_dec_frames, avg_dec_time, s_odi.max_dec_time, (u32) s_odi.instant_bitrate/1000);
		} else {
			u32 nb_frames_drawn;
			Double FPS;
			gf_term_get_simulation_frame_rate(term, &nb_frames_drawn);
			tot_time = gf_sys_clock() - bench_mode_start;
			FPS = gf_term_get_framerate(term, 1);
			fprintf(stderr, "%d f FPS %.2f (abs %.2f) ", nb_frames_drawn, (1000.0*nb_frames_drawn / tot_time), FPS);
		}
	}
	else if (audio_odm) {
		if (!print_codecs) {
			gf_term_get_object_info(term, audio_odm, &a_odi);
		}
		tot_time = a_odi.last_frame_time - a_odi.first_frame_time;
		if (!tot_time) tot_time=1;
		if (a_odi.duration) fprintf(stderr, "%d%% ", (u32) (100*a_odi.current_time / a_odi.duration ) );
		fprintf(stderr, "%d frames (ms/f %.2f avg %.2f max)", a_odi.nb_dec_frames, ((Float)tot_time)/a_odi.nb_dec_frames, a_odi.max_dec_time/1000.0);
	}
}
