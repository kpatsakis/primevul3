int rand() {
  rand_m_z = 36969 * (rand_m_z & 65535) + (rand_m_z >> 16);
  rand_m_w = 18000 * (rand_m_w & 65535) + (rand_m_w >> 16);
  return (int)RAND_MAX & (int)((rand_m_z << 16) + rand_m_w);  /* 32-bit result */
}
