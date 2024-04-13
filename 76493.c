void SandboxedExtensionUnpacker::OnProcessCrashed() {
  if (got_response_)
    return;

  ReportFailure("Utility process crashed while trying to install.");
}
