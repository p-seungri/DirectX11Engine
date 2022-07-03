#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, string window_title, string window_class, int width, int height)
{
	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
	{
		return false;
	}

	if (!this->gfx.Initialize(render_window.GetHWND(), width, height))
	{
		return false;
	}

	return true;
}

bool Engine::ProcessMessages()
{
	return this->render_window.ProcessMessages();
}

void Engine::Update()
{
	
	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = keyboard.ReadChar();

	}

	while (!keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();

	}


	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();


	}
}

void Engine::RenderFrame()
{
	gfx.RenderFrame();
}
