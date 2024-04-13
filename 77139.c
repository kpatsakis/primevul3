void MediaStreamImpl::OnVideoDeviceFailed(const std::string& label,
                                          int index) {
  DCHECK(CalledOnValidThread());
  DVLOG(1) << "MediaStreamImpl::OnVideoDeviceFailed("
           << label << ", " << index << ")";
  NOTIMPLEMENTED();
}
