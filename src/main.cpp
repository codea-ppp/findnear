#include "src/stupid_dir.h"
#include "src/flags.h"
#include <memory>

int main(int argc, char* argv[])
{
	google::SetUsageMessage(usage);
	google::ParseCommandLineFlags(&argc, &argv, true);
	if (FLAGS_search_file.empty())
	{
		google::ProgramUsage();
		return 1;
	}

	try
	{
		stupid::directionary_virus root(FLAGS_root);
		if (root.find(FLAGS_search_file))
		{
			printf("%s/%s", root.get_path().c_str(), FLAGS_search_file.c_str());
			return 0;
		}


		std::unique_ptr<stupid::directionary_virus> dirptr(&root);

		std::queue<std::unique_ptr<std::vector<std::unique_ptr<stupid::directionary_virus>>>> dirs_que;
		dirs_que.push(std::make_unique<std::vector<std::unique_ptr<stupid::directionary_virus>>>(dirptr->spread()));

		for (uint64_t i = 0; i < FLAGS_depth; ++i)
		{
			std::unique_ptr<std::vector<std::unique_ptr<stupid::directionary_virus>>> dir_collect= std::move(dirs_que.front());

			for (auto& dir: *dir_collect)
			{
				if (dir->find(FLAGS_search_file))
				{
					printf("%s/%s", dir->get_path().c_str(), FLAGS_search_file.c_str());
					return 0;
				}

				dirs_que.push(std::make_unique<std::vector<std::unique_ptr<stupid::directionary_virus>>>(dir->spread()));
			}

			dirs_que.pop();
		}

		printf("cannot find %s at %lu level\n", FLAGS_search_file.c_str(), FLAGS_depth);
	}
	catch (std::string err)
	{
		printf("%s\n", err.c_str());
	}

	return 0;
}
