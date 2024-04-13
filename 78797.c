  static int TaskTestMain() {
    int errors = 0;
#if defined(OS_MACOSX)
    mac::ScopedNSAutoreleasePool pool;
#endif
    const uint32 kDataSize = 1024;
    SharedMemory memory;
    bool rv = memory.CreateNamed(s_test_name_, true, kDataSize);
    EXPECT_TRUE(rv);
    if (rv != true)
      errors++;
    rv = memory.Map(kDataSize);
    EXPECT_TRUE(rv);
    if (rv != true)
      errors++;
    int *ptr = static_cast<int*>(memory.memory());

    for (int idx = 0; idx < 20; idx++) {
      memory.Lock();
      int i = (1 << 16) + idx;
      *ptr = i;
      PlatformThread::Sleep(TimeDelta::FromMilliseconds(10));
      if (*ptr != i)
        errors++;
      memory.Unlock();
    }

    memory.Close();
    return errors;
  }
