void SharedMemory::Unlock() {
  LockOrUnlockCommon(F_ULOCK);
  g_thread_lock_.Get().Release();
}
