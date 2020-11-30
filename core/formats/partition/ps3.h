#ifndef PS3_H
#define PS3_H

#include <stdint.h>

#define MAGIC1			0x0FACE0FFULL
#define MAGIC2			0xDEADFACEULL

struct p_acl_entry {
  uint64_t laid;
  uint64_t rights;
};

struct d_partition {
  uint64_t p_start;
  uint64_t p_size;
  struct p_acl_entry p_acl[8];
};

struct disklabel {
  uint8_t d_res1[16];
  uint64_t d_magic1;
  uint64_t d_magic2;
  uint64_t d_res2;
  uint64_t d_res3;
};

#endif /* PS3_H */
