void SharedMemory::Lock() {
  g_thread_lock_.Get().Acquire();
  LockOrUnlockCommon(F_LOCK);
}
