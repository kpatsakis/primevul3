void ClearNetworkingHistorySinceOnIOThread(
    ProfileImplIOData* io_data, base::Time time) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  io_data->transport_security_state()->DeleteSince(time);
  io_data->http_server_properties()->Clear();
}
