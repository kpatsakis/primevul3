  bool OnHostTalkGadgetPrefixPolicyUpdate(
      const std::string& talkgadget_prefix) {
    DCHECK(context_->network_task_runner()->BelongsToCurrentThread());

    if (talkgadget_prefix != talkgadget_prefix_) {
      LOG(INFO) << "Updated talkgadget policy.";
      talkgadget_prefix_ = talkgadget_prefix;
      return true;
    }
    return false;
  }
