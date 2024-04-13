    WatchEntry(InotifyReader::Watch watch, const FilePath::StringType& subdir)
        : watch_(watch),
          subdir_(subdir) {}
