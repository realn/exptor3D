#include "gui_ValueSync.h"

namespace gui {
  ValueSync::ValueSync(std::shared_ptr<IValueHolder> holder, const cb::string& name, const cb::string& prefix, const cb::string& sufix) :
    holder(holder),
    varName(name),
    prefix(prefix),
    sufix(sufix) {
  }

  const cb::string	ValueSync::getVarName() const {
    return varName;
  }

  void	ValueSync::onValueChange(const cb::string& value) {
    holder->setSyncValue(prefix + value + sufix);
  }

  void	ValueSyncList::addValueSync(std::shared_ptr<IValueHolder> holder, const cb::string& valueName, const cb::string& prefix, const cb::string& sufix) {
    list.push_back(ValueSync(holder, valueName, prefix, sufix));
  }

  const bool	ValueSyncList::isVarMonitored(const cb::string& name) const {
    auto it = std::find_if(list.begin(), list.end(), [&](const ValueSync& item) { return item.getVarName() == name; });
    return it != list.end();
  }

  void	ValueSyncList::onVarChanged(const cb::string& name, const cb::string& value) {
    for (auto& item : list) {
      if (item.getVarName() == name) {
        item.onValueChange(value);
      }
    }
  }
}
