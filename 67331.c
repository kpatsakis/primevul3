fptr_finalize(mrb_state *mrb, struct mrb_io *fptr, int quiet)
{
  int saved_errno = 0;

  if (fptr == NULL) {
    return;
  }

  if (fptr->fd > 2) {
#ifdef _WIN32
    if (fptr->is_socket) {
      if (closesocket(fptr->fd) != 0) {
        saved_errno = WSAGetLastError();
      }
      fptr->fd = -1;
    }
#endif
    if (fptr->fd != -1) {
      if (close(fptr->fd) == -1) {
        saved_errno = errno;
      }
    }
    fptr->fd = -1;
  }

  if (fptr->fd2 > 2) {
    if (close(fptr->fd2) == -1) {
      if (saved_errno == 0) {
        saved_errno = errno;
      }
    }
    fptr->fd2 = -1;
  }

  if (fptr->pid != 0) {
#if !defined(_WIN32) && !defined(_WIN64)
    pid_t pid;
    int status;
    do {
      pid = waitpid(fptr->pid, &status, 0);
    } while (pid == -1 && errno == EINTR);
    if (!quiet && pid == fptr->pid) {
      io_set_process_status(mrb, pid, status);
    }
#else
    HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, fptr->pid);
    DWORD status;
    if (WaitForSingleObject(h, INFINITE) && GetExitCodeProcess(h, &status))
      if (!quiet)
        io_set_process_status(mrb, fptr->pid, (int)status);
    CloseHandle(h);
#endif
    fptr->pid = 0;
    /* Note: we don't raise an exception when waitpid(3) fails */
  }

  if (!quiet && saved_errno != 0) {
    errno = saved_errno;
    mrb_sys_fail(mrb, "fptr_finalize failed.");
  }
}
