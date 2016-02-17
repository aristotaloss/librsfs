#include <iostream>
#include <fstream>
#include "file_system.h"
#include "compression.h"
#include <stdio.h>

#ifdef WINDOWS
#include <direct.h>
#define xpf_cwd _getcwd
#else
#include <unistd.h>
#define xpf_cwd getcwd
#endif

using namespace std;

FileSystem *filesystem = nullptr;
void process_lines(istream &);

void help() {
	cout << endl;
	cout << "load <path>" << endl;
	cout << "   Loads a file store from the specified path." << endl;
	cout << "   e.g.: load /home/me/filestore" << endl;
	cout << endl;

	cout << "unload" << endl;
	cout << "   Closes and unloads the currently opened filestore." << endl;
	cout << endl;

	cout << "indices" << endl;
	cout << "   Shows a summary of the indices present and their number of (estimated) folders." << endl;
	cout << endl;

	cout << "info <index> [folder]" << endl;
	cout << "   Displays low-level information about an index and/or folder." << endl;
	cout << "   The index is required, folder is optional." << endl;
	cout << "   If the folder is not provided, all folders will be printed." << endl;
	cout << "   e.g.: info 2 16" << endl;
	cout << endl;

	cout << "dump <index> <folder>" << endl;
	cout << "   Writes the contents of a given folder in an index to a file." << endl;
	cout << "   This file will be called dump_<index>_<folder>.dat." << endl;
	cout << "   e.g.: dump 7 18471" << endl;
}

void load(string line) {
	try {
		string path = line.substr(5);
		cout << "Loading from " << path << "..." << endl;

		FileSystem *loaded = new FileSystem();
		loaded->load_directory(const_cast<char *>(path.c_str()));

		//Swap old and new, and free up old
		delete filesystem;
		filesystem = loaded;

		cout << "Loaded filestore successfully." << endl;
		cout << "Number of indices present: " << filesystem->get_index_count() << "." << endl;
	} catch (exception &exc) {
		cerr << "Could not load filestore." << endl;
		cerr << exc.what() << endl;
	}
}

void unload() {
	// Make sure we don't operate on a nullpointer.
	if (filesystem == nullptr) {
		cerr << "There is no filesystem loaded." << endl;
		return;
	}

	// Delete and unset the variable.
	delete filesystem;
	filesystem = nullptr;

	cout << "Filesystem unloaded." << endl;
}

void list_indices() {
	if (filesystem == nullptr) {
		cerr << "Cannot list indices without a loaded filestore. See 'help'." << endl;
		return;
	}

	int index_count = 0;
	int total = 0;

	// Loop through indices available and print its information
	for (int index = 0; index < 255; index++) {
		auto index_object = filesystem->get_index(index);

		if (index_object) {
			int count = filesystem->get_index(index)->get_entry_count();
			cout << "Index " << index << " has " << count << " folders." << endl;

			total += count;
			index_count++;
		}
	}

	// Print total statistics
	cout << "Total of " << total << " folders across " << index_count << " indices." << endl;
}

void execute_macro(string line) {
	try {
		string path = line.substr(6);
		fstream macro_program(path.c_str(), ios_base::in);

		process_lines(macro_program);
	} catch (exception &exc) {
		cerr << "Error loading macro." << endl;
		cerr << exc.what() << endl;
	}
}

void info(string line) {
	try {
		string index = line.substr(5, line.find(' '));
		cout << "Index: " << index << endl;
	} catch (exception &exc) {
		cerr << "Error dumping folder." << endl;
		cerr << exc.what() << endl;
	}
}

void dump(string line) {
	if (filesystem == nullptr) {
		cerr << "Cannot list indices without a loaded filestore. See 'help'." << endl;
		return;
	}

	try {
		string params = line.substr(5);
		string index = params.substr(0, params.find(' '));
		string folder = params.substr(params.find(' ') + 1);

		int index_int = atoi(index.c_str());
		int folder_int = atoi(folder.c_str());

		cout << "Dumping data from " << index_int << ":" << folder_int << " to dump_" << index_int << "_" << folder_int << ".dat.." << endl;

		auto idx = filesystem->get_index(index_int);
		if (idx == nullptr) {
			cout << "Index " << idx << " does not exist." << endl;
			return;
		}

		auto info = idx->get_folder_info(folder_int);
		if (info.get_size() < 1) {
			cout << "This folder does not exist, or has no data." << endl;
			return;
		}

		vector<char> data;
		int totalread = idx->get_decompressed(folder_int, data);
		ofstream output("dump_" + to_string(index_int) + "_" + to_string(folder_int) + ".dat", ios::out | ios::binary);
		output.write(data.data(), data.size());
		cout << "Dumped " << totalread << " bytes." << endl;
	} catch (exception &exc) {
		cerr << "Error dumping folder." << endl;
		cerr << exc.what() << endl;
	}
}

void process_command(string line) {
	string command = line.find(" ") > 0 ? line.substr(0, line.find(" ")) : line;

	if (command == "help") {
		help();
	} else if (command == "load") {
		load(line);
	} else if (command == "unload") {
		unload();
	} else if (command == "listindices" || command == "listindexes" || command == "indices" || command == "indexes") {
		list_indices();
	} else if (command == "macro") {
		execute_macro(line);
	} else if (command == "dump") {
		dump(line);
	} else {
		cout << "Unknown command. Type 'help' for options." << endl;
	}
}

void process_lines(istream &stream) {
	cout << "> ";

	for (string line; getline(stream, line);) {
		process_command(line);

		cout << endl << "> ";
	}
}

int main() {
	// Print current working dir..
	char cwd[512];
	xpf_cwd(cwd, 512);
	cout << "Working directory: " << cwd << endl << endl;

	process_lines(cin);
	return 0;
}
