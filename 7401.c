void *mz_zip_reader_extract_to_heap(mz_zip_archive *pZip, mz_uint file_index,
                                    size_t *pSize, mz_uint flags) {
  mz_uint64 comp_size, uncomp_size, alloc_size;
  const mz_uint8 *p = mz_zip_reader_get_cdh(pZip, file_index);
  void *pBuf;

  if (pSize) *pSize = 0;
  if (!p) return NULL;

  comp_size = MZ_READ_LE32(p + MZ_ZIP_CDH_COMPRESSED_SIZE_OFS);
  uncomp_size = MZ_READ_LE32(p + MZ_ZIP_CDH_DECOMPRESSED_SIZE_OFS);

  alloc_size = (flags & MZ_ZIP_FLAG_COMPRESSED_DATA) ? comp_size : uncomp_size;
#ifdef _MSC_VER
  if (((0, sizeof(size_t) == sizeof(mz_uint32))) && (alloc_size > 0x7FFFFFFF))
#else
  if (((sizeof(size_t) == sizeof(mz_uint32))) && (alloc_size > 0x7FFFFFFF))
#endif
    return NULL;
  if (NULL ==
      (pBuf = pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, (size_t)alloc_size)))
    return NULL;

  if (!mz_zip_reader_extract_to_mem(pZip, file_index, pBuf, (size_t)alloc_size,
                                    flags)) {
    pZip->m_pFree(pZip->m_pAlloc_opaque, pBuf);
    return NULL;
  }

  if (pSize) *pSize = (size_t)alloc_size;
  return pBuf;
}