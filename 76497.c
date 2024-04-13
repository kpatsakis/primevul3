void SandboxedExtensionUnpacker::ReportSuccess() {
  client_->OnUnpackSuccess(temp_dir_.Take(), extension_root_, extension_);
  extension_ = NULL;
}
