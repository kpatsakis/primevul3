SyncManager::Status::Status()
    : summary(INVALID),
      authenticated(false),
      server_up(false),
      server_reachable(false),
      notifications_enabled(false),
      notifications_received(0),
      unsynced_count(0),
      conflicting_count(0),
      committed_count(0),
      syncing(false),
      initial_sync_ended(false),
      updates_available(0),
      updates_received(0),
      tombstone_updates_received(0),
      num_local_overwrites_total(0),
      num_server_overwrites_total(0),
      nonempty_get_updates(0),
      empty_get_updates(0),
      sync_cycles_with_commits(0),
      sync_cycles_without_commits(0),
      useless_sync_cycles(0),
      useful_sync_cycles(0),
      cryptographer_ready(false),
      crypto_has_pending_keys(false) {
}
