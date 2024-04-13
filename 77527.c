void SyncManager::SyncInternal::OnIncomingNotification(
    const syncable::ModelTypePayloadMap& type_payloads,
    sync_notifier::IncomingNotificationSource source) {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (source == sync_notifier::LOCAL_NOTIFICATION) {
    if (scheduler()) {
      scheduler()->ScheduleNudgeWithPayloads(
          TimeDelta::FromMilliseconds(kSyncRefreshDelayMsec),
          browser_sync::NUDGE_SOURCE_LOCAL_REFRESH,
          type_payloads, FROM_HERE);
    }
  } else if (!type_payloads.empty()) {
    if (scheduler()) {
      scheduler()->ScheduleNudgeWithPayloads(
          TimeDelta::FromMilliseconds(kSyncSchedulerDelayMsec),
          browser_sync::NUDGE_SOURCE_NOTIFICATION,
          type_payloads, FROM_HERE);
    }
    allstatus_.IncrementNotificationsReceived();
    UpdateNotificationInfo(type_payloads);
  } else {
    LOG(WARNING) << "Sync received notification without any type information.";
  }

  if (js_event_handler_.IsInitialized()) {
    DictionaryValue details;
    ListValue* changed_types = new ListValue();
    details.Set("changedTypes", changed_types);
    for (syncable::ModelTypePayloadMap::const_iterator
             it = type_payloads.begin();
         it != type_payloads.end(); ++it) {
      const std::string& model_type_str =
          syncable::ModelTypeToString(it->first);
      changed_types->Append(Value::CreateStringValue(model_type_str));
    }
    details.SetString("source", (source == sync_notifier::LOCAL_NOTIFICATION) ?
        "LOCAL_NOTIFICATION" : "REMOTE_NOTIFICATION");
    js_event_handler_.Call(FROM_HERE,
                           &JsEventHandler::HandleJsEvent,
                           "onIncomingNotification",
                           JsEventDetails(&details));
  }
}
