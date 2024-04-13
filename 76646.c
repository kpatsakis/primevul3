ChildProcessSecurityPolicyImpl::~ChildProcessSecurityPolicyImpl() {
  web_safe_schemes_.clear();
  pseudo_schemes_.clear();
  STLDeleteContainerPairSecondPointers(security_state_.begin(),
                                       security_state_.end());
  security_state_.clear();
}
