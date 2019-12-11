#ifndef DRAWTEXTURE
#define DRAWTEXTURE

#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>

#include"DirectX.h"

#define MAP_SIZE_WIDTH (28)	
#define MAP_SIZE_HEIGHT (20)

class Texture
{
private:


	// マップチップ
	// ----------------------------------------------------------------------------------------
	int   m_map_height_number; // マップチップの横の個数
	int   m_map_widht_number;  // マップチップの縦の個数
	float m_mapChip_height;    // マップチップの一つの幅
	float m_mapChip_widht;     // マップチップの一つの高さ
	float m_texture_height;    // マップチップの画像の幅
	float m_texture_width;     // マップチップの画像の高さ
	float m_draw_x;            // 描画位置(X座標)
	float m_draw_y;            // 描画位置(Y座標)
	float m_chip_x;            // 統合画像から切り取る座標(X座標)
	float m_chip_y;            // 統合画像から切り取る座標(Y座標)
// ------------------------------------------------------------------------------------------


// テクスチャ情報保存用構造体
// ------------------------------------------------------------------------------------------
	typedef struct
	{
		float	// 頂点座標
			x,  // 幅
			y,  // 高さ
			z;  // 奥行

		float   // 除算数
			rhw;

		float   // テクスチャ座標
			tu,  // 幅
			tv;	// 高さ
	} CUSTOMVERTEX, * P_CUSTOMVERTEX;
	// -----------------------------------------------------------------------------------------

	static const int MaxTexture = 20;

public:

	void DrawTexture(float x, float y, float width, float height, float tu, float tv, float tu_width, float tv_height, LPDIRECT3DTEXTURE9* Texture, DirectX directX);

	void LoadTexture(const char* file_name, LPDIRECT3DTEXTURE9* Texture, DirectX* directX);

	void DrawMapChip(DirectX* directX, LPDIRECT3DTEXTURE9* Texture, int MapChipList[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH]);

	void DrawMap(DirectX* directX, LPDIRECT3DTEXTURE9* Texture);


	// テクスチャ保存用構造体
	// ------------------------------------------------------------------------------------------
	struct TEXTUREDATA
	{

		LPDIRECT3DTEXTURE9 m_pTexture[MaxTexture];  // テクスチャ情報
		float m_width;					            // テクスチャの幅
		float m_height;					            // テクスチャの高さ

	};
	// ------------------------------------------------------------------------------------------

	// 座標4つ指定用構造体
	// ------------------------------------------------------------------------------------------
	struct TEXTUREDATAEX
	{
		float X, Y, Z;
		float Tu, Tv;
	};

	// ------------------------------------------------------------------------------------------


	void DrawTextureEx(LPDIRECT3DTEXTURE9* Texture, TEXTUREDATAEX TextureDataEx[], DirectX directX);
};



#endif // !
