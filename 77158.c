void NetworkActionPredictor::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  switch (type) {
    case chrome::NOTIFICATION_HISTORY_URLS_DELETED: {
      DCHECK(initialized_);
      const content::Details<const history::URLsDeletedDetails>
          urls_deleted_details =
              content::Details<const history::URLsDeletedDetails>(details);
      if (urls_deleted_details->all_history)
        DeleteAllRows();
      else
        DeleteRowsWithURLs(urls_deleted_details->urls);
      break;
    }

    case chrome::NOTIFICATION_OMNIBOX_OPENED_URL: {
      DCHECK(initialized_);

      OnOmniboxOpenedUrl(*content::Details<AutocompleteLog>(details).ptr());
      break;
    }

    case chrome::NOTIFICATION_HISTORY_LOADED: {
      DCHECK(!initialized_);
      TryDeleteOldEntries(content::Details<HistoryService>(details).ptr());

      notification_registrar_.Remove(this,
                                     chrome::NOTIFICATION_HISTORY_LOADED,
                                     content::Source<Profile>(profile_));
      break;
    }

    default:
      NOTREACHED() << "Unexpected notification observed.";
      break;
  }
}
