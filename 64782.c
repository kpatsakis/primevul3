bson_iter_init_from_data_at_offset (bson_iter_t *iter,
                                    const uint8_t *data,
                                    size_t length,
                                    uint32_t offset,
                                    uint32_t keylen)
{
   const char *key;
   uint32_t bson_type;
   bool unsupported;

   BSON_ASSERT (iter);
   BSON_ASSERT (data);

   if (BSON_UNLIKELY ((length < 5) || (length > INT_MAX))) {
      memset (iter, 0, sizeof *iter);
      return false;
   }

   iter->raw = (uint8_t *) data;
   iter->len = (uint32_t) length;
   iter->off = 0;
   iter->type = 0;
   iter->key = 0;
   iter->next_off = offset;
   iter->err_off = 0;

   if (!_bson_iter_next_internal (
          iter, keylen, &key, &bson_type, &unsupported)) {
      memset (iter, 0, sizeof *iter);
      return false;
   }

   return true;
}
