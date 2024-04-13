  NetworkLibraryImpl()
      : network_manager_monitor_(NULL),
        data_plan_monitor_(NULL),
        ethernet_(NULL),
        wifi_(NULL),
        cellular_(NULL),
        available_devices_(0),
        enabled_devices_(0),
        connected_devices_(0),
        offline_mode_(false) {
    if (EnsureCrosLoaded()) {
      Init();
      network_manager_monitor_ =
          MonitorNetworkManager(&NetworkManagerStatusChangedHandler,
                                this);
      data_plan_monitor_ = MonitorCellularDataPlan(&DataPlanUpdateHandler,
                                                   this);
    } else {
      InitTestData();
    }
  }
