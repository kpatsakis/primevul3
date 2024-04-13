GF_Err gf_sm_aggregate(GF_SceneManager *ctx, u16 ESID)
{
	GF_Err e;
	u32 i, stream_count;
#ifndef GPAC_DISABLE_VRML
	u32 j;
	GF_AUContext *au;
	GF_Command *com;
#endif

	e = GF_OK;

#if DEBUG_RAP
	com_count = 0;
	stream_count = gf_list_count(ctx->streams);
	for (i=0; i<stream_count; i++) {
		GF_StreamContext *sc = (GF_StreamContext *)gf_list_get(ctx->streams, i);
		if (sc->streamType == GF_STREAM_SCENE) {
			au_count = gf_list_count(sc->AUs);
			for (j=0; j<au_count; j++) {
				au = (GF_AUContext *)gf_list_get(sc->AUs, j);
				com_count += gf_list_count(au->commands);
			}
		}
	}
	GF_LOG(GF_LOG_INFO, GF_LOG_SCENE, ("[SceneManager] Making RAP with %d commands\n", com_count));
#endif

	stream_count = gf_list_count(ctx->streams);
	for (i=0; i<stream_count; i++) {
		GF_AUContext *carousel_au;
		GF_List *carousel_commands;
		GF_StreamContext *aggregate_on_stream;
		GF_StreamContext *sc = (GF_StreamContext *)gf_list_get(ctx->streams, i);
		if (ESID && (sc->ESID!=ESID)) continue;

		/*locate the AU in which our commands will be aggregated*/
		carousel_au = NULL;
		carousel_commands = NULL;
		aggregate_on_stream = sc->aggregate_on_esid ? gf_sm_get_stream(ctx, sc->aggregate_on_esid) : NULL;
		if (aggregate_on_stream==sc) {
			carousel_commands = gf_list_new();
		} else if (aggregate_on_stream) {
			if (!gf_list_count(aggregate_on_stream->AUs)) {
				carousel_au = gf_sm_stream_au_new(aggregate_on_stream, 0, 0, 1);
			} else {
				/* assert we already performed aggregation */
				assert(gf_list_count(aggregate_on_stream->AUs)==1);
				carousel_au = gf_list_get(aggregate_on_stream->AUs, 0);
			}
			carousel_commands = carousel_au->commands;
		}
		/*TODO - do this as well for ODs*/
#ifndef GPAC_DISABLE_VRML
		if (sc->streamType == GF_STREAM_SCENE) {
			Bool has_modif = 0;
			/*we check for each stream if it is a base stream (SceneReplace ...) - several streams may carry RAPs if inline nodes are used*/
			Bool base_stream_found = 0;

			/*in DIMS we use an empty initial AU with no commands to signal the RAP*/
			if (sc->objectType == GPAC_OTI_SCENE_DIMS) base_stream_found = 1;

			/*apply all commands - this will also apply the SceneReplace*/
			while (gf_list_count(sc->AUs)) {
				u32 count;
				au = (GF_AUContext *) gf_list_get(sc->AUs, 0);
				gf_list_rem(sc->AUs, 0);

				/*AU not aggregated*/
				if (au->flags & GF_SM_AU_NOT_AGGREGATED) {
					gf_sm_au_del(sc, au);
					continue;
				}

				count = gf_list_count(au->commands);

				for (j=0; j<count; j++) {
					u32 store=0;
					com = gf_list_get(au->commands, j);
					if (!base_stream_found) {
						switch (com->tag) {
						case GF_SG_SCENE_REPLACE:
						case GF_SG_LSR_NEW_SCENE:
						case GF_SG_LSR_REFRESH_SCENE:
							base_stream_found = 1;
							break;
						}
					}

					/*aggregate the command*/

					/*if stream doesn't carry a carousel or carries the base carousel (scene replace), always apply the command*/
					if (base_stream_found || !sc->aggregate_on_esid) {
						store = 0;
					}
					/*otherwise, check wether the command should be kept in this stream as is, or can be aggregated on this stream*/
					else {
						switch (com->tag) {
						/*the following commands do not impact a sub-tree (eg do not deal with nodes), we cannot
						aggregate them... */
						case GF_SG_ROUTE_REPLACE:
						case GF_SG_ROUTE_DELETE:
						case GF_SG_ROUTE_INSERT:
						case GF_SG_PROTO_INSERT:
						case GF_SG_PROTO_DELETE:
						case GF_SG_PROTO_DELETE_ALL:
						case GF_SG_GLOBAL_QUANTIZER:
						case GF_SG_LSR_RESTORE:
						case GF_SG_LSR_SAVE:
						case GF_SG_LSR_SEND_EVENT:
						case GF_SG_LSR_CLEAN:
							/*todo check in which category to put these commands*/
							store = 1;
							break;
						/*other commands:
							!!! we need to know if the target node of the command has been inserted in this stream !!!

						This is a tedious task, for now we will consider the following cases:
							- locate a similar command in the stored list: remove the similar one and aggregate on stream
							- by default all AUs are stored if the stream is in aggregate mode - we should fix that by checking insertion points:
							 if a command apllies on a node that has been inserted in this stream, we can aggregate, otherwise store
						*/
						default:
							/*check if we can directly store the command*/
							assert(carousel_commands);
							store = store_or_aggregate(sc, com, carousel_commands, &has_modif);
							break;
						}
					}

					switch (store) {
					/*command has been merged with a previous command in carousel and needs to be destroyed*/
					case 2:
						gf_list_rem(au->commands, j);
						j--;
						count--;
						gf_sg_command_del((GF_Command *)com);
						break;
					/*command shall be moved to carousel without being applied*/
					case 1:
						gf_list_insert(carousel_commands, com, 0);
						gf_list_rem(au->commands, j);
						j--;
						count--;
						break;
					/*command can be applied*/
					default:
						e = gf_sg_command_apply(ctx->scene_graph, com, 0);
						break;
					}
				}
				gf_sm_au_del(sc, au);
			}

			/*and recreate scene replace*/
			if (base_stream_found) {
				au = gf_sm_stream_au_new(sc, 0, 0, 1);

				switch (sc->objectType) {
				case GPAC_OTI_SCENE_BIFS:
				case GPAC_OTI_SCENE_BIFS_V2:
					com = gf_sg_command_new(ctx->scene_graph, GF_SG_SCENE_REPLACE);
					break;
				case GPAC_OTI_SCENE_LASER:
					com = gf_sg_command_new(ctx->scene_graph, GF_SG_LSR_NEW_SCENE);
					break;
				case GPAC_OTI_SCENE_DIMS:
				/* We do not create a new command, empty AU is enough in DIMS*/
				default:
					com = NULL;
					break;
				}

				if (com) {
					com->node = ctx->scene_graph->RootNode;
					ctx->scene_graph->RootNode = NULL;
					gf_list_del(com->new_proto_list);
					com->new_proto_list = ctx->scene_graph->protos;
					ctx->scene_graph->protos = NULL;
					/*indicate the command is the aggregated scene graph, so that PROTOs and ROUTEs
					are taken from the scenegraph when encoding*/
					com->aggregated = 1;
					gf_list_add(au->commands, com);
				}
			}
			/*update carousel flags of the AU*/
			else if (carousel_commands) {
				/*if current stream caries its own carousel*/
				if (!carousel_au) {
					carousel_au = gf_sm_stream_au_new(sc, 0, 0, 1);
					gf_list_del(carousel_au->commands);
					carousel_au->commands = carousel_commands;
				}
				carousel_au->flags |= GF_SM_AU_RAP | GF_SM_AU_CAROUSEL;
				if (has_modif) carousel_au->flags |= GF_SM_AU_MODIFIED;
			}
		}
#endif
	}
	return e;
}
