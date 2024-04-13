BGD_DECLARE(gdIOCtx *) gdNewDynamicCtx(int initialSize, void *data)
{
	/* 2.0.23: Phil Moore: 'return' keyword was missing! */
	return gdNewDynamicCtxEx(initialSize, data, 1);
}
