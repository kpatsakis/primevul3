void SandboxedExtensionUnpacker::OnUnpackExtensionFailed(
    const std::string& error) {
  DCHECK(BrowserThread::CurrentlyOn(thread_identifier_));
  got_response_ = true;
  ReportFailure(error);
}
