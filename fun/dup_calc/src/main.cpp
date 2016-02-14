#include <experimental/filesystem>
#include <iostream>
#include <iomanip> 
#include <fstream>
#include <unordered_map>
#include <openssl/md5.h>
#include <set>
#include <map>

namespace fs = std::experimental::filesystem; 
typedef std::unordered_map<uint64_t, std::vector<fs::path>> files_map;

const size_t BUFFER_SIZE = 4096;

void usage() {
	std::cout << "Usage: ./dup_calc <dir_name>" << std::endl;
}

void recoursive_find_files(fs::path const & path, files_map & files) {
	for (auto const & p : fs::recursive_directory_iterator(path)) {
		if (fs::is_regular_file(p)) {
			files[fs::file_size(p)].push_back(p);
		}
	}
}

void evaluate_hash(fs::path const & path, uint8_t md5digest[MD5_DIGEST_LENGTH]){
	MD5_CTX md5_handler;
	uint8_t buffer[BUFFER_SIZE];
	MD5_Init(&md5_handler);
	std::ifstream is(path, std::ifstream::binary);
	while (!is.eof()) {
		is.read((char*)buffer, BUFFER_SIZE);
		size_t readed = is.gcount();
		MD5_Update(&md5_handler, buffer, readed);
	}

	MD5_Final(md5digest, &md5_handler);
}

std::map<std::string, std::vector<fs::path>> findDups(std::vector<fs::path> pathes) {
	std::map<std::string, std::vector<fs::path>> hash_files_map;
	uint8_t md5digest[MD5_DIGEST_LENGTH];
	
	for (auto const & filePath : pathes) {
		evaluate_hash(filePath, md5digest);
		std::string hash(reinterpret_cast<const char *>(md5digest), MD5_DIGEST_LENGTH);
		hash_files_map[hash].push_back(filePath);
	}

	return hash_files_map;
}

std::string get_pretty_size(double size){
	std::stringstream sstream;
	sstream << std::fixed << std::setprecision(1);
	if (size < 1024)
		sstream << size << " B";
	else if (size < 1024 * 1024)
		sstream << size / 1024 << " KB";
	else if (size < 1024 * 1024 * 1024)
		sstream << size / 1024 / 1024 << " MB";
	else
		sstream << size / 1024 / 1024 / 1024 << " GB";
	return sstream.str();
}

void print_files_group(std::vector<fs::path> const & pathes, size_t file_size, size_t group_num) {
	std::cout << "#" << group_num << " " << get_pretty_size(file_size) << std::endl;
	for (auto const & path : pathes) {
		std::cout << path.string() << std::endl;
	}

	std::cout << std::endl;
}

int main(int argc, char * argv[]) {
	if (argc != 2 || !is_directory(fs::path(argv[1]))) {
		usage();
		return 1;
	}

	files_map size_filenames_mapping;
	recoursive_find_files(argv[1], size_filenames_mapping);

	size_t group_num = 1;
	uint64_t total_size = 0;
	for (auto const & size_files_pair : size_filenames_mapping) {
		if (size_files_pair.second.size() < 2) continue;
		auto hash_files_map = findDups(size_files_pair.second);
		for (auto const & hash_files_pair : hash_files_map) {
			if (hash_files_pair.second.size() > 1) {
				print_files_group(hash_files_pair.second, size_files_pair.first, group_num++);
				total_size += size_files_pair.first * (hash_files_pair.second.size() - 1);
			}
		}
	}

	std::cout << get_pretty_size(total_size) << " wasted in total." << std::endl;
	return 0;
}
