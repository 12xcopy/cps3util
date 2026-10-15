#include "cps3.h"

struct vfs_s *simm_combine(struct vfs_s *source);
struct vfs_s *simm_split(struct vfs_s *source,const char* gamename);
struct vfs_s *simm_decrypt(struct vfs_s *source,int game);
struct vfs_s *simm_encrypt(struct vfs_s *source,int game);
