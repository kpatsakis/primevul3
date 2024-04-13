void DeleteFile(const FilePath& file_path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  file_util::Delete(file_path, false);
}
