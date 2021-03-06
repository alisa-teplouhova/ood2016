#include "stdafx.h"
#include "ModernGraphicsRenderer.h"

using namespace modern_graphics_lib;
using namespace std;

CModernGraphicsRenderer::CModernGraphicsRenderer(ostream & strm) 
	: m_out(strm)
{
}

CModernGraphicsRenderer::~CModernGraphicsRenderer()
{
	if (m_drawing) // ��������� ���������, ���� ��� ���� ������
	{
		EndDraw();
	}
}

// ���� ����� ������ ���� ������ � ������ ���������
void CModernGraphicsRenderer::BeginDraw()
{
	if (m_drawing)
	{
		throw logic_error("Drawing has already begun");
	}
	m_out << "<draw>" << endl;
	m_drawing = true;
}

// ��������� ��������� �����
void CModernGraphicsRenderer::DrawLine(const CPoint & start, const CPoint & end, const CRGBAColor & color)
{
	if (!m_drawing)
	{
		throw logic_error("DrawLine is allowed between BeginDraw()/EndDraw() only");
	}
	m_out << boost::format(R"(  <line fromX="%1%" fromY="%2%" toX="%3%" toY="%4%">)") % start.x % start.y % end.x % end.y << endl;
	m_out << boost::format(R"(    <color r="%1$.3f" g="%2$.3f" b="%3$.3f" a="%4$.3f" />)") % color.r % color.g % color.b % color.a << endl;
	m_out << "  </line>" << endl;
}

// ���� ����� ������ ���� ������ � ����� ���������
void CModernGraphicsRenderer::EndDraw()
{
	if (!m_drawing)
	{
		throw logic_error("Drawing has not been started");
	}
	m_out << "</draw>" << endl;
	m_drawing = false;
}
