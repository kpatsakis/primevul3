void RenderThreadImpl::OnTempCrashWithData(const GURL& data) {
  GetContentClient()->SetActiveURL(data);
  CHECK(false);
}
