static byte eencrypt(byte plain)
{
    const byte cipher = (plain ^ (t1_er >> 8));
    t1_er = (cipher + t1_er) * t1_c1 + t1_c2;
    return cipher;
}
