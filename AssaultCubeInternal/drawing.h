#pragma once
#ifndef DRAWING_H
#define DRAWING_H

#pragma comment(lib, "OpenGL32.lib")
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <gl/GL.h>
#include "offsets.h"
#include "Imgui/imgui.h"

//Namespace with drawing and font functions(Reference Rake's OpenGl tut)
namespace GL
{
	Vector2 GetRes();
	void SetupOrtho();
	void RestoreGL();

	void DrawLine(float startx, float starty, float endx, float endy, float lineWidth, ImVec4 color);
	void DrawFilledRect(float x, float y, float x2, float y2, ImVec4 color);
	void DrawOutline(float x, float y, float x2, float y2, float lineWidth, ImVec4 color);
	void DrawCircle(float x, float y, float radius, int numOfTriangles, float lineWidth, ImVec4 color);

	class Font
	{
	public:
		bool bBuilt = false;
		unsigned int base;
		HDC hdc = nullptr;
		int height;
		int width;

		void Build(int height);
		void Print(float x, float y, ImVec4 color, const char* format, ...);

		Vector3 centerText(float x, float y, float width, float height, float textWidth, float textHeight);
		float centerText(float x, float width, float textWidth);
	};
}

#endif