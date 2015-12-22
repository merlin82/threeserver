#ifndef _CONFIG_H_
#define _CONFIG_H_
#include "gflags/gflags.h"
#include "glog/logging.h"

DECLARE_string(listen_ip);
DECLARE_string(listen_port);
DECLARE_int32(thread_pool_size);

#endif

