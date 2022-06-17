#include "Render.h"
#include <iostream>


Render::Render() {
	m_pTextureAtlas = nullptr;
	m_pSprite		= nullptr;

	m_pTriIndex		= nullptr;
	m_pTriVertex	= nullptr;
	m_pLineIndex	= nullptr;
	m_pLineVertex	= nullptr;
	m_pDevice		= nullptr;
	m_iMaxVertices	= 0;
	m_iLineCount		= m_iTriangleCount			= 0;
	m_iLineIndices		= m_iTriangleIndices		= 0;
	m_iLineVertices		= m_iTriangleVertices		= 0;
	m_pLineVertexBuffer		= nullptr;
	m_pLineIndexBuffer		= nullptr;
	m_pTriVertexBuffer		= nullptr;
	m_pTriIndexBuffer		= nullptr;
}
Render::Render(IDirect3DDevice9* pDevice, HMODULE hMod) {
	Initialize(pDevice, hMod);
} 

IDirect3DDevice9* Render::GetDevice() {
	return m_pDevice;
}

void Render::Initialize(IDirect3DDevice9* pDevice, HMODULE hMod) {
	D3DXCreateTextureFromResource(pDevice, hMod, MAKEINTRESOURCE(ICON_ATLAS), &m_pTextureAtlas);
	D3DXCreateSprite(pDevice, &m_pSprite);

	m_pTriIndex		= nullptr;
	m_pTriVertex	= nullptr;
	m_pLineIndex	= nullptr;
	m_pLineVertex	= nullptr;
	m_pDevice		= pDevice;

	m_iMaxVertices	= MAXSHORT;
	m_iLineIndices	= m_iTriangleIndices	= 0;
	m_iLineCount	= m_iTriangleCount		= 0;
	m_iLineVertices = m_iTriangleVertices	= 0;

	//Create buffers to hold lines and triangle meshes

	m_pDevice->CreateVertexBuffer	(m_iMaxVertices * sizeof(CustomVertex)	, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pLineVertexBuffer, NULL);
	m_pDevice->CreateIndexBuffer	(m_iMaxVertices * 2 * sizeof(int)		, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32		, D3DPOOL_DEFAULT, &m_pLineIndexBuffer, NULL);
	m_pDevice->CreateVertexBuffer	(m_iMaxVertices * sizeof(CustomVertex)	, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pTriVertexBuffer, NULL);
	m_pDevice->CreateIndexBuffer	(m_iMaxVertices * 2 * sizeof(int)		, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32		, D3DPOOL_DEFAULT, &m_pTriIndexBuffer, NULL);
}

Render::~Render() {
	Release();

	m_pDevice		= nullptr;
	m_iMaxVertices	= 0;
	m_iLineIndices	= m_iTriangleIndices	= 0;
	m_iLineCount	= m_iTriangleCount		= 0;
	m_iLineVertices = m_iTriangleVertices	= 0;
}

void Render::Release() {
	m_pSprite->Release();
	m_pSprite = nullptr;
	m_pTextureAtlas->Release();
	m_pTextureAtlas = nullptr;
	if (m_pLineVertexBuffer) {
		m_pLineVertexBuffer->Release();
		m_pLineVertexBuffer = nullptr;
	}
	if (m_pLineIndexBuffer) {
		m_pLineIndexBuffer->Release();
		m_pLineIndexBuffer = nullptr;
	}
	if (m_pTriVertexBuffer) {
		m_pTriVertexBuffer->Release();
		m_pTriVertexBuffer = nullptr;
	}
	if (m_pTriIndexBuffer) {
		m_pTriIndexBuffer->Release();
		m_pTriIndexBuffer = nullptr;
	}

}

void Render::Begin(BUFFER_TYPE tBufferType) {
	if (IsInitialized()) {

		m_pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		switch (tBufferType) {
		case BUFFER_TYPE::BUFFER_ALL:
			m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
			m_pLineVertexBuffer->	Lock(0, 0, (void**)&m_pLineVertex, D3DLOCK_DISCARD);
			m_pLineIndexBuffer->	Lock(0, 0, (void**)&m_pLineIndex, D3DLOCK_DISCARD);
			m_pTriVertexBuffer->	Lock(0, 0, (void**)&m_pTriVertex, D3DLOCK_DISCARD);
			m_pTriIndexBuffer->		Lock(0, 0, (void**)&m_pTriIndex, D3DLOCK_DISCARD);
			break;
		case BUFFER_TYPE::BUFFER_LINE:
			m_pLineVertexBuffer->	Lock(0, 0, (void**)&m_pLineVertex, D3DLOCK_DISCARD);
			m_pLineIndexBuffer->	Lock(0, 0, (void**)&m_pLineIndex, D3DLOCK_DISCARD);
			break;
		case BUFFER_TYPE::BUFFER_TRI:
			m_pTriVertexBuffer->	Lock(0, 0, (void**)&m_pTriVertex, D3DLOCK_DISCARD);
			m_pTriIndexBuffer->		Lock(0, 0, (void**)&m_pTriIndex, D3DLOCK_DISCARD);
			break;
		case BUFFER_TYPE::BUFFER_TEXT:
			m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
			break;
		}

	}
}



void Render::End(BUFFER_TYPE tBufferType) {
	if (IsInitialized()) {

		m_pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		
		switch (tBufferType) {
		case BUFFER_TYPE::BUFFER_ALL:
			//Draw both line and triangle batch
			m_pLineVertexBuffer->	Unlock();
			m_pLineIndexBuffer->	Unlock();
			m_pTriVertexBuffer->	Unlock();
			m_pTriIndexBuffer->		Unlock();

			m_pDevice->SetStreamSource(0, m_pTriVertexBuffer, 0, sizeof(CustomVertex));
			m_pDevice->SetIndices(m_pTriIndexBuffer);

			m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iTriangleVertices, 0, m_iTriangleCount);
			m_iTriangleCount = m_iTriangleIndices = m_iTriangleVertices = 0;

			m_pDevice->SetStreamSource(0, m_pLineVertexBuffer, 0, sizeof(CustomVertex));
			m_pDevice->SetIndices(m_pLineIndexBuffer);

			m_pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_iLineVertices, 0, m_iLineCount);
			m_iLineCount = m_iLineIndices = m_iLineVertices = 0;


			m_pSprite->End();

			break;
		case BUFFER_TYPE::BUFFER_LINE:
			//Draw line batch
			m_pLineVertexBuffer->	Unlock();
			m_pLineIndexBuffer->	Unlock();

			m_pDevice->SetStreamSource(0, m_pLineVertexBuffer, 0, sizeof(CustomVertex));
			m_pDevice->SetIndices(m_pLineIndexBuffer);

			m_pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_iLineVertices, 0, m_iLineCount);
			m_iLineCount = m_iLineIndices = m_iLineVertices = 0;

			break;
		case BUFFER_TYPE::BUFFER_TRI:
			//Draw traingle batch
			m_pTriVertexBuffer->Unlock();
			m_pTriIndexBuffer->	Unlock();

			m_pDevice->SetStreamSource(0, m_pTriVertexBuffer, 0, sizeof(CustomVertex));
			m_pDevice->SetIndices(m_pTriIndexBuffer);

			m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iTriangleVertices, 0, m_iTriangleCount);
			m_iTriangleCount = m_iTriangleIndices = m_iTriangleVertices = 0;

			break;
		case BUFFER_TYPE::BUFFER_TEXT:
			m_pSprite->End();
			break;
		}

		

	}
	
	
}

bool Render::IsInitialized() {
	return m_pDevice && m_pLineVertexBuffer && m_pLineIndexBuffer && m_pTriIndexBuffer && m_pTriVertexBuffer;
}

void Render::ManageBatch(D3DPRIMITIVETYPE tPrimitiveType, unsigned int iNeededVerts, unsigned int iNeededIndices) {
	//Ensure there is enough room in the buffer for the next primitive, empty if needed
	switch (tPrimitiveType) {
	case D3DPT_LINELIST:
		if (m_iLineIndices + iNeededIndices > m_iMaxVertices * 2 || m_iLineVertices + iNeededVerts > m_iMaxVertices) {
			End(BUFFER_TYPE::BUFFER_LINE);
			Begin(BUFFER_TYPE::BUFFER_LINE);
		}

		break;
	case D3DPT_TRIANGLELIST:
		if (m_iTriangleIndices + iNeededIndices > m_iMaxVertices * 2 || m_iTriangleVertices + iNeededVerts > m_iMaxVertices) {
			End(BUFFER_TYPE::BUFFER_TRI);
			Begin(BUFFER_TYPE::BUFFER_TRI);
		}

		break;
	}
}
unsigned int Render::AddVertex(D3DPRIMITIVETYPE tPrimitiveType, Vector2D vLocation, D3DCOLOR cColor) {

	switch (tPrimitiveType) {
	case D3DPT_LINELIST:
		m_pLineVertex[m_iLineVertices] = { vLocation.x, vLocation.y, 0, 1, cColor };
		m_iLineVertices++;
		return m_iLineVertices - 1;
	case D3DPT_TRIANGLELIST:
		m_pTriVertex[m_iTriangleVertices] = { vLocation.x, vLocation.y, 0, 1, cColor };
		m_iTriangleVertices++;
		return m_iTriangleVertices - 1;
	default:
		return UINT_MAX;
	}

}

void Render::AddIndex(D3DPRIMITIVETYPE tPrimitiveType, unsigned int iVertexPos) {

	switch (tPrimitiveType) {
	case D3DPT_LINELIST:
		m_pLineIndex[m_iLineIndices] = iVertexPos;
		m_iLineIndices++;
		break;
	case D3DPT_TRIANGLELIST:
		m_pTriIndex[m_iTriangleIndices] = iVertexPos;
		m_iTriangleIndices++;
		break;
	}

}

void Render::AddTriangle(unsigned int iVertexPosOne, unsigned int iVertexPosTwo, unsigned int iVertexPosThree) {
	AddIndex(D3DPT_TRIANGLELIST, iVertexPosOne);
	AddIndex(D3DPT_TRIANGLELIST, iVertexPosTwo);
	AddIndex(D3DPT_TRIANGLELIST, iVertexPosThree);
	m_iTriangleCount++;
}




HRESULT Render::DrawSprite(D3DXVECTOR4 vRect, Vector2D vLocation, D3DCOLOR cColor, float flScale, float flRotation) {
	//Transform sprite onto the scene

	RECT rRect = { vRect.x, vRect.y, vRect.z + vRect.x, vRect.w + vRect.y };
	D3DXMATRIX mMatrix, mOldMatrix;
	m_pSprite->GetTransform(&mOldMatrix);
	D3DXVECTOR3 vPos = { vLocation.x, vLocation.y, 0 };

	D3DXVECTOR2 vCenter = { vRect.z * flScale / 2, vRect.w * flScale / 2 };
	D3DXVECTOR2 vScale = { flScale, flScale };
	D3DXVECTOR2 vLoc = { vLocation.x, vLocation.y };

	D3DXMatrixTransformation2D(&mMatrix, NULL, 0.0, &vScale, &vCenter, flRotation, &vLoc);
	m_pSprite->SetTransform(&mMatrix);

	HRESULT hResult = m_pSprite->Draw(m_pTextureAtlas, &rRect, NULL, NULL, cColor);
	m_pSprite->SetTransform(&mOldMatrix);
	return hResult;


}

HRESULT Render::DrawLine(Vector2D vLocationOne, Vector2D vLocationTwo, D3DCOLOR cColor, D3DCOLOR cColor2) {
	if (IsInitialized()) {
		ManageBatch(D3DPT_LINELIST, 2, 2);

		AddIndex(D3DPT_LINELIST, AddVertex(D3DPT_LINELIST, vLocationOne, cColor));
		AddIndex(D3DPT_LINELIST, AddVertex(D3DPT_LINELIST, vLocationTwo, cColor2 ? cColor2 : cColor));
		m_iLineCount++;
		return S_OK;
	}
	return OLE_E_BLANK;

}

HRESULT Render::DrawRectangle(D3DXVECTOR4 vBounds, D3DCOLOR cColor, D3DCOLOR cColor2, bool bVertical) {
	if (IsInitialized()) {
		ManageBatch(D3DPT_TRIANGLELIST, 4, 6);
		vBounds.w += vBounds.y;
		vBounds.z += vBounds.x;
		unsigned int v1, v2, v3, v4;
		cColor2 = cColor2 ? cColor2 : cColor;
		v1 = AddVertex(D3DPT_TRIANGLELIST, { vBounds.x, vBounds.w }, bVertical ? cColor2 : cColor);
		v2 = AddVertex(D3DPT_TRIANGLELIST, { vBounds.x, vBounds.y }, cColor);
		v3 = AddVertex(D3DPT_TRIANGLELIST, { vBounds.z, vBounds.w }, cColor2);
		v4 = AddVertex(D3DPT_TRIANGLELIST, { vBounds.z, vBounds.y }, bVertical ? cColor : cColor2);
		AddTriangle(v1, v2, v3);
		AddTriangle(v2, v4, v3);
		return S_OK;

	}
	return OLE_E_BLANK;
}
HRESULT Render::DrawOutlinedRect(D3DXVECTOR4 vBounds, unsigned int iThickness, D3DCOLOR cOutline, D3DCOLOR cColor, D3DCOLOR cColor2, bool bVertical) {
	if (IsInitialized()) {
		DrawRectangle(vBounds, cOutline);
		vBounds.x += iThickness; vBounds.y += iThickness; vBounds.z -= iThickness * 2, vBounds.w -= iThickness * 2;
		DrawRectangle(vBounds, cColor, cColor2, bVertical);
		return S_OK;

	}
	return OLE_E_BLANK;
}
HRESULT Render::DrawRoundedRectangle(D3DXVECTOR4 vBounds, float flCornerSize, D3DCOLOR cColor, D3DCOLOR cColor2, bool bVertical) {

	if (IsInitialized()) {

		if (bVertical) {
			if (cColor2) {
				D3DCOLOR cLerpOne = LerpColor(cColor, cColor2, flCornerSize * 2 / vBounds.w);
				D3DCOLOR cLerpTwo = LerpColor(cColor2, cColor, flCornerSize * 2 / vBounds.w);
				DrawFadingCircle({ vBounds.x + flCornerSize, vBounds.y + flCornerSize }, flCornerSize, ROUND_CORNER_LOD, cColor, cLerpOne, bVertical, .25, 180);
				DrawFadingCircle({ vBounds.x + vBounds.z - flCornerSize, vBounds.y + flCornerSize }, flCornerSize, ROUND_CORNER_LOD, cColor, cLerpOne, bVertical, .25, 270);
				DrawFadingCircle({ vBounds.x + flCornerSize, vBounds.y + vBounds.w - flCornerSize }, flCornerSize, ROUND_CORNER_LOD, cLerpTwo, cColor2, bVertical, .25, 90);
				DrawFadingCircle({ vBounds.x + vBounds.z - flCornerSize, vBounds.y + vBounds.w - flCornerSize }, flCornerSize, ROUND_CORNER_LOD, cLerpTwo, cColor2, bVertical, .25, 0);
				DrawRectangle({ vBounds.x, vBounds.y + flCornerSize, vBounds.z, vBounds.w - flCornerSize * 2 }, LerpColor(cColor, cColor2, flCornerSize / vBounds.w), LerpColor(cColor2, cColor, flCornerSize / vBounds.w), bVertical);

			}
			else {
				DrawCircle({ vBounds.x + flCornerSize, vBounds.y + flCornerSize }, flCornerSize, ROUND_CORNER_LOD, cColor, .25, 180);
				DrawCircle({ vBounds.x + vBounds.z - flCornerSize, vBounds.y + flCornerSize }, flCornerSize, ROUND_CORNER_LOD, cColor, .25, 270);
				DrawCircle({ vBounds.x + flCornerSize, vBounds.y + vBounds.w - flCornerSize }, flCornerSize, ROUND_CORNER_LOD, cColor, .25, 90);
				DrawCircle({ vBounds.x + vBounds.z - flCornerSize, vBounds.y + vBounds.w - flCornerSize }, flCornerSize, ROUND_CORNER_LOD, cColor, .25, 0);
				DrawRectangle({ vBounds.x, vBounds.y + flCornerSize, vBounds.z, vBounds.w - flCornerSize * 2 }, cColor, cColor2, bVertical);

			}
			DrawRectangle({ vBounds.x + flCornerSize, vBounds.y, vBounds.z - flCornerSize * 2, vBounds.w }, cColor, cColor2, bVertical);
		}
		else {
			D3DCOLOR cLerpOne = LerpColor(cColor, cColor2, flCornerSize * 2 / vBounds.z);
			D3DCOLOR cLerpTwo = LerpColor(cColor2, cColor, flCornerSize * 2 / vBounds.z);
			DrawFadingCircle({ vBounds.x + flCornerSize, vBounds.y + flCornerSize }, flCornerSize, ROUND_CORNER_LOD, cColor, cLerpOne, bVertical, .25, 180);
			DrawFadingCircle({ vBounds.x + vBounds.z - flCornerSize, vBounds.y + flCornerSize }, flCornerSize, ROUND_CORNER_LOD, cLerpTwo, cColor2, bVertical, .25, 270);
			DrawFadingCircle({ vBounds.x + flCornerSize, vBounds.y + vBounds.w - flCornerSize }, flCornerSize, ROUND_CORNER_LOD, cColor, cLerpOne, bVertical, .25, 90);
			DrawFadingCircle({ vBounds.x + vBounds.z - flCornerSize, vBounds.y + vBounds.w - flCornerSize }, flCornerSize, ROUND_CORNER_LOD, cLerpTwo, cColor2, bVertical, .25, 0);

			DrawRectangle({ vBounds.x + flCornerSize, vBounds.y, vBounds.z - flCornerSize * 2, vBounds.w }, LerpColor(cColor, cColor2, flCornerSize / vBounds.z), LerpColor(cColor2, cColor, flCornerSize / vBounds.z), bVertical);
			DrawRectangle({ vBounds.x, vBounds.y + flCornerSize, vBounds.z, vBounds.w - flCornerSize * 2 }, cColor, cColor2, bVertical);
		}

		return S_OK;
	}
	return OLE_E_BLANK;



}
HRESULT Render::DrawFadingCircle(Vector2D vLocation, float flRadius, unsigned int iSides, D3DCOLOR cColor, D3DCOLOR cColor2, bool bVertical, float flFraction, float flRotation) {
	if (IsInitialized()) {
		ManageBatch(D3DPT_TRIANGLELIST, iSides + 2, iSides * 3);
		float flCos = cosf(2 * PI / iSides * flFraction);
		float flSin = sinf(2 * PI / iSides * flFraction);
		float flFracCos = cosf(flRotation * DEG_TO_RAD);
		float flFracSin = sinf(flRotation * DEG_TO_RAD);
		float flR = flRadius * flFracCos;
		float flD = flRadius * flFracSin;
		float flTemp;
		unsigned int iCenter = AddVertex(D3DPT_TRIANGLELIST, { vLocation.x, vLocation.y }, LerpColor(cColor, cColor2, .5));
		unsigned int iPrev = AddVertex(D3DPT_TRIANGLELIST, { vLocation.x + flR, vLocation.y + flD }, LerpColor(cColor, cColor2, (bVertical ? flD : flR) / flRadius));


		for (unsigned int i = 0; i < iSides; i++) {
			flTemp = flR;
			flR = flCos * flR - flSin * flD;
			flD = flSin * flTemp + flCos * flD;

			unsigned int iTemp = AddVertex(D3DPT_TRIANGLELIST, { vLocation.x + flR, vLocation.y + flD }, LerpColor(cColor, cColor2, (bVertical ? flD : flR) / flRadius));
			AddTriangle(iTemp, iCenter, iPrev);

			iPrev = iTemp;

		}

		return S_OK;

	}
	return OLE_E_BLANK;
}

HRESULT Render::DrawCircleOutline(Vector2D vLocation, float flRadius, unsigned int iSides, D3DCOLOR cColor, float flFraction, float flRotation) {
	if (IsInitialized()) {
		ManageBatch(D3DPT_LINELIST, iSides + 1, iSides * 2);
		float flCos = cosf(2 * PI / iSides * flFraction);
		float flSin = sinf(2 * PI / iSides * flFraction);
		float flFracCos = cosf(flRotation * DEG_TO_RAD);
		float flFracSin = sinf(flRotation * DEG_TO_RAD);
		float flR = flRadius * flFracCos;
		float flD = flRadius * flFracSin;
		float flTemp;


		unsigned int iPrev = AddVertex(D3DPT_LINELIST, { vLocation.x + flR, vLocation.y + flD }, cColor);


		for (unsigned int i = 0; i < iSides; i++) {
			flTemp = flR;
			flR = flCos * flR - flSin * flD;
			flD = flSin * flTemp + flCos * flD;

			unsigned int iTemp = AddVertex(D3DPT_LINELIST, { vLocation.x + flR, vLocation.y + flD }, cColor);

			AddIndex(D3DPT_LINELIST, iPrev);
			AddIndex(D3DPT_LINELIST, iTemp);
			m_iLineCount++;


			iPrev = iTemp;

		}

		return S_OK;

	}
	return OLE_E_BLANK;
}

HRESULT Render::DrawCircle(Vector2D vLocation, float flRadius, unsigned int iSides, D3DCOLOR cColor, float flFraction, float flRotation) {
	if (IsInitialized()) {
		ManageBatch(D3DPT_TRIANGLELIST, iSides + 2, iSides * 3);
		float flCos = cosf(2 * PI / iSides * flFraction);
		float flSin = sinf(2 * PI / iSides * flFraction);
		float flFracCos = cosf(flRotation * DEG_TO_RAD);
		float flFracSin = sinf(flRotation * DEG_TO_RAD);
		float flR = flRadius * flFracCos;
		float flD = flRadius * flFracSin;
		float flTemp;

		unsigned int iCenter = AddVertex(D3DPT_TRIANGLELIST, { vLocation.x, vLocation.y }, cColor);
		unsigned int iPrev = AddVertex(D3DPT_TRIANGLELIST, { vLocation.x + flR, vLocation.y + flD }, cColor);


		for (unsigned int i = 0; i < iSides; i++) {
			flTemp = flR;
			flR = flCos * flR - flSin * flD;
			flD = flSin * flTemp + flCos * flD;

			unsigned int iTemp = AddVertex(D3DPT_TRIANGLELIST, { vLocation.x + flR, vLocation.y + flD }, cColor);
			AddTriangle(iTemp, iCenter, iPrev);


			iPrev = iTemp;

		}

		return S_OK;

	}
	return OLE_E_BLANK;
}

HRESULT Render::DrawString(Vector2D vLocation, D3DCOLOR cColor, ID3DXFont* pFont, const char* szString, ...) {

	RECT pRect;

	pRect.left = (long)vLocation.x;
	pRect.top = (long)vLocation.y;
	return pFont->DrawTextA(m_pSprite, szString, -1, &pRect, DT_NOCLIP, cColor);
}
HRESULT Render::DrawString(Vector2D vLocation, D3DCOLOR cColor, ID3DXFont* pFont, const WCHAR* szString, ...) {

	RECT pRect;

	pRect.left = (long)vLocation.x;
	pRect.top = (long)vLocation.y;
	return pFont->DrawTextW(m_pSprite, szString, -1, &pRect, DT_NOCLIP, cColor);
}

Vector2D Render::GetStringSize(ID3DXFont* pFont, const WCHAR* szString, ...) {
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.bottom = 0;
	rect.right = 0;
	pFont->DrawTextW(NULL, szString, -1, &rect, DT_CALCRECT, WHITE);
	return { (float)rect.right, (float)rect.bottom };
}
Vector2D Render::GetStringSize(ID3DXFont* pFont, const char* szString, ...) {

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.bottom = 0;
	rect.right = 0;
	pFont->DrawTextA(NULL, szString, -1, &rect, DT_CALCRECT, WHITE);
	return { (float)rect.right, (float)rect.bottom };
}


D3DCOLOR LerpColor(D3DCOLOR cColorOne, D3DCOLOR cColorTwo, float flPercent) {
	if (flPercent <= 0) {
		return cColorOne;
	}
	else if (flPercent >= 1) {
		return cColorTwo;
	}

	byte a1 = (cColorOne >> 24);
	byte a2 = (cColorTwo >> 24);
	byte r1 = (cColorOne >> 16);
	byte r2 = (cColorTwo >> 16);
	byte g1 = (cColorOne >> 8);
	byte g2 = (cColorTwo >> 8);
	byte b1 = (cColorOne);
	byte b2 = (cColorTwo);
	return (byte)((a2 - a1) * flPercent + a1) << 24 |
		(byte)((r2 - r1) * flPercent + r1) << 16 |
		(byte)((g2 - g1) * flPercent + g1) << 8 |
		(byte)((b2 - b1) * flPercent + b1);

}
D3DCOLOR LerpAlpha(D3DCOLOR cColor, float flPercent, bool bToZero) {
	if (flPercent >= 1) {
		return bToZero ? EMPTY : cColor;
	}
	if (bToZero) {
		return (cColor & 0x00FFFFFF) + ((byte)((1 - flPercent) * 0x000000FF) << 24);
	}

	return (cColor & 0x00FFFFFF) + ((byte)(flPercent * 0x000000FF) << 24);
}