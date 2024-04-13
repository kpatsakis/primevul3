  static void TerminateWorkerOnIOThread(scoped_refptr<WorkerData> worker_data) {
    if (WorkerService::GetInstance()->TerminateWorker(
            worker_data->worker_process_id, worker_data->worker_route_id)) {
      WorkerService::GetInstance()->AddObserver(
          new WorkerTerminationObserver(worker_data.get()));
      return;
    }
    FAIL() << "Failed to terminate worker.\n";
  }
