#include <experimental/filesystem>
#include <iostream>
#include <unordered_map>
namespace fs = std::experimental::filesystem;

typedef std::unordered_map<uint64_t, std::vector<fs::path>> files_map;

void usage() {
	std::cout << "Usage: ./dup_calc <dir_name>" << std::endl;
}

void recoursiveFindFiles(fs::path const & path, files_map & files) {
	for (auto const & p : fs::recursive_directory_iterator(path)) {
		if (fs::is_regular_file(p)) {
			files[fs::file_size(p)].push_back(p);
		}
	}
}

std::vector<fs::path> findDups(std::vector<fs::path> pathes) {
	std::vector<fs::path> dups;
	return dups;
}

int main(int argc, char * argv[]) {
	if (argc != 2 || !is_directory(fs::path(argv[1]))) {
		usage();
		return 1;
	}

	files_map sizeFilenamesMapping;
	recoursiveFindFiles(argv[1], sizeFilenamesMapping);

	for (auto const & sizeFilesPair : sizeFilenamesMapping) {
		std::cout << "Size = " << sizeFilesPair.first << std::endl;
		for (auto const & filePath : sizeFilesPair.second) {
			std::cout << '\t' << filePath << std::endl;
		}
	}

	return 0;
}
