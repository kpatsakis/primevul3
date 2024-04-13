int RenderThreadImpl::PostTaskToAllWebWorkers(const base::Closure& closure) {
  return WorkerTaskRunner::Instance()->PostTaskToAllThreads(closure);
}
