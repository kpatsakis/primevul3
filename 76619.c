ChildProcessSecurityPolicyImpl* ChildProcessSecurityPolicyImpl::GetInstance() {
  return Singleton<ChildProcessSecurityPolicyImpl>::get();
}
