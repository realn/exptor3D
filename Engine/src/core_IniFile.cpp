/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	inifile.cpp
Autor:	Real_Noname (realnoname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Patrz -> inifile.h.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include <fstream>

#include <CBCore/StringConvert.h>

#include "core_IniFile.h"
#include "StrEx.h"

namespace core {
	namespace {
		bool isSection(const cb::string& line) {
			auto result = core::trim(line);
			if (result.empty())
				return false;
			return result.front() == L'[' && result.back() == L']';
		}

		cb::string getSectionName(cb::string line) {
			line = core::trim(line);
			if (!line.empty() && line.front() == L'[')
				line.erase(line.begin());
			if (!line.empty() && line.back() == L']')
				line.pop_back();
			return core::trim(line);
		}

		bool getEntryName(const cb::string& line, cb::string& result) {
			if (line.empty())
				return false;
			auto pos = line.find(L'=');
			if (pos == cb::string::npos)
				return false;
			result = core::trim(line.substr(0, pos));
			return !result.empty();
		}

		bool getEntryValue(const cb::string& line, cb::string& result) {
			if (line.empty())
				return false;
			auto pos = line.find(L'=');
			if (pos == cb::string::npos)
				return false;
			result = core::trim(line.substr(pos + 1));
			return !result.empty();
		}
	}

	struct IniFile::Section {
		using entry_t = std::pair<cb::string, cb::string>;
		using entries_t = std::vector<entry_t>;

		cb::string name;
		entries_t entries;
	};

	IniFile::IniFile(const cb::string& filename) 
		: core::Object(L"IniFile")
		, file(filename)
	{
		load(filename);
	}

	IniFile::~IniFile() {
		if (changed)
			save();
	}

	bool IniFile::load(const cb::string& filename) {
		if (filename.empty()) {
			error(L"ini filename empty");
			return false;
		}
		file = filename;
		changed = false;
		errors = 0;
		readedValues = 0;

		std::ifstream stream(filename);
		if (!stream.is_open()) {
			error(L"Failed to open ini file.");
			return false;
		}

		log(L"opened ini file, reading...");
		do {
			cb::utf8string lineutf8;
			if (!std::getline(stream, lineutf8))
				return false;

			auto line = cb::fromUtf8(lineutf8);
			if (core::trim(line).empty())
				continue;
			
			if (isSection(line)) {
				auto name = getSectionName(line);
				sections.push_back({ name });
				debug(L"added section " + name);
				continue;
			}

			if (sections.empty()) {
				errors++;
				continue;
			}
			auto& section = sections.back();

			cb::string name, value;
			if (!getEntryName(line, name)) {
				errors++;
				continue;
			}
			if (!getEntryValue(line, value)) {
				errors++;
				continue;
			}
			
			section.entries.push_back({ name, value });
			readedValues++;
			debug(L"added entry " + name + L" of value " + value);
		} while (!stream.eof());

		report(L"Read ini file, values: " + cb::toStr(readedValues) + L", errors: " + cb::toStr(errors));
		return true;
	}

	void IniFile::save() {
		std::ofstream stream(file);
		if (!stream.is_open()) {
			error(L"Failed to open ini file.");
			return;
		}

		for (const auto& section : sections) {
			stream << L"[" << cb::toUtf8(section.name) << L"]" << std::endl;

			for (const auto& entry : section.entries) {
				stream << cb::toUtf8(entry.first) << L"=" << cb::toUtf8(entry.second) << std::endl;
			}
		}

		stream.flush();
		changed = false;
	}

	bool IniFile::getValueStr(cb::string section, cb::string name, cb::string& result) const {
		auto itsec = std::find_if(sections.begin(), sections.end(), [&](const Section& s) {return s.name == section; });
		if (itsec == sections.end())
			return false;
		auto it = std::find_if(itsec->entries.begin(), itsec->entries.end(), [&](const Section::entry_t& e) {return e.first == name; });
		if (it == itsec->entries.end())
			return false;

		result = it->second;
		return true;
	}

	cb::string IniFile::getLogName() const {
		return getClassName() + L"(" + file + L"):";
	}
}
