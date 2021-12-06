#pragma once

#include <CBCore/Defines.h>

namespace gui {
	class IValueHolder {
	public:
		virtual ~IValueHolder() = default;

		virtual void setSyncValue(const std::string& value) = 0;
		virtual const std::string getSyncValue() const = 0;
	};

	class ValueSync {
	protected:
		std::shared_ptr<IValueHolder> holder;
		std::string		varName;
		std::string		prefix;
		std::string		sufix;

	public:
		ValueSync(std::shared_ptr<IValueHolder> holder, const std::string& name, const std::string& prefix = "", const std::string& sufix = "");

		const std::string	getVarName() const;

		void	onValueChange(const std::string& value);
	};
	
	class ValueSyncList {
	public:
		void	addValueSync(std::shared_ptr<IValueHolder> holder, const std::string& valueName, const std::string& prefix = "", const std::string& sufix = "");

		const bool	isVarMonitored(const std::string& name) const;
		void	onVarChanged(const std::string& name, const std::string& value);

	private:
		using valuesynclist_t = std::vector<ValueSync>;

		valuesynclist_t list;
	};
}