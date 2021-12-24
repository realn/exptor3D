/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	gui.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Patrz -> gio.h

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <CBSDL/Consts.h>
#include <CBGL/COpenGL.h>
#include <CBGL/State.h>

#include "GUIRenderContext.h"
#include "gui_Screen.h"
#include "gui_Element.h"
#include "event_Event.h"

#include "gui.h"
#include "StrEx.h"

//const std::string ConFunc[] = { "Speed", "MotionBlur", "MBKeyFrames", "Reflection",
//								"RefLevel", "WireFrame", "Note", "SaveRndInfo",
//								"SearchExt", "GetRndInfo", "GetExtList", "LoadLevel",
//								"EyeAngle", "TexDLevel", "UpTexDLevel", "DownTexDLevel",
//								"Cliping", "EnableEngine", "DisableEngine", "Quit",
//								"EnableGUI", "DisableGUI", "EnableFullScreen", "DisableFullScreen",
//								"Smoke", "MaxSpecial", "ShowAITarget" };
//const int ConFuncCount = 27;

/*	KLASA Main
	To mia³a byæ tylko ma³a klasa poœrednicz¹ca pomiêdzy
	Graczem a Gr¹, a zrobi³ siê z niej g³ówny system
	steruj¹cy wszystkim. Tutaj dochodz¹ wszystkie metody,
	tutaj zarz¹dza siê wszystkim i tutaj zakodowana jest konsola.
	Wiem, wiem, mo¿na to by³o inaczej zrobiæ :/
*/

namespace gui {
	Main::Main(gfx::TextureRepository& texManager, std::shared_ptr<logic::ScriptParser> scriptParser, glm::vec2 screenSize) 
		: core::Object(L"GUIMain")
		, ScriptParser(scriptParser)
		, screenSize(screenSize)
		, cursorX(0)
		, cursorY(0) 
	{
		textPrinter = std::make_shared<TextPrinter>(texManager.Get("Font.tga"));

		//Log.Log( "Inicjalizacja GUI" );
		Cursor = texManager.Get("Kursor.tga");
		CH = texManager.Get("cel.tga");

		menu = std::make_shared<MenuMain>(getAspectRatio(), scriptParser);
		menu->load(L"Data/menu.mnu", textPrinter->getFontInfo());
		//screen.load(L"Data/screen.gui", textPrinter.getFontInfo());

		log(L"adding event mappings.");
		mapper.addAction(L"gui_move_up", [this](const event::EventAction&) { eventProcess(GuiEventType::Up); });
		mapper.addAction(L"gui_move_down", [this](const event::EventAction&) { eventProcess(GuiEventType::Down); });
		mapper.addAction(L"gui_enter", [this](const event::EventAction&) { eventProcess(GuiEventType::Enter); });
		mapper.addAction(L"gui_exit", [this](const event::EventAction&) { eventProcess(GuiEventType::Back); });
		mapper.addRange(L"gui_pointer_x", [this](const event::EventRange& r) { eventPointerMoveX(r.getValue()); });
		mapper.addRange(L"gui_pointer_y", [this](const event::EventRange& r) { eventPointerMoveY(r.getValue()); });
		report(L"mappings added.");
	}


	/*	Pierwsza metoda wykonuje w³aœciwy silnik GUI, a druga renderuje GUI.
		W³aœciwego silnika nie da siê wy³¹czyæ
	*/
	void Main::update(float timeDelta) {
		if (screenStack.empty())
			return;

		screenStack.back()->update(timeDelta);
		//Console.Update(timeDelta);
	}

	void Main::render() const {
		auto blendState = cb::gl::getBlendState();
		auto cullState = cb::gl::getCullState();
		auto depthState = cb::gl::getDepthState();

		blendState.enabled = true;
		blendState.setFunc(cb::gl::BlendFactor::SRC_ALPHA, cb::gl::BlendFactor::ONE_MINUS_SRC_ALPHA);

		cullState.enabled = false;
		depthState.enabled = false;

		cb::gl::setState(blendState);
		cb::gl::setState(cullState);
		cb::gl::setState(depthState);

		auto ctx = RenderContext();
		ctx.setProjectionMatrix(glm::ortho(0.0f, screenSize.x, screenSize.y, 0.0f));

		for (auto screen : screenStack) {
			screen->render(ctx, *textPrinter, screenSize);
		}

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glm::value_ptr(ctx.getProjectionMatrix()));
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		ctx.render();
	}

	// DO TESTÓW: metoda bezpoœrednio rysuje na ekranie tekst: NIE PRZEZNACZONA DO U¯YWANIA POZA DEBUG'iem
	void Main::Print(float x, float y, std::string text, float ScaleX, float ScaleY) {
		//TextRender.StartPrint(800.0f, 600.0f);
		//TextRender.SetColor( 1.0f, 1.0f, 1.0f );
		//TextRender.Print( x, y, text, ScaleX, ScaleY );
		//TextRender.EndPrint();
	}

	void	Main::PrintConsole(const std::string& text) {
		//Console.Print(text);
	}

	void Main::eventPointerMoveX(float posX) {
		pointerPos.x = posX;
		eventMouseMove(pointerPos);
	}

	void Main::eventPointerMoveY(float posY) {
		pointerPos.y = posY;
		eventMouseMove(pointerPos);
	}

	void Main::eventMouseMove(const glm::vec2& pos) {
		if (screenStack.empty())
			return;

		auto screenPos = pos * screenSize;
		screenStack.back()->eventPointerMove(screenPos, screenSize);

	}

	void Main::eventProcess(GuiEventType type) {
		if (screenStack.empty())
			return;

		screenStack.back()->eventProcess(type);
	}

	glm::vec2 Main::getScreenSize() const {
		return screenSize;
	}

	float Main::getAspectRatio() const {
		if(screenSize.y == 0.0f)
			return 0.0f;
		return screenSize.x / screenSize.y;
	}
}