static byte cencrypt(byte plain, unsigned short *cr)
{
    const byte cipher = (byte) (plain ^ (*cr >> 8));
    *cr = (unsigned short) ((cipher + *cr) * t1_c1 + t1_c2);
    return cipher;
}
