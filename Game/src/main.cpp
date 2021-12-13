/*///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Plik:	main.cpp
Autor:	Real_Noname (real_noname@wp.pl)
(C):	CODE RULERS (Real_Noname)
WWW:	www.coderulers.prv.pl
Opis:	Znajduj� tu si� g��wne funkcje kt�re program inicjalizuj�
    , wprawiaj� w ruch i ko�cz�. Wu te� znajduje si� funkcja WinMain.
    Po wi�cej info na temat tworzenia okna zapraszam do Render.cpp/Render.h.
    Potrzebne biblioteki, kt�re trzeba doda� w nowym projekcie: OpenGL32.lib i GLU32.lib

/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////*/
#include <CBCore/Logger.h>

#include "Application.h"

int WinMain(void* hInstance,			// Instancja
            void* hPrevInstance,		// Poprzednia instancja
            char* lpCmdLine,			// Parametry
            int			nCmdShow)			// Tryb widoczno�ci okna
{
  auto logger = cb::Logger::getInstance();
  logger->addStream(std::make_shared<std::wofstream>("main.log"));

  e3dt::Application app;
  return app.exec();
}
