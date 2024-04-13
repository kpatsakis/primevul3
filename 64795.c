bson_iter_overwrite_timestamp (bson_iter_t *iter,
                               uint32_t timestamp,
                               uint32_t increment)
{
   uint64_t value;
   BSON_ASSERT (iter);

   if (ITER_TYPE (iter) == BSON_TYPE_TIMESTAMP) {
      value = ((((uint64_t) timestamp) << 32U) | ((uint64_t) increment));
      value = BSON_UINT64_TO_LE (value);
      memcpy ((void *) (iter->raw + iter->d1), &value, sizeof (value));
   }
}
