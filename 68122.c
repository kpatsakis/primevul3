WriteFormattedPix(char  *fname,
                  PIX   *pix)
{
l_int32  d;

    d = pixGetDepth(pix);
    if (d == 1 || pixGetColormap(pix))
        pixWrite(fname, pix, IFF_PNG);
    else
        pixWrite(fname, pix, IFF_JFIF_JPEG);
    return;
}
