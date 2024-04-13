static byte edecrypt(byte cipher)
{
    byte plain;
    if (t1_pfa) {
        while (cipher == 10 || cipher == 13)
            cipher = (byte) t1_getbyte();
        last_hexbyte = cipher = (byte) ((hexval(cipher) << 4) + hexval(t1_getbyte()));
    }
    plain = (byte) (cipher ^ (t1_dr >> 8));
    t1_dr = (unsigned short) ((cipher + t1_dr) * t1_c1 + t1_c2);
    return plain;
}
