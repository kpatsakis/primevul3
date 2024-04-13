void SandboxedExtensionUnpacker::StartProcessOnIOThread(
    const FilePath& temp_crx_path) {
  UtilityProcessHost* host = new UtilityProcessHost(
      rdh_, this, thread_identifier_);
  host->StartExtensionUnpacker(temp_crx_path);
}
