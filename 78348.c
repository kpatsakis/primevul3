    RenderThreadImpl::HostAllocateSharedMemoryBuffer(size_t size) {
  if (size > static_cast<size_t>(std::numeric_limits<int>::max()))
    return scoped_ptr<base::SharedMemory>();

  base::SharedMemoryHandle handle;
  bool success;
  IPC::Message* message =
      new ChildProcessHostMsg_SyncAllocateSharedMemory(size, &handle);

  if (base::MessageLoop::current() == message_loop())
    success = ChildThread::Send(message);
  else
    success = sync_message_filter()->Send(message);

  if (!success)
    return scoped_ptr<base::SharedMemory>();

  if (!base::SharedMemory::IsHandleValid(handle))
    return scoped_ptr<base::SharedMemory>();

  return scoped_ptr<base::SharedMemory>(new base::SharedMemory(handle, false));
}
