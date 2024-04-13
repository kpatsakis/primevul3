SandboxedExtensionUnpacker::SandboxedExtensionUnpacker(
    const FilePath& crx_path,
    const FilePath& temp_path,
    ResourceDispatcherHost* rdh,
    SandboxedExtensionUnpackerClient* client)
    : crx_path_(crx_path), temp_path_(temp_path),
      thread_identifier_(BrowserThread::ID_COUNT),
      rdh_(rdh), client_(client), got_response_(false) {
}
