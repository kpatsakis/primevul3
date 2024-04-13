void DecodeIPV6RegisterTests(void)
{
#ifdef UNITTESTS
    UtRegisterTest("DecodeIPV6FragTest01", DecodeIPV6FragTest01);
    UtRegisterTest("DecodeIPV6RouteTest01", DecodeIPV6RouteTest01);
    UtRegisterTest("DecodeIPV6HopTest01", DecodeIPV6HopTest01);
#endif /* UNITTESTS */
}
