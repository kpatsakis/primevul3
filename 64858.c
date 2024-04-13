test_bson_regex_lengths (void)
{
   bson_t new = BSON_INITIALIZER;
   bson_oid_t oid;

   bson_oid_init_from_string (&oid, "1234567890abcdef12345678");
   bson_append_oid (&new, "0123456", -1, &oid);

   bson_append_regex (&new,
                      "0_________1_________2_________3___4",
                      -1,
                      "0_________1_________2_________3_________4_________5___4",
                      "i");

   ASSERT (new.len == 121);
   ASSERT (new.flags &BSON_FLAG_STATIC);
   ASSERT (!(new.flags &BSON_FLAG_INLINE));

   bson_destroy (&new);
}
