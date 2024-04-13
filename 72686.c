GF_Err EncodeFile(char *in, GF_ISOFile *mp4, GF_SMEncodeOptions *opts, FILE *logs)
{
#ifdef GPAC_DISABLE_SMGR
	return GF_NOT_SUPPORTED;
#else
	GF_Err e;
	GF_SceneLoader load;
	GF_SceneManager *ctx;
	GF_SceneGraph *sg;
#ifndef GPAC_DISABLE_SCENE_STATS
	GF_StatManager *statsman = NULL;
#endif

	sg = gf_sg_new();
	ctx = gf_sm_new(sg);
	memset(&load, 0, sizeof(GF_SceneLoader));
	load.fileName = in;
	load.ctx = ctx;
	load.swf_import_flags = swf_flags;
	load.swf_flatten_limit = swf_flatten_angle;
	/*since we're encoding we must get MPEG4 nodes only*/
	load.flags = GF_SM_LOAD_MPEG4_STRICT;
	e = gf_sm_load_init(&load);
	if (e<0) {
		gf_sm_load_done(&load);
		fprintf(stderr, "Cannot load context %s - %s\n", in, gf_error_to_string(e));
		goto err_exit;
	}
	e = gf_sm_load_run(&load);
	gf_sm_load_done(&load);

#ifndef GPAC_DISABLE_SCENE_STATS
	if (opts->auto_quant) {
		fprintf(stderr, "Analysing Scene for Automatic Quantization\n");
		statsman = gf_sm_stats_new();
		e = gf_sm_stats_for_scene(statsman, ctx);
		if (!e) {
			GF_SceneStatistics *stats = gf_sm_stats_get(statsman);
			/*LASeR*/
			if (opts->auto_quant==1) {
				if (opts->resolution > (s32)stats->frac_res_2d) {
					fprintf(stderr, " Given resolution %d is (unnecessarily) too high, using %d instead.\n", opts->resolution, stats->frac_res_2d);
					opts->resolution = stats->frac_res_2d;
				} else if (stats->int_res_2d + opts->resolution <= 0) {
					fprintf(stderr, " Given resolution %d is too low, using %d instead.\n", opts->resolution, stats->int_res_2d - 1);
					opts->resolution = 1 - stats->int_res_2d;
				}
				opts->coord_bits = stats->int_res_2d + opts->resolution;
				fprintf(stderr, " Coordinates & Lengths encoded using ");
				if (opts->resolution < 0) fprintf(stderr, "only the %d most significant bits (of %d).\n", opts->coord_bits, stats->int_res_2d);
				else fprintf(stderr, "a %d.%d representation\n", stats->int_res_2d, opts->resolution);

				fprintf(stderr, " Matrix Scale & Skew Coefficients ");
				if (opts->coord_bits - 8 < stats->scale_int_res_2d) {
					opts->scale_bits = stats->scale_int_res_2d - opts->coord_bits + 8;
					fprintf(stderr, "encoded using a %d.8 representation\n", stats->scale_int_res_2d);
				} else  {
					opts->scale_bits = 0;
					fprintf(stderr, "encoded using a %d.8 representation\n", opts->coord_bits - 8);
				}
			}
#ifndef GPAC_DISABLE_VRML
			/*BIFS*/
			else if (stats->base_layer) {
				GF_AUContext *au;
				GF_CommandField *inf;
				M_QuantizationParameter *qp;
				GF_Command *com = gf_sg_command_new(ctx->scene_graph, GF_SG_GLOBAL_QUANTIZER);
				qp = (M_QuantizationParameter *) gf_node_new(ctx->scene_graph, TAG_MPEG4_QuantizationParameter);

				inf = gf_sg_command_field_new(com);
				inf->new_node = (GF_Node *)qp;
				inf->field_ptr = &inf->new_node;
				inf->fieldType = GF_SG_VRML_SFNODE;
				gf_node_register(inf->new_node, NULL);
				au = gf_list_get(stats->base_layer->AUs, 0);
				gf_list_insert(au->commands, com, 0);
				qp->useEfficientCoding = 1;
				qp->textureCoordinateQuant = 0;
				if ((stats->count_2f+stats->count_2d) && opts->resolution) {
					qp->position2DMin = stats->min_2d;
					qp->position2DMax = stats->max_2d;
					qp->position2DNbBits = opts->resolution;
					qp->position2DQuant = 1;
				}
				if ((stats->count_3f+stats->count_3d) &&  opts->resolution) {
					qp->position3DMin = stats->min_3d;
					qp->position3DMax = stats->max_3d;
					qp->position3DQuant = opts->resolution;
					qp->position3DQuant = 1;
					qp->textureCoordinateQuant = 1;
				}
#if 0
				if (stats->count_float && opts->resolution) {
					qp->scaleMin = stats->min_fixed;
					qp->scaleMax = stats->max_fixed;
					qp->scaleNbBits = 2*opts->resolution;
					qp->scaleQuant = 1;
				}
#endif
			}
#endif
		}
	}
#endif /*GPAC_DISABLE_SCENE_STATS*/

	if (e<0) {
		fprintf(stderr, "Error loading file %s\n", gf_error_to_string(e));
		goto err_exit;
	} else {
		gf_log_cbk prev_logs = NULL;
		if (logs) {
			gf_log_set_tool_level(GF_LOG_CODING, GF_LOG_DEBUG);
			prev_logs = gf_log_set_callback(logs, scene_coding_log);
		}
		opts->src_url = in;
		e = gf_sm_encode_to_file(ctx, mp4, opts);
		if (logs) {
			gf_log_set_tool_level(GF_LOG_CODING, GF_LOG_ERROR);
			gf_log_set_callback(NULL, prev_logs);
		}
	}

	gf_isom_set_brand_info(mp4, GF_ISOM_BRAND_MP42, 1);
	gf_isom_modify_alternate_brand(mp4, GF_ISOM_BRAND_ISOM, 1);

err_exit:
#ifndef GPAC_DISABLE_SCENE_STATS
	if (statsman) gf_sm_stats_del(statsman);
#endif
	gf_sm_del(ctx);
	gf_sg_del(sg);
	return e;

#endif /*GPAC_DISABLE_SMGR*/
}
