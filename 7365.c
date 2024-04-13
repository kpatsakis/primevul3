static void mz_zip_reader_sort_central_dir_offsets_by_filename(
    mz_zip_archive *pZip) {
  mz_zip_internal_state *pState = pZip->m_pState;
  const mz_zip_array *pCentral_dir_offsets = &pState->m_central_dir_offsets;
  const mz_zip_array *pCentral_dir = &pState->m_central_dir;
  mz_uint32 *pIndices = &MZ_ZIP_ARRAY_ELEMENT(
      &pState->m_sorted_central_dir_offsets, mz_uint32, 0);
  const int size = pZip->m_total_files;
  int start = (size - 2) >> 1, end;
  while (start >= 0) {
    int child, root = start;
    for (;;) {
      if ((child = (root << 1) + 1) >= size) break;
      child +=
          (((child + 1) < size) &&
           (mz_zip_reader_filename_less(pCentral_dir, pCentral_dir_offsets,
                                        pIndices[child], pIndices[child + 1])));
      if (!mz_zip_reader_filename_less(pCentral_dir, pCentral_dir_offsets,
                                       pIndices[root], pIndices[child]))
        break;
      MZ_SWAP_UINT32(pIndices[root], pIndices[child]);
      root = child;
    }
    start--;
  }

  end = size - 1;
  while (end > 0) {
    int child, root = 0;
    MZ_SWAP_UINT32(pIndices[end], pIndices[0]);
    for (;;) {
      if ((child = (root << 1) + 1) >= end) break;
      child +=
          (((child + 1) < end) &&
           mz_zip_reader_filename_less(pCentral_dir, pCentral_dir_offsets,
                                       pIndices[child], pIndices[child + 1]));
      if (!mz_zip_reader_filename_less(pCentral_dir, pCentral_dir_offsets,
                                       pIndices[root], pIndices[child]))
        break;
      MZ_SWAP_UINT32(pIndices[root], pIndices[child]);
      root = child;
    }
    end--;
  }
}