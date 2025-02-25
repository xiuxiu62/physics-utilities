#pragma once

#include "common/debug.hpp"
#include <cstdio>

// TODO: expand into nothingin release mode

#define info(fmt, ...) when_debug(fprintf(stderr, "[INFO] " fmt "\n", ##__VA_ARGS__));

#define warn(fmt, ...) when_debug(fprintf(stderr, "[WARN] " fmt "\n", ##__VA_ARGS__));

#define error(fmt, ...) when_debug(fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__));
