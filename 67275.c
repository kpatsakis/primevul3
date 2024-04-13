static int __init etm_perf_init(void)
{
	int ret;

	etm_pmu.capabilities		= PERF_PMU_CAP_EXCLUSIVE;

	etm_pmu.attr_groups		= etm_pmu_attr_groups;
	etm_pmu.task_ctx_nr		= perf_sw_context;
	etm_pmu.read			= etm_event_read;
	etm_pmu.event_init		= etm_event_init;
	etm_pmu.setup_aux		= etm_setup_aux;
	etm_pmu.free_aux		= etm_free_aux;
	etm_pmu.start			= etm_event_start;
	etm_pmu.stop			= etm_event_stop;
	etm_pmu.add			= etm_event_add;
	etm_pmu.del			= etm_event_del;
	etm_pmu.addr_filters_sync	= etm_addr_filters_sync;
	etm_pmu.addr_filters_validate	= etm_addr_filters_validate;
	etm_pmu.nr_addr_filters		= ETM_ADDR_CMP_MAX;

	ret = perf_pmu_register(&etm_pmu, CORESIGHT_ETM_PMU_NAME, -1);
	if (ret == 0)
		etm_perf_up = true;

	return ret;
}
