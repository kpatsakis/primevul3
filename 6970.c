TEST_P(SslReadBufferLimitTest, SomeLimit) {
  readBufferLimitTest(32 * 1024, 32 * 1024, 256 * 1024, 1, false);
}