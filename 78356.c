void RenderThreadImpl::OnCreateNewSharedWorker(
    const WorkerProcessMsg_CreateWorker_Params& params) {
  new EmbeddedSharedWorkerStub(params.url,
                               params.name,
                               params.content_security_policy,
                               params.security_policy_type,
                               params.pause_on_start,
                               params.route_id);
}
