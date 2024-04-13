  bool OnHostDomainPolicyUpdate(const std::string& host_domain) {
    DCHECK(context_->network_task_runner()->BelongsToCurrentThread());

    if (!host_domain.empty() &&
        !EndsWith(xmpp_login_, std::string("@") + host_domain, false)) {
      Shutdown(kInvalidHostDomainExitCode);
    }
    return false;
  }
