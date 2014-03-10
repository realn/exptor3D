#include "GUIMenu.h"

#include "gui.h"

/*======================
	KLASA guiMenuItem
	Jest to klasa reprezentuj¹ca
	wybór w menu.
======================*/
guiMenuItem::guiMenuItem()
{
	Enabled = true;
	Highlight = false;
	Action = "";
	GoTo = -1;
	Color = 0.0f;
	scrollX = 0.0f;
	scrollY = 0.0f;
}

void guiMenuItem::Enable()
{
	Enabled = true;
}

void guiMenuItem::Disable()
{
	Enabled = false;
}

void guiMenuItem::HighLight()
{
	Highlight = true;
}

void guiMenuItem::DontHighLight()
{
	Highlight = false;
}

void guiMenuItem::Update()
{
	if( !Highlight )
	{
		if( Color > 0.0f )
			Color -= 0.01f;
		
		if( scrollX > 0.0f )
			scrollX -= 0.1f;
	}
	else
	{
		if( Color < 0.6f )
			Color += 0.01f;

		if( scrollX < 10.0f )
			scrollX += 0.1f;
	}
}

void guiMenuItem::Render( CTextRenderer *TText )
{
	if( Enabled )
		TText->SetColor( 0.4f + Color, 0.4f + Color, 0.4f + Color );
	else TText->SetColor( 0.2f, 0.2f, 0.2f, 0.5f );
	TText->Print( X + scrollX, Y, this->Caption, 1.5f, 1.5f );
}

void guiMenuItem::DoAction()
{
	GUI.ScriptParser.Execute( Action );
	//GUI.SendConMsg( Action, true, false );
}

void guiMenuItem::DoGoTo()
{
	GUI.Menu.GoToMenu( GoTo );
}

