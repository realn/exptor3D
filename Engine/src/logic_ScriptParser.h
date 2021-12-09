#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "core_StrArgList.h"
#include "event_Observer.h"
#include "event_Mapper.h"

namespace event {
  class Manager;
}

typedef void	(*SCRIPT_FUNC)(void* pUserData, const std::vector<std::string>& params);

class CScriptVar {
public:
  std::string	Name;
  std::string	Value;

  CScriptVar(const std::string& name, const std::string& value) :
    Name(name), Value(value) {
  }
};


namespace logic {
  class IFuncHolder {
  public:
    virtual ~IFuncHolder() = default;
  };

  class ScriptFunc {
  public:
    using func_t = std::function<void(const core::StrArgList& args)>;

    ScriptFunc() = default;
    ScriptFunc(const cb::string& name, func_t func, std::shared_ptr<IFuncHolder> holder = nullptr) : name(name), func(func), hasHolder(holder), holder(holder) {}

    const cb::string& getName() const { return name; }
    bool isValid() const { return hasHolder ? holder.lock() != nullptr : true; }

    void execute(const cb::strvector& args) {
      func(core::StrArgList(args));
    }

  private:
    cb::string name;
    func_t func;
    bool hasHolder = false;
    std::weak_ptr<IFuncHolder> holder;
  };

  class ScriptParser : public event::IObserver {
  public:
    ScriptParser(std::shared_ptr<event::Manager> eventManager);
    ~ScriptParser() override;

    //void addVar(const std::string& name, const std::string& value = "");
    void addFunc(const cb::string& name, ScriptFunc::func_t func);
    void addFunc(const cb::string& name, std::shared_ptr<IFuncHolder> holder, ScriptFunc::func_t func);
    ///void setVar(const std::string& name, const std::string& value);
    //const std::string getVarValue(const std::string& name) const;
    //const bool getVarValue(const std::string& name, std::string& outValue) const;

    void execute(const cb::string& text, const bool printResult = false);

    cb::strvector searchFuncNames(const cb::string& what) const;

    void	processEvent(const event::Event& event) override;

  private:
    using funcs_t = std::vector<ScriptFunc>;

    void clearDeleted();

    void executeVar(const std::string& text, const bool printResult);
    void executeFunc(const std::string& text, const bool printResult);

    //CScriptVar* findVar(const std::string& name) const;
    //CScriptFunc* findFunc(const std::string& name) const;

    std::shared_ptr<event::Manager> eventManager;
    event::Mapper mapper;

    funcs_t funcs;
    std::vector<CScriptVar> VarList;
  };
}