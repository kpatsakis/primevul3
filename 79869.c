SavePackage::SavePackage(WebContents* web_contents,
                         SavePageType save_type,
                         const FilePath& file_full_path,
                         const FilePath& directory_full_path)
    : WebContentsObserver(web_contents),
      file_manager_(NULL),
      download_manager_(NULL),
      download_(NULL),
      page_url_(GetUrlToBeSaved()),
      saved_main_file_path_(file_full_path),
      saved_main_directory_path_(directory_full_path),
      title_(web_contents->GetTitle()),
      start_tick_(base::TimeTicks::Now()),
      finished_(false),
      mhtml_finishing_(false),
      user_canceled_(false),
      disk_error_occurred_(false),
      save_type_(save_type),
      all_save_items_count_(0),
      file_name_set_(&FilePath::CompareLessIgnoreCase),
      wait_state_(INITIALIZE),
      contents_id_(web_contents->GetRenderProcessHost()->GetID()),
      unique_id_(g_save_package_id++),
      wrote_to_completed_file_(false),
      wrote_to_failed_file_(false) {
  DCHECK(page_url_.is_valid());
  DCHECK((save_type_ == SAVE_PAGE_TYPE_AS_ONLY_HTML) ||
         (save_type_ == SAVE_PAGE_TYPE_AS_MHTML) ||
         (save_type_ == SAVE_PAGE_TYPE_AS_COMPLETE_HTML));
  DCHECK(!saved_main_file_path_.empty() &&
         saved_main_file_path_.value().length() <= kMaxFilePathLength);
  DCHECK(!saved_main_directory_path_.empty() &&
         saved_main_directory_path_.value().length() < kMaxFilePathLength);
  InternalInit();
}
