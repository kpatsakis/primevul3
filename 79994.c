void RenderViewImpl::GetWindowSnapshot(const WindowSnapshotCallback& callback) {
  int id = next_snapshot_id_++;
  pending_snapshots_.insert(std::make_pair(id, callback));
  Send(new ViewHostMsg_GetWindowSnapshot(routing_id_, id));
}
