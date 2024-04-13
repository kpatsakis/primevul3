  ResourceRequestTask(int process_id,
                      int render_view_host_id,
                      ResourceRequestAction action)
      : action_(action),
        process_id_(process_id),
        render_view_host_id_(render_view_host_id),
        resource_dispatcher_host_(
            g_browser_process->resource_dispatcher_host()) {
  }
