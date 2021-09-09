#include "src/stupid_dir.h"
#include <exception>
#include <system_error>

namespace stupid
{

directionary_virus::directionary_virus(const std::string& path, 
									   const directionary_virus* father, 
									   const size_t depth)
	: spreadable(father, depth)
	, _namelist(nullptr)
	, _record_count(-1)
{
	make_absolute(std::move(path));
	load_list();
}

directionary_virus::directionary_virus(const char* path, 
									   const directionary_virus* father,
									   const size_t depth)
	: spreadable(father, depth)
	, _namelist(nullptr)
	, _record_count(-1)
{

	make_absolute(path);
	load_list();
}

directionary_virus::~directionary_virus()
{
	if (is_open())
	{
		clear_list();
	}
}

const std::string& directionary_virus::get_path() const
{
	return _dirpath;
}

bool directionary_virus::is_open() const
{
	return _namelist != nullptr;
}

bool directionary_virus::find(const std::string& filename) const
{ 
	return find(filename.c_str()); 
}

bool directionary_virus::find(const char* filename) const
{
	for (auto& i : _filelist)
	{
		if (!strncmp(filename, i->d_name, 256))
			return true;
	}

	return false;
}

void directionary_virus::show_all() const
{
	assert(_namelist != nullptr);

	for (int i = 0; i < _record_count; ++i)
	{
		printf("%s ", _namelist[i]->d_name);
	}

	printf("\n");
}

void directionary_virus::show_dir() const 
{
	assert(_namelist != nullptr);

	for (auto& i : _dirlist)
	{
		printf("%s ", i->d_name);
	}

	printf("\n");
}

void directionary_virus::show_file() const 
{
	assert(_namelist != nullptr);

	for (auto& i : _filelist)
	{
		printf("%s ", i->d_name);
	}

	printf("\n");
}

std::vector<std::unique_ptr<directionary_virus>> directionary_virus::spread() const
{
	std::string current_path = get_path();
	current_path.append("/");

	std::string father_path;

	const directionary_virus* father = get_father();
	if (father != nullptr)
	{
		father_path.assign(father->get_path());
	}

	std::vector<std::unique_ptr<directionary_virus>> accu;
	for (auto& i : _dirlist)
	{
		if (!strncmp(i->d_name, ".", 2))
			continue;

		std::string target_path(current_path);
		target_path.append(i->d_name);

		compact_path(target_path);

		if (target_path == father_path)
			continue;

		try
		{
			accu.push_back(std::make_unique<directionary_virus>(target_path, this, get_depth() + 1));
		}
		catch (std::error_code err)
		{
			printf("%s: %s\n", target_path.c_str(), strerror(err.value()));
		}
	}

	return accu;
}

void directionary_virus::fresh_list()
{
	clear_list();
	load_list();
}

void directionary_virus::load_list()
{
	assert(_namelist == nullptr);

	_record_count = scandir(_dirpath.c_str(), &_namelist, NULL, alphasort);
	if (_record_count == -1)
		throw std::error_code(errno, std::generic_category());

	for (int i = 0; i < _record_count; ++i)
	{
		switch (_namelist[i]->d_type)
		{
		case DT_REG: _filelist.push_back(_namelist[i]); 	break;
		case DT_DIR: _dirlist.push_back(_namelist[i]); 		break;
		case DT_LNK: dispath_symlink(_namelist[i]);			break;

		case DT_BLK:
		case DT_CHR:
		case DT_FIFO:
		case DT_SOCK: 
		case DT_UNKNOWN: 
		default: break;
		}
	}
}

void directionary_virus::clear_list()
{
	assert(_namelist != nullptr);

	for (int i = 0; i < _record_count; ++i)
	{
		free(_namelist[i]);
	}

	free(_namelist);

	_record_count = -1;
	_namelist = nullptr;

	_filelist.clear();
	_dirlist.clear();
}

void directionary_virus::dispath_symlink(const dirent* symlink)
{
	char buffer[4096] = { 0 };
	snprintf(buffer, sizeof(buffer), "%s/%s", _dirpath.c_str(), symlink->d_name);

	static struct stat status;
	stat(buffer, &status);

	if (S_ISREG(status.st_mode))
		_filelist.push_back(symlink);
	else if (S_ISDIR(status.st_mode))
		_dirlist.push_back(symlink);
}

void directionary_virus::make_absolute(const std::string& path)
{
	make_absolute(path.c_str());
}

void directionary_virus::make_absolute(const char* path_in)
{
	if (path_in[0] == '/')
	{
		_dirpath.assign(path_in);
		compact_path(_dirpath);
		return;
	}
	else if (path_in[0] == '~' && (path_in[1] == '\0' || path_in[1] == '/'))
	{
		// ~/ or ~
		_dirpath.assign(getenv("HOME"));
		if (path_in[1] == '/')
			_dirpath.append("/").append(path_in + 2);

		compact_path(_dirpath);
		return;
	}

	char* path_abs = get_current_dir_name();
	if (path_abs == nullptr)
		throw std::error_code(errno, std::generic_category());

	_dirpath.assign(path_abs).append("/").append(path_in);
	free(path_abs);

	compact_path(_dirpath); // _dirpath is key to check father
}

void directionary_virus::compact_path(std::string& relative_path) const
{
	std::string::size_type tail_slash = 0;
	std::string::size_type head_slash = relative_path.find("/", tail_slash + 1);

	std::string temp;
	std::string accu;

	auto temp_update = [&]() {
		if (head_slash != std::string::npos)
			temp = relative_path.substr(tail_slash, head_slash - tail_slash);
		else 
			temp = relative_path.substr(tail_slash);
	};

	while (true)
	{
		temp_update();
		
		if (head_slash - tail_slash == 1) { /* such ./bar//foo*/ }
		else if (temp == "/.") { }
		else if (temp == "/..")
			accu = accu.substr(0, accu.rfind("/", tail_slash - 1));
		else
			accu.append(temp);

		if (head_slash == std::string::npos)
			break;

		tail_slash = head_slash;
		head_slash = relative_path.find("/", tail_slash + 1);
	}

	std::swap(accu, relative_path);
}

}

