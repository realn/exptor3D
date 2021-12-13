/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	inifile.h
Autor:	Real_Noname (realnoname@coderulers.info)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.info
Opis:	Zawiera definicje klasy do odczytu pliku INI.

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#pragma once

#include <CBCore/Defines.h>
#include <CBCore/StringConvert.h>

#include "core_object.h"

namespace core {
	class IniFile : core::Object {
	public:
		IniFile(const cb::string& filename);
		~IniFile() override;

		bool load(const cb::string& filename);
		void save();

		bool getValueStr(cb::string section, cb::string name, cb::string& result) const;

		template<class _Type>
		_Type getValue(cb::string section, cb::string name, _Type defValue) const {
			cb::string result;
			if (!getValueStr(section, name, result))
				return defValue;
			_Type resultValue;
			if (!cb::fromStr(result, resultValue))
				return defValue;
			return resultValue;
		}

	protected:
		cb::string getLogName() const override;

	private:
		struct Section;
		using sections_t = std::vector<Section>;

		cb::string file;
		sections_t sections;
		bool changed = false;
		cb::u32 readedValues = 0;
		cb::u32 errors = 0;
	};
}