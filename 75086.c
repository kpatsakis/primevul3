  virtual void Run() {
    switch (action_) {
      case BLOCK:
        resource_dispatcher_host_->BlockRequestsForRoute(
            process_id_, render_view_host_id_);
        break;
      case RESUME:
        resource_dispatcher_host_->ResumeBlockedRequestsForRoute(
            process_id_, render_view_host_id_);
        break;
      case CANCEL:
        resource_dispatcher_host_->CancelBlockedRequestsForRoute(
            process_id_, render_view_host_id_);
        break;
      default:
        NOTREACHED();
    }
  }
