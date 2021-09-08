#include <gflags/gflags.h>

DEFINE_uint64(depth, 8, "how far the directionary spread");
DEFINE_string(root, ".", "where search begin");
DEFINE_string(search_file, "", "file search for");

const char usage[] = 
	"find the nearest file from path\n"
	"findnear [--depth=<depth>] --root=<path> --search_file=<filename>\n"
	"\t<depth>: how many level search spread outside\n"
	"\t<path>: the root of this search\n"
	"\t<filename>: the file to search\n";
