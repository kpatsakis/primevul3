void MediaStreamImpl::OnAudioDeviceFailed(const std::string& label,
                                          int index) {
  DCHECK(CalledOnValidThread());
  DVLOG(1) << "MediaStreamImpl::OnAudioDeviceFailed("
           << label << ", " << index << ")";
  NOTIMPLEMENTED();
}
