test_bson_iter_init_from_data_at_offset (void)
{
   bson_t *bson = bson_with_all_types ();
   /* zero out iter, since bson_iter_init doesn't zero out iter->value. */
   bson_iter_t iter = {0};

   BSON_ASSERT (bson_iter_init (&iter, bson));
   ASSERT_CMPINT (bson_iter_offset (&iter), ==, 0);
   while (bson_iter_next (&iter)) {
      const uint8_t *data = bson_get_data (bson);
      int keylen = bson_iter_key_len (&iter);
      uint32_t offset = bson_iter_offset (&iter);
      bson_iter_t recreated = {0};

      BSON_ASSERT (bson_iter_init_from_data_at_offset (
         &recreated, data, bson->len, offset, keylen));
      if (memcmp ((void *) &iter, (void *) &recreated, sizeof (bson_iter_t)) !=
          0) {
         int i;
         bson_iter_t *iters[] = {&iter, &recreated};
         fprintf (stderr,
                  "recreated iterator does not match initial iterator:\n");
         for (i = 0; i < 2; i++) {
            fprintf (stderr, "iter %d: ", i);
            fprintf (stderr,
                     "len=%d, off=%d, type=%d, key=%d, d1=%d, d2=%d, "
                     "d3=%d, d4=%d, next_off=%d, err_off=%d\n",
                     iters[i]->len,
                     iters[i]->off,
                     iters[i]->type,
                     iters[i]->key,
                     iters[i]->d1,
                     iters[i]->d2,
                     iters[i]->d3,
                     iters[i]->d4,
                     iters[i]->next_off,
                     iters[i]->err_off);
         }
         ASSERT (false);
      }
   }
}
