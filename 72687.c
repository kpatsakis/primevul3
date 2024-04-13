GF_Err EncodeFileChunk(char *chunkFile, char *bifs, char *inputContext, char *outputContext, const char *tmpdir)
{
#if defined(GPAC_DISABLE_SMGR) || defined(GPAC_DISABLE_BIFS_ENC) || defined(GPAC_DISABLE_SCENE_ENCODER) || defined (GPAC_DISABLE_SCENE_DUMP)
	fprintf(stderr, "BIFS encoding is not supported in this build of GPAC\n");
	return GF_NOT_SUPPORTED;
#else
	GF_Err e;
	GF_SceneGraph *sg;
	GF_SceneManager	*ctx;
	GF_SceneLoader load;

	/*Step 1: create context and load input*/
	sg = gf_sg_new();
	ctx = gf_sm_new(sg);
	memset(&load, 0, sizeof(GF_SceneLoader));
	load.fileName = inputContext;
	load.ctx = ctx;
	/*since we're encoding we must get MPEG4 nodes only*/
	load.flags = GF_SM_LOAD_MPEG4_STRICT;
	e = gf_sm_load_init(&load);
	if (!e) e = gf_sm_load_run(&load);
	gf_sm_load_done(&load);
	if (e) {
		fprintf(stderr, "Cannot load context %s - %s\n", inputContext, gf_error_to_string(e));
		goto exit;
	}

	/* Step 2: make sure we have only ONE RAP for each stream*/
	e = gf_sm_aggregate(ctx, 0);
	if (e) goto exit;

	/*Step 3: loading the chunk into the context*/
	memset(&load, 0, sizeof(GF_SceneLoader));
	load.fileName = chunkFile;
	load.ctx = ctx;
	load.flags = GF_SM_LOAD_MPEG4_STRICT | GF_SM_LOAD_CONTEXT_READY;
	e = gf_sm_load_init(&load);
	if (!e) e = gf_sm_load_run(&load);
	gf_sm_load_done(&load);
	if (e) {
		fprintf(stderr, "Cannot load chunk context %s - %s\n", chunkFile, gf_error_to_string(e));
		goto exit;
	}
	fprintf(stderr, "Context and chunks loaded\n");

	/* Assumes that the first AU contains only one command a SceneReplace and
	   that is not part of the current chunk */
	/* Last argument is a callback to pass the encoded AUs: not needed here
	   Saving is not handled correctly */
	e = EncodeBIFSChunk(ctx, bifs, NULL);
	if (e) goto exit;


	if (outputContext) {
		u32 d_mode, do_enc;
		char szF[GF_MAX_PATH], *ext;

		/*make random access for storage*/
		e = gf_sm_aggregate(ctx, 0);
		if (e) goto exit;

		/*check if we dump to BT, XMT or encode to MP4*/
		strcpy(szF, outputContext);
		ext = strrchr(szF, '.');
		d_mode = GF_SM_DUMP_BT;
		do_enc = 0;
		if (ext) {
			if (!stricmp(ext, ".xmt") || !stricmp(ext, ".xmta")) d_mode = GF_SM_DUMP_XMTA;
			else if (!stricmp(ext, ".mp4")) do_enc = 1;
			ext[0] = 0;
		}

		if (do_enc) {
			GF_ISOFile *mp4;
			strcat(szF, ".mp4");
			mp4 = gf_isom_open(szF, GF_ISOM_WRITE_EDIT, tmpdir);
			e = gf_sm_encode_to_file(ctx, mp4, NULL);
			if (e) gf_isom_delete(mp4);
			else gf_isom_close(mp4);
		}
		else e = gf_sm_dump(ctx, szF, GF_FALSE, d_mode);
	}

exit:
	if (ctx) {
		sg = ctx->scene_graph;
		gf_sm_del(ctx);
		gf_sg_del(sg);
	}

	return e;

#endif /*defined(GPAC_DISABLE_BIFS_ENC) || defined(GPAC_DISABLE_SCENE_ENCODER) || defined (GPAC_DISABLE_SCENE_DUMP)*/

}
