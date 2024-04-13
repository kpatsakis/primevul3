void gf_fs_del(GF_FilterSession *fsess)
{
	assert(fsess);

	gf_fs_stop(fsess);
	GF_LOG(GF_LOG_DEBUG, GF_LOG_FILTER, ("Session destroy begin\n"));

	if (fsess->parsed_args) {
		while (gf_list_count(fsess->parsed_args)) {
			GF_FSArgItem *ai = gf_list_pop_back(fsess->parsed_args);
			gf_free(ai->argname);
			gf_free(ai);
		}
		gf_list_del(fsess->parsed_args);
	}

	//temporary until we don't introduce fsess_stop
	assert(fsess->run_status != GF_OK);
	if (fsess->filters) {
		u32 i, count=gf_list_count(fsess->filters);
		//first pass: disconnect all filters, since some may have references to property maps or packets 
		for (i=0; i<count; i++) {
			u32 j;
			GF_Filter *filter = gf_list_get(fsess->filters, i);
			filter->process_th_id = 0;
			filter->scheduled_for_next_task = GF_TRUE;

			if (filter->detached_pid_inst) {
				while (gf_list_count(filter->detached_pid_inst)) {
					GF_FilterPidInst *pidi = gf_list_pop_front(filter->detached_pid_inst);
					gf_filter_pid_inst_del(pidi);
				}
				gf_list_del(filter->detached_pid_inst);
				filter->detached_pid_inst = NULL;
			}

			if (filter->postponed_packets) {
				while (gf_list_count(filter->postponed_packets)) {
					GF_FilterPacket *pck = gf_list_pop_front(filter->postponed_packets);
					gf_filter_packet_destroy(pck);
				}
				gf_list_del(filter->postponed_packets);
				filter->postponed_packets = NULL;
			}
			gf_mx_p(filter->tasks_mx);
			for (j=0; j<filter->num_input_pids; j++) {
				GF_FilterPidInst *pidi = gf_list_get(filter->input_pids, j);
				gf_filter_pid_inst_reset(pidi);
			}
			gf_mx_v(filter->tasks_mx);
			filter->scheduled_for_next_task = GF_FALSE;
		}
		//second pass, finalize all
		for (i=0; i<count; i++) {
			GF_Filter *filter = gf_list_get(fsess->filters, i);
			if (filter->freg->finalize && !filter->finalized) {
				filter->finalized = GF_TRUE;
				FSESS_CHECK_THREAD(filter)
				filter->freg->finalize(filter);
			}
		}

		while (gf_list_count(fsess->filters)) {
			GF_Filter *filter = gf_list_pop_back(fsess->filters);

			gf_filter_del(filter);
		}
		gf_list_del(fsess->filters);
		fsess->filters = NULL;
	}

	gf_fs_unload_script(fsess, NULL);

	if (fsess->download_manager) gf_dm_del(fsess->download_manager);
#ifndef GPAC_DISABLE_PLAYER
	if (fsess->font_manager) gf_font_manager_del(fsess->font_manager);
#endif

	if (fsess->registry) {
		while (gf_list_count(fsess->registry)) {
			GF_FilterRegister *freg = gf_list_pop_back(fsess->registry);
			if (freg->register_free) freg->register_free(fsess, freg);
		}
		gf_list_del(fsess->registry);
	}

	if (fsess->tasks)
		gf_fq_del(fsess->tasks, gf_void_del);

	if (fsess->tasks_reservoir)
		gf_fq_del(fsess->tasks_reservoir, gf_void_del);

	if (fsess->threads) {
		if (fsess->main_thread_tasks)
			gf_fq_del(fsess->main_thread_tasks, gf_void_del);

		while (gf_list_count(fsess->threads)) {
			GF_SessionThread *sess_th = gf_list_pop_back(fsess->threads);
			gf_th_del(sess_th->th);
			gf_free(sess_th);
		}
		gf_list_del(fsess->threads);
	}

	if (fsess->prop_maps_reservoir)
		gf_fq_del(fsess->prop_maps_reservoir, gf_propmap_del);
#if GF_PROPS_HASHTABLE_SIZE
	if (fsess->prop_maps_list_reservoir)
		gf_fq_del(fsess->prop_maps_list_reservoir, (gf_destruct_fun) gf_list_del);
#endif
	if (fsess->prop_maps_entry_reservoir)
		gf_fq_del(fsess->prop_maps_entry_reservoir, gf_void_del);
	if (fsess->prop_maps_entry_data_alloc_reservoir)
		gf_fq_del(fsess->prop_maps_entry_data_alloc_reservoir, gf_propalloc_del);
	if (fsess->pcks_refprops_reservoir)
		gf_fq_del(fsess->pcks_refprops_reservoir, gf_void_del);


	if (fsess->props_mx)
		gf_mx_del(fsess->props_mx);

	if (fsess->info_mx)
		gf_mx_del(fsess->info_mx);

	if (fsess->ui_mx)
		gf_mx_del(fsess->ui_mx);

	if (fsess->semaphore_other && (fsess->semaphore_other != fsess->semaphore_main) )
		gf_sema_del(fsess->semaphore_other);

	if (fsess->semaphore_main)
		gf_sema_del(fsess->semaphore_main);

	if (fsess->tasks_mx)
		gf_mx_del(fsess->tasks_mx);

	if (fsess->filters_mx)
		gf_mx_del(fsess->filters_mx);

	if (fsess->evt_mx) gf_mx_del(fsess->evt_mx);
	if (fsess->event_listeners) gf_list_del(fsess->event_listeners);

	if (fsess->links) {
		gf_filter_sess_reset_graph(fsess, NULL);
		gf_list_del(fsess->links);
	}
	if (fsess->links_mx) gf_mx_del(fsess->links_mx);

#ifndef GPAC_DISABLE_3D
	gf_list_del(fsess->gl_providers);
	if (fsess->gl_driver) {
		fsess->gl_driver->Shutdown(fsess->gl_driver);
		gf_modules_close_interface((GF_BaseInterface *)fsess->gl_driver);
	}
#endif

	if (fsess->auto_inc_nums) {
		while(gf_list_count(fsess->auto_inc_nums)) {
			GF_FSAutoIncNum *aint = gf_list_pop_back(fsess->auto_inc_nums);
			gf_free(aint);
		}
		gf_list_del(fsess->auto_inc_nums);
	}

#ifdef GF_FS_ENABLE_LOCALES
	if (fsess->uri_relocators) gf_list_del(fsess->uri_relocators);
	if (fsess->locales.szAbsRelocatedPath) gf_free(fsess->locales.szAbsRelocatedPath);
#endif

	gf_free(fsess);
	GF_LOG(GF_LOG_DEBUG, GF_LOG_FILTER, ("Session destroyed\n"));
}