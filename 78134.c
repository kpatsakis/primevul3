void WebContentsImpl::OnEnumerateDirectory(int request_id,
                                           const base::FilePath& path) {
  if (!delegate_)
    return;

  ChildProcessSecurityPolicyImpl* policy =
      ChildProcessSecurityPolicyImpl::GetInstance();
  if (policy->CanReadDirectory(GetRenderProcessHost()->GetID(), path))
    delegate_->EnumerateDirectory(this, request_id, path);
}
