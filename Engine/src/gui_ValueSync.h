#pragma once

#include <CBCore/Defines.h>

namespace gui {
  class IValueHolder {
  public:
    virtual ~IValueHolder() = default;

    virtual void setSyncValue(const cb::string& value) = 0;
    virtual const cb::string getSyncValue() const = 0;
  };

  class ValueSync {
  protected:
    std::shared_ptr<IValueHolder> holder;
    cb::string		varName;
    cb::string		prefix;
    cb::string		sufix;

  public:
    ValueSync(std::shared_ptr<IValueHolder> holder, const cb::string& name, const cb::string& prefix = cb::string(), const cb::string& sufix = cb::string());

    const cb::string	getVarName() const;

    void	onValueChange(const cb::string& value);
  };

  class ValueSyncList {
  public:
    void	addValueSync(std::shared_ptr<IValueHolder> holder, const cb::string& valueName, const cb::string& prefix = cb::string(), const cb::string& sufix = cb::string());

    const bool	isVarMonitored(const cb::string& name) const;
    void	onVarChanged(const cb::string& name, const cb::string& value);

  private:
    using valuesynclist_t = std::vector<ValueSync>;

    valuesynclist_t list;
  };
}