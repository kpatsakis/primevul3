TIFFCvtIEEEFloatToNative(TIFF* tif, u_int n, float* f)
{
	float_t* fp = (float_t*) f;

	while (n-- > 0) {
		IEEEFLOAT2NATIVE(fp);
		fp++;
	}
}
