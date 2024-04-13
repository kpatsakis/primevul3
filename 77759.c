void ClearPendingEmailOnIOThread(content::ResourceContext* context) {
  ProfileIOData* io_data = ProfileIOData::FromResourceContext(context);
  DCHECK(io_data);
  io_data->set_reverse_autologin_pending_email(std::string());
}
