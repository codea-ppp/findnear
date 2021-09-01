#include <gflags/gflags.h>

DEFINE_uint64(depth, 8, "how far the directionary spread");
DEFINE_string(root, ".", "where search begin");
DEFINE_string(search_file, "", "file search for");

const char usage[] = "find the nearest file from path";
