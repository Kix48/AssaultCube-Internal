#define _USE_MATH_DEFINES
#include <Windows.h>
#include <string>
#include <vector>
#include <math.h>
#include "config.h"
#include "drawing.h"

class GL::Font f;

//Gets resolution of game
Vector2 GL::GetRes()
{
	Vector2 screenRes;

	float viewport[4] = { 0 };
	glGetFloatv(GL_VIEWPORT, viewport);

	screenRes.x = viewport[2];
	screenRes.y = viewport[3];

	return screenRes;
}

//Sets up viewport for drawing
void GL::SetupOrtho()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

//Basically does what the name states
void GL::RestoreGL()
{
	glPopMatrix();
	glPopAttrib();
}

//Draws a line duh
void GL::DrawLine(float startx, float starty, float endx, float endy, float lineWidth, ImVec4 color)
{
	glColor4f(color.x, color.y, color.z, color.w);
	glBegin(GL_LINES);
	glVertex2f(startx, starty);
	glVertex2f(endx, endy);
	glEnd();
}

//Draws a filled rectangle duh
void GL::DrawFilledRect(float x, float y, float x2, float y2, ImVec4 color)
{
	glColor4f(color.x, color.y, color.z, color.w);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x2 , y);
	glVertex2f(x2, y2);
	glVertex2f(x, y2);
	glEnd();
}

//Draws an outlined rectangle duh
void GL::DrawOutline(float x, float y, float x2, float y2, float lineWidth, ImVec4 color)
{
	glLineWidth(lineWidth);
	glColor4f(color.x, color.y, color.z, color.w);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x2, y);
	glVertex2f(x2, y2);
	glVertex2f(x, y2);
	glEnd();
}

//Was gonna use this to draw an fov circle for an fov aimbot but that didn't work out so you can use this to try to make one yourself
void GL::DrawCircle(float x, float y, float radius, int numOfTriangles, float lineWidth, ImVec4 color)
{
	float doublePi = 2 * (FLOAT)M_PI;
	
	glLineWidth(lineWidth);
	glColor4f(color.x, color.y, color.z, color.w);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i <= numOfTriangles; i++)
	{
		float theta = doublePi * i / numOfTriangles;
		float vx = cosf(theta) * radius + x;
		float vy = sinf(theta) * radius + y;

		glVertex2f(vx, vy);
	}

	glEnd();
}

//Builds our font duh
void GL::Font::Build(int height)
{
	hdc = wglGetCurrentDC();
	base = glGenLists(96);
	HFONT hFont = CreateFontA(-height, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Arial");
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
	wglUseFontBitmaps(hdc, 32, 96, base);
	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);

	bBuilt = true;
}

//Prints text duh
void GL::Font::Print(float x, float y, ImVec4 color, const char *format, ...)
{
	HDC fontHDC = wglGetCurrentDC();

	if (fontHDC != f.hdc)
	{
		base = glGenLists(96);
		HFONT hFont = CreateFontA(-Config::fHeight, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Arial");
		HFONT hOldFont = (HFONT)SelectObject(fontHDC, hFont);
		wglUseFontBitmaps(fontHDC, 32, 96, base);
		SelectObject(fontHDC, hOldFont);
		DeleteObject(hFont);
		f.hdc = fontHDC;
	}

	glColor4f(color.x, color.y, color.z, color.w);
	glRasterPos2f(x, y);

	char text[100];
	va_list	args;

	va_start(args, format);
	vsprintf_s(text, 100, format, args);
	va_end(args);

	glPushAttrib(GL_LIST_BIT);
	glListBase(base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

//Centers text duh(Originally used with Rake's OpenGl tut but I scrapped the use of this function
Vector3 GL::Font::centerText(float x, float y, float width, float height, float textWidth, float textHeight)
{
	Vector3 text;
	text.x = x + (width - textWidth) / 2;
	text.y = y + textHeight;
	return text;
}

//Look at the comment above ^^
float GL::Font::centerText(float x, float width, float textWidth)
{
	if (width > textWidth)
	{
		float difference = width - textWidth;
		return (x + (difference / 2));
	}
	else
	{
		float difference = textWidth - width;
		return(x - (difference / 2));
	}
}
