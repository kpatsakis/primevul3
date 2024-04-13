bson_iter_overwrite_oid (bson_iter_t *iter, const bson_oid_t *value)
{
   BSON_ASSERT (iter);

   if (ITER_TYPE (iter) == BSON_TYPE_OID) {
      memcpy (
         (void *) (iter->raw + iter->d1), value->bytes, sizeof (value->bytes));
   }
}
