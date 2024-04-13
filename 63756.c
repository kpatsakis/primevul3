static byte cencrypt(byte plain, unsigned short *cr)
{
    const byte cipher = (plain ^ (*cr >> 8));
    *cr = (cipher + *cr) * t1_c1 + t1_c2;
    return cipher;
}
