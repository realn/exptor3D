#include <CBCore/Defines.h>
#include <CBCore/StringConvert.h>

#include "core_LineParser.h"

#include "event_Event.h"
#include "event_Manager.h"

#include "StrEx.h"
#include "logic_ScriptParser.h"

namespace logic {
  namespace {
    const cb::string EVENT_ACTION_VAR_ADD = L"script_var_add";
    const cb::string EVENT_ACTION_VAR_SET = L"script_var_set";
    const cb::string EVENT_ACTION_FUNC_ADD = L"script_func_add";
    const cb::string EVENT_ACTION_FUNC_CALL = L"script_func_call";
    const cb::string EVENT_ACTION_SCRIPT_EXEC = L"script_exec";
  }

  ScriptParser::ScriptParser(std::shared_ptr<event::Manager> eventManager)
    : eventManager(eventManager) {
    mapper.addAction(EVENT_ACTION_SCRIPT_EXEC, [this](const event::EventAction& a) { execute(a.getArg(0)); });
  }

  ScriptParser::~ScriptParser() = default;

  //void	ScriptParser::addVar(const std::string& name, const std::string& value) {
  //	auto pVar = findVar(name);
  //	if (pVar == nullptr) {
  //		CScriptVar var(name, value);
  //		VarList.push_back(var);

  //		eventManager->addActionEvent(EVENT_ACTION_VAR_ADD, { cb::fromUtf8(name) });
  //	}
  //}

  void	ScriptParser::addFunc(const cb::string& name, ScriptFunc::func_t func) {
    addFunc(name, nullptr, func);
  }

  void ScriptParser::addFunc(const cb::string& name, std::shared_ptr<IFuncHolder> holder, ScriptFunc::func_t func) {
    clearDeleted();

    if (std::find_if(funcs.begin(), funcs.end(), [&](logic::ScriptFunc& f) { return f.getName() == name; }) != funcs.end()) {
      return;
    }

    funcs.emplace_back(ScriptFunc(name, func, holder));
    eventManager->addActionEvent(EVENT_ACTION_FUNC_ADD, { name });
  }

  //void	ScriptParser::setVar(const std::string& name, const std::string& value) {
  //	auto pVar = findVar(name);
  //	if (pVar != nullptr) {
  //		pVar->Value = value;

  //		eventManager->addActionEvent(EVENT_ACTION_VAR_SET, { cb::fromUtf8(name), cb::fromUtf8(value) });
  //	}
  //}

  //const bool	ScriptParser::getVarValue(const std::string& name, std::string& outValue) const {
  //	auto pVar = findVar(name);
  //	if (pVar == nullptr)
  //		return false;

  //	outValue = pVar->Value;
  //	return true;
  //}

  //const std::string	ScriptParser::getVarValue(const std::string& name) const {
  //	auto pVar = findVar(name);
  //	if (pVar == nullptr)
  //		return "";

  //	return pVar->Value;
  //}

  void	ScriptParser::execute(const cb::string& text, const bool printResult) {
    auto parser = core::LineParser();
    parser.parse(text);
    if (parser.getCmd().empty())
      return;

    clearDeleted();
    auto it = std::find_if(funcs.begin(), funcs.end(), [&](ScriptFunc& f) {return f.getName() == parser.getCmd(); });
    if (it == funcs.end())
      return;

    it->execute(parser.getArgs());
    eventManager->addActionEvent(EVENT_ACTION_FUNC_CALL, { parser.getCmd() });
  }

  cb::strvector	ScriptParser::searchFuncNames(const cb::string& what) const {
    auto names = cb::strvector();
    std::transform(funcs.begin(), funcs.end(), std::back_inserter(names), [](const ScriptFunc& f) { return f.getName(); });
    auto result = cb::strvector();
    std::copy_if(names.begin(), names.end(), std::back_inserter(result), [&](const cb::string& name) { return name.substr(0, what.length()) == what; });

    return result;
  }

  void ScriptParser::processEvent(const event::Event& event) {
    mapper.executeEvent(event);
  }

  void ScriptParser::clearDeleted() {
    for(auto it = funcs.begin(); it != funcs.end();) {
      if (!it->isValid())
        it = funcs.erase(it);
      else
        it++;
    }
  }

  //void	ScriptParser::executeVar(const std::string& text, const bool printResult) {
  //	if (ClearWhiteSpaceFront(text).find("var") == 0) {
  //		auto name = ClearWhiteSpace(text).substr(3);
  //		auto pVar = findVar(name);
  //		if (pVar == nullptr) {
  //			addVar(name);
  //			if (printResult)
  //				executeFunc("Print(Defined var " + name + ")", printResult);
  //		}
  //	}
  //	auto pos = text.find("=");
  //	if (pos != std::string::npos) {
  //		auto name = ClearWhiteSpace(text.substr(0, pos));
  //		auto pVar = findVar(name);
  //		if (pVar != nullptr) {
  //			auto value = ClearWhiteSpaceFront(text.substr(pos + 1));
  //			setVar(name, value);
  //			if (printResult)
  //				executeFunc("Print(Var " + name + " was set to " + value + ")", printResult);
  //		}
  //	}
  //	else {
  //		auto name = ClearWhiteSpace(text);
  //		auto pVar = findVar(name);
  //		if (pVar != nullptr && printResult)
  //			executeFunc("Print(Var " + pVar->Name + " has value " + pVar->Value + ")", printResult);
  //	}
  //}

  //void	ScriptParser::executeFunc(const std::string& text, const bool printResult) {
  //	auto pos = text.find("(");
  //	if (pos == std::string::npos)
  //		return;

  //	auto name = ClearWhiteSpace(text.substr(0, pos));
  //	auto pFunc = findFunc(name);
  //	if (pFunc == nullptr)
  //		return;

  //	pos++;
  //	auto endpos = text.find(")");
  //	if (endpos == std::string::npos)
  //		return;

  //	std::vector<std::string> params;
  //	SplitString(text.substr(pos, endpos - pos), ",", params);
  //	if (params.size() < pFunc->MinParams)
  //		return;

  //	pFunc->pFunc(pFunc->pUserData, params);

  //	eventManager->addActionEvent(EVENT_ACTION_FUNC_CALL, { cb::fromUtf8(pFunc->Name) });
  //}

  //CScriptVar* ScriptParser::findVar(const std::string& name) const {
  //	for (unsigned i = 0; i < VarList.size(); i++) {
  //		if (VarList[i].Name == name)
  //			return const_cast<CScriptVar*>(&VarList[i]);
  //	}
  //	return nullptr;
  //}

  //CScriptFunc* ScriptParser::findFunc(const std::string& name) const {
  //	for (unsigned i = 0; i < FuncList.size(); i++) {
  //		if (FuncList[i].Name == name)
  //			return const_cast<CScriptFunc*>(&FuncList[i]);
  //	}
  //	return nullptr;
  //}
}