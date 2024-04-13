  LoginUtils* get() {
    AutoLock create(create_lock_);
    if (!ptr_.get())
      reset(new LoginUtilsImpl);
    return ptr_.get();
  }
