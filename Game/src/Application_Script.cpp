
#include <StrEx.h>

#include "Application.h"

namespace e3dt {
	void	Application::RegScript() {
		scriptParser->addFunc(L"loadLevel", [this](const core::StrArgList& args) { LoadLevel(args.getArgUtf8(0)); });
		scriptParser->addFunc(L"print", [this](const core::StrArgList& args) { Print(args.getArgUtf8(0)); });
	}
}