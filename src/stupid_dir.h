#ifndef STUPID_DIR_H_ 
#define STUPID_DIR_H_

#include <errno.h>

#include <memory>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#include "src/spread.h"

namespace stupid
{
	
class directionary_virus : public spreadable<directionary_virus>
{
	directionary_virus(const directionary_virus&) = delete;
	const directionary_virus& operator=(const directionary_virus&) = delete;

public:
	directionary_virus(const std::string& path, 
					   const directionary_virus* father = nullptr, 
					   const size_t depth = 0);

	directionary_virus(const char* path, 
		  			   const directionary_virus* father = nullptr,
					   const size_t depth = 0);

	~directionary_virus();

	// absolute
	const std::string& get_path() const;

	bool is_open() const;

	bool find(const std::string& filename) const;
	bool find(const char* filename) const;

	void show_all() const;
	void show_dir() const;
	void show_file() const;

	std::vector<std::unique_ptr<directionary_virus>> spread() const override;

private:
	void fresh_list();
	void load_list();
	void clear_list();

	void dispath_symlink(const dirent*);

	void make_absolute(const std::string&);
	void make_absolute(const char*);
	
	// must after make_absolute. /bar/../bar -> /bar
	void compact_path(std::string&) const;

private:
	std::string _dirpath;
	std::vector<const dirent*> _filelist;
	std::vector<const dirent*> _dirlist;
	struct dirent** _namelist;
	int _record_count;
};

using dirptr = std::unique_ptr<directionary_virus>;

};

#endif 	// STUPID_DIR_H_
