#pragma once
#include <cstdint>
#include <filesystem>

#include <string>
#include <vector>
#include <sstream>
#include <exception>
#include <chrono>
#include "./pfc_bitmap_3.h"

#include "Macros.h"

using pixel_t = pfc::BGR_4_t;

namespace fs = std::filesystem;
namespace t = std::chrono;
inline void CleanTheFolders(std::vector<std::string>& FolderToCheck) {
	fs::path dir = fs::current_path();
	for (auto folder : FolderToCheck) {

		if (fs::exists(dir / folder)) { 
			std::uintmax_t n = fs::remove_all(dir / folder);
			fs::create_directories(dir / folder);
		}
		else 
		{
			fs::create_directories(dir / folder);
		}
	}
}


inline void WriteImagetoFile(std::string& FolderToSave, pfc::BGR_4_t* h_p, int const width, int const height,size_t size, size_t beginOfCount = 0)
{
	size_t iter = 0;
		fs::path fpath(fs::current_path().string() + "//" + FolderToSave);
		if (fs::exists(fpath)) {
			
			//std::cout << "copying images "<< iter<< "... this may take some time" << std::endl;
			do {
				std::string name = "./" + FolderToSave +"/test_" + std::to_string(iter+ beginOfCount) + ".bmp";
				//gsl::span < pfc::BGR_4_t* > span(h_p[size], height,width);
				pfc::bitmap temp(WIDTH, HEIGHT) ;
				//std::memcpy(std::data(temp.pixel_span()), h_p[iter], height*width*sizeof(pfc::BGR_4_t));
				std::memcpy(std::data(temp.pixel_span()), h_p, height*width*sizeof(pfc::BGR_4_t));
				temp.to_file(name);
				std::cout << "figure " << name << " finished" << std::endl;
				++iter;
			} while (iter < size);
		}
}

inline void WriteImagetoFile(std::string& FolderToSave, pfc::BGR_4_t** h_p, int const width, int const height, size_t size, size_t beginOfCount = 0)
{
	size_t iter = 0;
	fs::path fpath(fs::current_path().string() + "//" + FolderToSave);
	if (fs::exists(fpath)) {

		std::cout << "copying images... this may take some time" << std::endl;
		do {
			std::string name = "./" + FolderToSave + "/test_" + std::to_string(iter + beginOfCount) + ".bmp";
			//gsl::span < pfc::BGR_4_t* > span(h_p[size], height,width);
			pfc::bitmap temp(WIDTH, HEIGHT);
			//std::memcpy(std::data(temp.pixel_span()), h_p[iter], height*width*sizeof(pfc::BGR_4_t));
			std::memcpy(std::data(temp.pixel_span()), h_p[iter], height*width * sizeof(pfc::BGR_4_t));
			temp.to_file(name);
#if INFO
			std::cout << "figure " << name << " finished" << std::endl;
#endif
			++iter;
		} while (iter < size);
	}
}



inline void GetTimeInfo(std::ostream& ost,std::chrono::nanoseconds time) {
	
	
	t::minutes		mm = t::duration_cast<t::minutes>(time % std::chrono::hours(1));
	t::seconds		ss = t::duration_cast<t::seconds>(time % std::chrono::minutes(1));
	t::milliseconds ms = t::duration_cast<t::milliseconds>(time % std::chrono::seconds(1));
	t::microseconds mikros = t::duration_cast<t::microseconds>(time % std::chrono::milliseconds(1));
	t::nanoseconds  ns = t::duration_cast<t::nanoseconds>(time % std::chrono::microseconds(1));
	ost << "min: " << mm.count() << ",sec: " << ss.count() << ",msec: " << ms.count()
		<< ",mikrosec: " << mikros.count() << ",nanosec:" << ns.count() << "\n";
}

inline double GetMBpPerSec(std::chrono::nanoseconds time, double Megabytes) {
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time);
	return (Megabytes / ms.count());

}






inline void  getFoldersize(std::string rootFolder, unsigned long long & f_size)
{
	fs::path folderPath(rootFolder);
	if (exists(folderPath))
	{

		//std::string path = "/path/to/directory";
		for (const auto & entry : fs::directory_iterator(rootFolder))
		{
# if INFO
			std::cout <<"checking the size of file '"<< entry.path()<<"' ..."<< std::endl;
#endif
			try {
				if (!fs::is_directory(entry.status()))
				{
					f_size = f_size + fs::file_size(entry.path());
				}
				else
				{
					getFoldersize(entry.path().string(), f_size);
				}
			}
			catch (std::exception& e) { std::cout << e.what() << std::endl; }
		}

	}
}

const char *FormatBytes(unsigned long long bytes, char *str)
{
	const char *sizes[5] = { "B", "KB", "MB", "GB", "TB" };

	int i;
	unsigned long long dblByte = bytes;
	unsigned long long Factor = 1024;
	for (i = 0; i < 6 && bytes >= 1024; i++, bytes /= 1024)
		dblByte = bytes / Factor;

	sprintf(str, "%.2f", dblByte);

	return strcat(strcat(str, " "), sizes[i]);
}