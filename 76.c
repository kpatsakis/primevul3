  virtual void RemoveCellularDataPlanObserver(
      CellularDataPlanObserver* observer) {
    data_plan_observers_.RemoveObserver(observer);
  }
