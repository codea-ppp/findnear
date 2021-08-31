#include "src/stupid_dir.h"

int main(int argc, char* argv[])
{
	stupid::directionary_virus test(".");

	printf("%s:\n", test.get_path().c_str());
	test.show_dir();
	test.show_file();
	printf("\n");
	
	
	auto spread = test.spread();
	for (auto& i : spread)
	{
		auto spread_2 = i->spread();
		for (auto& j : spread_2)
		{
			printf("%s:\n", j->get_path().c_str());
			j->show_dir();
			j->show_file();
			printf("\n");
		}
	}

	return 0;
}
