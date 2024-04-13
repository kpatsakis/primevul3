static byte cdecrypt(byte cipher, unsigned short *cr)
{
    const byte plain = (byte) (cipher ^ (*cr >> 8));
    *cr = (unsigned short) ((cipher + *cr) * t1_c1 + t1_c2);
    return plain;
}
