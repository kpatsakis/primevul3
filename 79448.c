  void OnPolicyUpdate(scoped_ptr<base::DictionaryValue> policies) {
    if (!context_->network_task_runner()->BelongsToCurrentThread()) {
      context_->network_task_runner()->PostTask(FROM_HERE, base::Bind(
          &HostProcess::OnPolicyUpdate, base::Unretained(this),
          base::Passed(&policies)));
      return;
    }

    bool restart_required = false;
    bool bool_value;
    std::string string_value;
    if (policies->GetString(policy_hack::PolicyWatcher::kHostDomainPolicyName,
                            &string_value)) {
      restart_required |= OnHostDomainPolicyUpdate(string_value);
    }
    if (policies->GetBoolean(policy_hack::PolicyWatcher::kNatPolicyName,
                             &bool_value)) {
      restart_required |= OnNatPolicyUpdate(bool_value);
    }
    if (policies->GetString(
            policy_hack::PolicyWatcher::kHostTalkGadgetPrefixPolicyName,
            &string_value)) {
      restart_required |= OnHostTalkGadgetPrefixPolicyUpdate(string_value);
    }
    if (policies->GetBoolean(
            policy_hack::PolicyWatcher::kHostRequireCurtainPolicyName,
            &bool_value)) {
      restart_required |= OnCurtainPolicyUpdate(bool_value);
    }
    if (!host_) {
      StartHost();
    } else if (restart_required) {
      RestartHost();
    }
  }
