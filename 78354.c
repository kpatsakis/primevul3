void RenderThreadImpl::NotifyTimezoneChange() {
  NotifyTimezoneChangeOnThisThread();
  RenderThread::Get()->PostTaskToAllWebWorkers(
      base::Bind(&NotifyTimezoneChangeOnThisThread));
}
