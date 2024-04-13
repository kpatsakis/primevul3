static byte cdecrypt(byte cipher, unsigned short *cr)
{
    const byte plain = (cipher ^ (*cr >> 8));
    *cr = (cipher + *cr) * t1_c1 + t1_c2;
    return plain;
}
