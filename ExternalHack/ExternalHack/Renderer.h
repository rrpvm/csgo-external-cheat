#pragma once
#include "GlobalVars.h"
#include "Singleton.h"
#include "vec3.h"
#include "vec2.h"
#include "menu.h"
#include "../IMGUI/imgui.h"

class Color
{
public:
	Color();
	Color(int _r, int _g, int _b);
	Color(int _r, int _g, int _b, int _a);
	Color(float _r, float _g, float _b) : Color(_r, _g, _b, 1.0f) {}
	Color(float _r, float _g, float _b, float _a)
		: Color(
			static_cast<int>(_r * 255.0f),
			static_cast<int>(_g * 255.0f),
			static_cast<int>(_b * 255.0f),
			static_cast<int>(_a * 255.0f))
	{
	}
	explicit Color(float* rgb) : Color(rgb[0], rgb[1], rgb[2], 1.0f) {}
	explicit Color(unsigned long argb)
	{
		_CColor[2] = (unsigned char)((argb & 0x000000FF) >> (0 * 8));
		_CColor[1] = (unsigned char)((argb & 0x0000FF00) >> (1 * 8));
		_CColor[0] = (unsigned char)((argb & 0x00FF0000) >> (2 * 8));
		_CColor[3] = (unsigned char)((argb & 0xFF000000) >> (3 * 8));
	}

	void    SetRawColor(int color32);
	int     GetRawColor() const;
	void    SetColor(int _r, int _g, int _b, int _a = 0);
	void    SetColor(float _r, float _g, float _b, float _a = 0);
	void    GetColor(int &_r, int &_g, int &_b, int &_a) const;

	std::string GetNormalnijHexColor() const;

	int r() const { return _CColor[0]; }
	int g() const { return _CColor[1]; }
	int b() const { return _CColor[2]; }
	int a() const { return _CColor[3]; }

	unsigned char &operator[](int index)
	{
		return _CColor[index];
	}
	const unsigned char &operator[](int index) const
	{
		return _CColor[index];
	}

	bool operator==(const Color &rhs) const;
	bool operator!=(const Color &rhs) const;
	Color &operator=(const Color &rhs);

	static Color FromHSB(float hue, float saturation, float brightness)
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 2)
		{
			return Color(
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 3)
		{
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
			);
		}
		else if (h < 4)
		{
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else if (h < 5)
		{
			return Color(
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else
		{
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
			);
		}
	}

	static Color Black;
	static Color White;
	static Color Red;
	static Color Green;
	static Color Blue;

private:
	unsigned char _CColor[4];
};
struct view_matrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}
	float matrix[4][4];
};
class Renderer : public Singleton <Renderer>
{
public:
	Renderer() {};
	~Renderer() {};
	void drawFrames();
	void init() {if(!pDevice) pDevice = GlobalVars::get().pDevice; }
	
	bool world_to_screen(const vec3& screen_size, const vec3& pos, vec3& out, view_matrix_t matrix);
private:
	void drawLine(vec2 pos, vec2 end, D3DCOLOR color);	
	void DrawFilledRect(vec2 pos, vec2 size, D3DCOLOR color);
	void DrawRect(vec2 pos, vec2 size, D3DCOLOR color);
	void DrawCornerBox(vec2 pos, vec2 size, D3DCOLOR color,  float thikness);
	void DrawSecondCornerBox(vec2 pos, vec2 size, D3DCOLOR color,  float thikness);
	void DrawTriangle(vec2 pos, vec2 size, D3DCOLOR color, bool rainbow = false);
	void drawString(vec2 pos, vec2 size, const char* name);
	bool transformCoord(Player* player, view_matrix_t matrix);
	void doBoxEsp(Player* player);
	void scaleCoords(Player* player);
	void HealthEsp(Player* player);
	void drawImCircle(ImVec2 pos, float radius, ImU32 col , int segments = 16, bool filled = false);
	void drawImText(ImFont* font,ImVec2 pos, Color color, const char* txt, bool outline = true, float fontSize = 10.f);
	void drawImRect(ImVec2 from, ImVec2 size, Color color, bool filled = false);
	ImDrawList* backBuffer;
	struct vertex {
		float x, y, z, rhw;
		D3DCOLOR color;
	};
	struct glow_t {
		uintptr_t base;
		float   m_flRed;           //0x0004
		float   m_flGreen;         //0x0008
		float   m_flBlue;          //0x000C
		float   m_flAlpha;                 //0x0010
		uint8_t pad_0014[4];               //0x0014
		float   m_flSomeFloat;             //0x0018
		uint8_t pad_001C[4];               //0x001C
		float   m_flAnotherFloat;          //0x0020
		bool    m_bRenderWhenOccluded;     //0x0024
		bool    m_bRenderWhenUnoccluded;   //0x0025
		bool    m_bFullBloomRender;        //0x0026
		uint8_t pad_0027[5];               //0x0027
		int32_t m_nGlowStyle;              //0x002C
		int32_t m_nSplitScreenSlot;        //0x0030
		int32_t m_nNextFreeSlot;           //0x0034
	};
	ImU32 GetU32(Color _color)
	{
		return ((_color[3] & 0xff) << 24) + ((_color[2] & 0xff) << 16) + ((_color[1] & 0xff) << 8)
			+ (_color[0] & 0xff);
	}
protected:
	IDirect3DDevice9* pDevice;
};

