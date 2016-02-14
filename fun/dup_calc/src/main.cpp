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

void recoursiveFindFiles(fs::path const & path, files_map & files) {
	for (auto const & p : fs::recursive_directory_iterator(path)) {
		if (fs::is_regular_file(p)) {
			files[fs::file_size(p)].push_back(p);
		}
	}
}

void evaluateHash(fs::path const & path, uint8_t md5digest[MD5_DIGEST_LENGTH]){
	MD5_CTX md5handler;
	uint8_t buffer[BUFFER_SIZE];
	MD5_Init(&md5handler);
	std::ifstream is(path, std::ifstream::binary);
	while (!is.eof()) {
		is.read((char*)buffer, BUFFER_SIZE);
		size_t readed = is.gcount();
		MD5_Update(&md5handler, buffer, readed);
	}

	MD5_Final(md5digest, &md5handler);
}

std::map<std::string, std::vector<fs::path>> findDups(std::vector<fs::path> pathes) {
	std::map<std::string, std::vector<fs::path>> hashFilesMap;
	uint8_t md5digest[MD5_DIGEST_LENGTH];
	
	for (auto const & filePath : pathes) {
		evaluateHash(filePath, md5digest);
		std::string hash(reinterpret_cast<const char *>(md5digest), MD5_DIGEST_LENGTH);
		hashFilesMap[hash].push_back(filePath);
	}

	return hashFilesMap;
}

std::string getSize(double size){
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

void printFilesGroup(std::vector<fs::path> const & pathes, size_t fileSize, size_t groupNum) {
	std::cout << "#" << groupNum << " " << getSize(fileSize) << std::endl;
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

	files_map sizeFilenamesMapping;
	recoursiveFindFiles(argv[1], sizeFilenamesMapping);

	size_t groupNum = 1;
	uint64_t totalSize = 0;
	for (auto const & sizeFilesPair : sizeFilenamesMapping) {
		if (sizeFilesPair.second.size() < 2) continue;
		auto hashFilesMap = findDups(sizeFilesPair.second);
		for (auto const & hashFilesPair : hashFilesMap) {
			if (hashFilesPair.second.size() > 1) {
				printFilesGroup(hashFilesPair.second, sizeFilesPair.first, groupNum++);
				totalSize += sizeFilesPair.first * (hashFilesPair.second.size() - 1);
			}
		}
	}

	std::cout << getSize(totalSize) << " wasted in total." << std::endl;
	return 0;
}
