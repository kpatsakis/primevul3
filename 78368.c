void RenderThreadImpl::RecordAction(const base::UserMetricsAction& action) {
  Send(new ViewHostMsg_UserMetricsRecordAction(action.str_));
}
