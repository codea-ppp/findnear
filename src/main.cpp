#include "src/stupid_dir.h"
#include "src/flags.h"
#include <system_error>

int main(int argc, char* argv[])
{
	google::SetUsageMessage(usage);
	google::ParseCommandLineFlags(&argc, &argv, true);
	if (FLAGS_search_file.empty())
	{
		google::ProgramUsage();
		return 1;
	}

	int dirvec_curr_index = 0;
	std::vector<std::vector<stupid::dirptr>> dirvec_slots[3];
	std::vector<std::vector<stupid::dirptr>>* dirvec_curr = &dirvec_slots[dirvec_curr_index];
	std::vector<std::vector<stupid::dirptr>>* dirvec_futu = &dirvec_slots[(dirvec_curr_index + 1) % 3];

	try
	{
		stupid::directionary_virus root(FLAGS_root);
		if (root.find(FLAGS_search_file))
		{
			printf("%s/%s", root.get_path().c_str(), FLAGS_search_file.c_str());
			return 0;
		}

		dirvec_curr->push_back(root.spread());
	}
	catch (std::error_code err)
	{
		printf("%s: %s\n", FLAGS_root.c_str(), strerror(err.value()));
	}

	for (uint64_t i = 0; i < FLAGS_depth; ++i)
	{
		dirvec_futu->clear();

		for (auto& dir_collect : *dirvec_curr)
		{
			for (auto& dir : dir_collect)
			{
				if (dir->find(FLAGS_search_file))
				{
					printf("%s/%s", dir->get_path().c_str(), FLAGS_search_file.c_str());
					return 0;
				}

				dirvec_futu->push_back(dir->spread());
			}
		}

		dirvec_curr_index++;
		dirvec_curr = &dirvec_slots[dirvec_curr_index % 3];
		dirvec_futu = &dirvec_slots[(dirvec_curr_index + 1) % 3];
	}

	printf("cannot find %s at %lu level\n", FLAGS_search_file.c_str(), FLAGS_depth);
	return 0;
}
