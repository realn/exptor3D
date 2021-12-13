/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	main.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Znajduj¹ tu siê g³ówne funkcje które program inicjalizuj¹
    , wprawiaj¹ w ruch i koñcz¹. Wu te¿ znajduje siê funkcja WinMain.
    Po wiêcej info na temat tworzenia okna zapraszam do Render.cpp/Render.h.
    Potrzebne biblioteki, które trzeba dodaæ w nowym projekcie: OpenGL32.lib i GLU32.lib

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include <CBCore/Logger.h>

#include "Application.h"

int WinMain(void* hInstance,			// Instancja
            void* hPrevInstance,		// Poprzednia instancja
            char* lpCmdLine,			// Parametry
            int			nCmdShow)			// Tryb widocznoœci okna
{
  auto logger = cb::Logger::getInstance();
  logger->addStream(std::make_shared<std::wofstream>("main.log"));

  e3dt::Application app;
  return app.exec();
}
