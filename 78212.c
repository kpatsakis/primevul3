void EnsureCreateFIFO(const base::FilePath& path) {
  unlink(path.value().c_str());
  CHECK(base::android::CreateFIFO(path, 0666))
    << "Unable to create the Android's FIFO: " << path.value().c_str();
}
