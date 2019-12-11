#include"DrawTexture.h"
#include"Engine.h"

const int D3DFVF_CUSTOMVERTEX(D3DFVF_XYZRHW | D3DFVF_TEX1);


// 描画開始前に必ず Engine.cpp にある前処理の DrawStart 関数を呼び開始処理を追加する
// 描画終了時には必ず Engine.cpp にある後処理の DrawEnd 関数を呼び終了処理を追加する
void Texture::DrawTexture(float x, float y, float width, float height, float tu, float tv, float tu_width, float tv_height, LPDIRECT3DTEXTURE9* Texture, DirectX directX)
{
	CUSTOMVERTEX customvertex[4] = {
		{ x         , y          , 0 , 1 , tu       , tv        },
		{ x + width , y          , 0 , 1 , tu_width , tv        },
		{ x + width , y + height , 0 , 1 , tu_width , tv_height },
		{ x         , y + height , 0 , 1 , tu       , tv_height },
	};

	directX.pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	directX.pDevice->SetTexture(0, *Texture);
	directX.pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, customvertex, sizeof(CUSTOMVERTEX));

};

void Texture::DrawTextureEx(LPDIRECT3DTEXTURE9* Texture, TEXTUREDATAEX TextureDataEx[], DirectX directX)
{
	CUSTOMVERTEX customvertex[4] = {
		{ TextureDataEx[0].X , TextureDataEx[0].Y , 0 , 1 , TextureDataEx[0].Tu ,TextureDataEx[0].Tv },
		{ TextureDataEx[1].X , TextureDataEx[1].Y , 0 , 1 , TextureDataEx[1].Tu ,TextureDataEx[1].Tv },
		{ TextureDataEx[2].X , TextureDataEx[2].Y , 0 , 1 , TextureDataEx[2].Tu ,TextureDataEx[2].Tv },
		{ TextureDataEx[3].X , TextureDataEx[3].Y , 0 , 1 , TextureDataEx[3].Tu ,TextureDataEx[3].Tv },
	};

	directX.pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	directX.pDevice->SetTexture(0, *Texture);
	directX.pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, customvertex, sizeof(CUSTOMVERTEX));

};

void Texture::LoadTexture(const char* file_name, LPDIRECT3DTEXTURE9* Texture, DirectX* directX)
{
	HRESULT result = D3DXCreateTextureFromFile(directX->pDevice, file_name, Texture);

	if ((_D3DXERR)result == D3DXERR_INVALIDDATA)
	{
		OutputDebugString(TEXT("\nテクスチャファイルが見つかりません\n"));
	}
}

void Texture::DrawMap(DirectX* directX, LPDIRECT3DTEXTURE9* Texture)
{
	float cy = m_chip_y / m_texture_height;// 高さ
	float cx = m_chip_x / m_texture_width; // 横
	float cx_size = m_mapChip_widht / m_texture_width;
	float cy_size = m_mapChip_height / m_texture_height;

	float cu_add = cx + cx_size;
	float cv_add = cy + cy_size;

	CUSTOMVERTEX sprite[] =
	{
		{ m_draw_x                   , m_draw_y                   , 0.0f, 1.0f , cx      ,cy      },
		{ m_draw_x + m_mapChip_widht , m_draw_y                   , 0.0f, 1.0f , cu_add  ,cy      },
		{ m_draw_x + m_mapChip_widht , m_draw_y + m_mapChip_height, 0.0f, 1.0f , cu_add  ,cv_add  },
		{ m_draw_x                   , m_draw_y + m_mapChip_height, 0.0f, 1.0f , cx      ,cv_add  },
	};

	directX->pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	directX->pDevice->SetTexture(0, *Texture);
	directX->pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, sprite, sizeof(CUSTOMVERTEX));
}

void Texture::DrawMapChip(DirectX* directX, LPDIRECT3DTEXTURE9* Texture, int MapChipList[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH])
{
	for (int i = 0; i < m_map_height_number; i++)
	{
		for (int j = 0; j < m_map_widht_number; j++)
		{
			int chip_id = MapChipList[i][j];

			int width_num = (int)m_texture_width / m_mapChip_widht;
			int height_num = (int)m_texture_height / m_mapChip_height;

			float chip_pos_x = (float)((chip_id % width_num) * m_mapChip_widht);
			float chip_pos_y = (float)((chip_id / height_num) * m_mapChip_height);

			// 描画する場所の指定 // 
			m_chip_x = chip_pos_x;
			m_chip_y = chip_pos_y;
			// チップ指定 //
			m_draw_x = m_mapChip_widht * j + 80;
			m_draw_y = m_mapChip_height * i + 80;

			DrawMap(directX, Texture);

		}

	}

}
