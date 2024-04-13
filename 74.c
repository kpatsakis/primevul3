  virtual void RefreshCellularDataPlans(const CellularNetwork* network) {
    DCHECK(network);
    if (!EnsureCrosLoaded() || !network)
      return;
    RequestCellularDataPlanUpdate(network->service_path().c_str());
  }
