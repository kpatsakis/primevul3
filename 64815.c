test_bson_append_dbpointer (void)
{
   bson_oid_t oid;
   bson_t *b;
   bson_t *b2;
   uint8_t malformed_data[] = {
      0x0C,
      0x00,
      0x00,
      0x00, /* document length (12) */
      0x0C, /* dbpointer type */
      0x00, /* empty string key */
      0x04,
      0x00,
      0x00,
      0x00, /* string length (4). This is OOB. */
      0x00, /* empty string */
      0x00  /* end of document */
   };
   size_t error_offset = 0;

   b = bson_new ();
   bson_oid_init_from_string (&oid, "0123abcd0123abcd0123abcd");
   BSON_ASSERT (bson_append_dbpointer (b, "dbpointer", -1, "foo", &oid));
   b2 = get_bson ("test28.bson");
   BSON_ASSERT_BSON_EQUAL (b, b2);
   bson_destroy (b);
   bson_destroy (b2);

   b = bson_new_from_data (malformed_data, sizeof (malformed_data));
   BSON_ASSERT (b);
   BSON_ASSERT (!bson_validate (b, BSON_VALIDATE_NONE, &error_offset));
   BSON_ASSERT (error_offset == 6);
   bson_destroy (b);
}
