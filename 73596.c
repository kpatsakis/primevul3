static void arr_replace(uint16_t *arr, uint8_t max, x86_reg r1, x86_reg r2)
{
	uint8_t i;

	for(i = 0; i < max; i++) {
		if (arr[i] == r1) {
			arr[i] = r2;
			break;
		}
	}
}
