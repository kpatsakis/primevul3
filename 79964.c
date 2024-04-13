  static void WaitForFirstSharedWorkerOnIOThread(
      scoped_refptr<WorkerData> worker_data) {
    std::vector<WorkerService::WorkerInfo> worker_info =
        WorkerService::GetInstance()->GetWorkers();
    if (!worker_info.empty()) {
      worker_data->worker_process_id = worker_info[0].process_id;
      worker_data->worker_route_id = worker_info[0].route_id;
      BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
          MessageLoop::QuitClosure());
      return;
    }

    WorkerService::GetInstance()->AddObserver(
        new WorkerCreationObserver(worker_data.get()));
  }
