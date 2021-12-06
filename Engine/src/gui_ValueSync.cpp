#include "gui_ValueSync.h"

namespace gui {
  ValueSync::ValueSync(std::shared_ptr<IValueHolder> holder, const std::string& name, const std::string& prefix, const std::string& sufix) :
    holder(holder),
    varName(name),
    prefix(prefix),
    sufix(sufix) {
  }

  const std::string	ValueSync::getVarName() const {
    return varName;
  }

  void	ValueSync::onValueChange(const std::string& value) {
    holder->setSyncValue(prefix + value + sufix);
  }

  void	ValueSyncList::addValueSync(std::shared_ptr<IValueHolder> holder, const std::string& valueName, const std::string& prefix, const std::string& sufix) {
    list.push_back(ValueSync(holder, valueName, prefix, sufix));
  }

  const bool	ValueSyncList::isVarMonitored(const std::string& name) const {
    auto it = std::find_if(list.begin(), list.end(), [&](const ValueSync& item) { return item.getVarName() == name; });
    return it != list.end();
  }

  void	ValueSyncList::onVarChanged(const std::string& name, const std::string& value) {
    for (auto& item : list) {
      if (item.getVarName() == name) {
        item.onValueChange(value);
      }
    }
  }
}
