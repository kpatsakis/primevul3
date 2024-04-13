bool SandboxedExtensionUnpacker::RewriteImageFiles() {
  ExtensionUnpacker::DecodedImages images;
  if (!ExtensionUnpacker::ReadImagesFromFile(temp_dir_.path(), &images)) {
    ReportFailure("Couldn't read image data from disk.");
    return false;
  }

  std::set<FilePath> image_paths = extension_->GetBrowserImages();
  if (image_paths.size() != images.size()) {
    ReportFailure("Decoded images don't match what's in the manifest.");
    return false;
  }

  for (std::set<FilePath>::iterator it = image_paths.begin();
       it != image_paths.end(); ++it) {
    FilePath path = *it;
    if (path.IsAbsolute() || path.ReferencesParent()) {
      ReportFailure("Invalid path for browser image.");
      return false;
    }
    if (!file_util::Delete(extension_root_.Append(path), false)) {
      ReportFailure("Error removing old image file.");
      return false;
    }
  }

  for (size_t i = 0; i < images.size(); ++i) {
    const SkBitmap& image = images[i].a;
    FilePath path_suffix = images[i].b;
    if (path_suffix.IsAbsolute() || path_suffix.ReferencesParent()) {
      ReportFailure("Invalid path for bitmap image.");
      return false;
    }
    FilePath path = extension_root_.Append(path_suffix);

    std::vector<unsigned char> image_data;
    if (!gfx::PNGCodec::EncodeBGRASkBitmap(image, false, &image_data)) {
      ReportFailure("Error re-encoding theme image.");
      return false;
    }

    const char* image_data_ptr = reinterpret_cast<const char*>(&image_data[0]);
    if (!file_util::WriteFile(path, image_data_ptr, image_data.size())) {
      ReportFailure("Error saving theme image.");
      return false;
    }
  }

  return true;
}
