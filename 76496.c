void SandboxedExtensionUnpacker::ReportFailure(const std::string& error) {
  client_->OnUnpackFailure(error);
}
