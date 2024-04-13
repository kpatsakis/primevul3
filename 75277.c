   virtual void Run() {
    while (true) {
      fd_set rfds;
      FD_ZERO(&rfds);
      FD_SET(inotify_fd_, &rfds);
      FD_SET(shutdown_fd_, &rfds);

      int select_result =
        HANDLE_EINTR(select(std::max(inotify_fd_, shutdown_fd_) + 1,
                            &rfds, NULL, NULL, NULL));
      if (select_result < 0) {
        DPLOG(WARNING) << "select failed";
        return;
      }

      if (FD_ISSET(shutdown_fd_, &rfds))
        return;

      int buffer_size;
      int ioctl_result = HANDLE_EINTR(ioctl(inotify_fd_, FIONREAD,
                                            &buffer_size));

      if (ioctl_result != 0) {
        DPLOG(WARNING) << "ioctl failed";
        return;
      }

      std::vector<char> buffer(buffer_size);

      ssize_t bytes_read = HANDLE_EINTR(read(inotify_fd_, &buffer[0],
                                             buffer_size));

      if (bytes_read < 0) {
        DPLOG(WARNING) << "read from inotify fd failed";
        return;
      }

      ssize_t i = 0;
      while (i < bytes_read) {
        inotify_event* event = reinterpret_cast<inotify_event*>(&buffer[i]);
        size_t event_size = sizeof(inotify_event) + event->len;
        DCHECK(i + event_size <= static_cast<size_t>(bytes_read));
        reader_->OnInotifyEvent(event);
        i += event_size;
      }
    }
  }
