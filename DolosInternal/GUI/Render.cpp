#include "Render.h"


Render::Render() {
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

	m_pDevice->CreateVertexBuffer	(m_iMaxVertices * sizeof(CustomVertex), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pLineVertexBuffer, NULL);
	m_pDevice->CreateIndexBuffer	(m_iMaxVertices * 2 * sizeof(int), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_pLineIndexBuffer, NULL);
	m_pDevice->CreateVertexBuffer	(m_iMaxVertices * sizeof(CustomVertex), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pTriVertexBuffer, NULL);
	m_pDevice->CreateIndexBuffer	(m_iMaxVertices * 2 * sizeof(int), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_pTriIndexBuffer, NULL);
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

	m_pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	m_pDevice->SetRenderState(D3DRS_CLIPPING, TRUE);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	switch (tBufferType) {
	case BUFFER_ALL:
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		m_pLineVertexBuffer->Lock(0, 0, (void**)&m_pLineVertex, D3DLOCK_DISCARD);
		m_pLineIndexBuffer->Lock(0, 0, (void**)&m_pLineIndex, D3DLOCK_DISCARD);
		m_pTriVertexBuffer->Lock(0, 0, (void**)&m_pTriVertex, D3DLOCK_DISCARD);
		m_pTriIndexBuffer->Lock(0, 0, (void**)&m_pTriIndex, D3DLOCK_DISCARD);
		break;
	case BUFFER_LINE:
		m_pLineVertexBuffer->Lock(0, 0, (void**)&m_pLineVertex, D3DLOCK_DISCARD);
		m_pLineIndexBuffer->Lock(0, 0, (void**)&m_pLineIndex, D3DLOCK_DISCARD);
		break;
	case BUFFER_TRI:
		m_pTriVertexBuffer->Lock(0, 0, (void**)&m_pTriVertex, D3DLOCK_DISCARD);
		m_pTriIndexBuffer->Lock(0, 0, (void**)&m_pTriIndex, D3DLOCK_DISCARD);
		break;
	case BUFFER_TEXT:
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		break;
	}


}



void Render::End(BUFFER_TYPE tBufferType) {
	if (IsInitialized()) {

		m_pDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		
		switch (tBufferType) {
		case BUFFER_ALL:
			m_pLineVertexBuffer->Unlock();
			m_pLineIndexBuffer->Unlock();
			m_pTriVertexBuffer->Unlock();
			m_pTriIndexBuffer->Unlock();

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
		case BUFFER_LINE:
			m_pLineVertexBuffer->Unlock();
			m_pLineIndexBuffer->Unlock();

			m_pDevice->SetStreamSource(0, m_pLineVertexBuffer, 0, sizeof(CustomVertex));
			m_pDevice->SetIndices(m_pLineIndexBuffer);

			m_pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, m_iLineVertices, 0, m_iLineCount);
			m_iLineCount = m_iLineIndices = m_iLineVertices = 0;

			break;
		case BUFFER_TRI:
			m_pTriVertexBuffer->Unlock();
			m_pTriIndexBuffer->Unlock();

			m_pDevice->SetStreamSource(0, m_pTriVertexBuffer, 0, sizeof(CustomVertex));
			m_pDevice->SetIndices(m_pTriIndexBuffer);

			m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iTriangleVertices, 0, m_iTriangleCount);
			m_iTriangleCount = m_iTriangleIndices = m_iTriangleVertices = 0;

			break;
		case BUFFER_TEXT:
			m_pSprite->End();
			break;
		}



	}
}

bool Render::IsInitialized() {
	return m_pDevice && m_pLineVertexBuffer && m_pLineIndexBuffer && m_pTriIndexBuffer && m_pTriVertexBuffer;
}

void Render::ManageBatch(D3DPRIMITIVETYPE tPrimitiveType, int iNeededVerts, int iNeededIndices) {

	switch (tPrimitiveType) {
	case D3DPT_LINELIST:
		if (m_iLineIndices + iNeededIndices > m_iMaxVertices * 2 || m_iLineVertices + iNeededVerts > m_iMaxVertices) {
			End(BUFFER_LINE);
			Begin(BUFFER_LINE);
		}

		break;
	case D3DPT_TRIANGLELIST:
		if (m_iTriangleIndices + iNeededIndices > m_iMaxVertices * 2 || m_iTriangleVertices + iNeededVerts > m_iMaxVertices) {
			End(BUFFER_TRI);
			Begin(BUFFER_TRI);
		}

		break;
	}
}
unsigned int Render::AddVertex(D3DPRIMITIVETYPE tPrimitiveType, D3DXVECTOR2 vLocation, D3DCOLOR cColor) {

	switch (tPrimitiveType) {
	case D3DPT_LINELIST:
		m_pLineVertex[m_iLineVertices] = { vLocation.x, vLocation.y, 0, 1, cColor };
		m_iLineVertices++;
		return m_iLineVertices - 1;
		break;
	case D3DPT_TRIANGLELIST:
		m_pTriVertex[m_iTriangleVertices] = { vLocation.x, vLocation.y, 0, 1, cColor };
		m_iTriangleVertices++;
		return m_iTriangleVertices - 1;
		break;
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

D3DCOLOR Render::LerpColor(D3DCOLOR cColorOne, D3DCOLOR cColorTwo, float flPercent) {
	if (flPercent <= 0) {
		return cColorOne;
	}
	if (flPercent >= 1) {
		return cColorTwo;
	}

	byte a1 = (cColorOne >> 24);
	byte a2 = (cColorOne >> 24);
	byte r1 = (cColorOne >> 16);
	byte r2 = (cColorOne >> 16);
	byte g1 = (cColorOne >> 8);
	byte g2 = (cColorOne >> 8);
	byte b1 = (cColorOne);
	byte b2 = (cColorOne);
	return (int)((a2 - a1) * flPercent + a1) << 24 |
		(int)((r2 - r1) * flPercent + r1) << 16 |
		(int)((g2 - g1) * flPercent + g1) << 8 |
		(int)((b2 - b1) * flPercent + b1);

}
D3DCOLOR Render::LerpAlpha(D3DCOLOR cColor, float flPercent, bool bToZero) {
	if (flPercent >= 1) {
		return bToZero ? EMPTY : cColor;
	}
	if (bToZero) {
		return (cColor & 0x00FFFFFF) + (1 - flPercent) * 0xFF000000;
	}
	return (cColor & 0x00FFFFFF) + flPercent * 0xFF000000;
}


HRESULT Render::DrawSprite(D3DXVECTOR4 vRect, D3DXVECTOR2 vLocation, D3DCOLOR cColor, float flScale, float flRotation) {
	RECT rRect = { vRect.x, vRect.y, vRect.z + vRect.x, vRect.w + vRect.y };
	D3DXMATRIX mMatrix, mOldMatrix;
	m_pSprite->GetTransform(&mOldMatrix);
	D3DXVECTOR3 vPos = { vLocation.x, vLocation.y, 0 };

	D3DXVECTOR2 vCenter = { vRect.z * flScale / 2, vRect.w * flScale / 2 };
	D3DXVECTOR2 vScale = { flScale, flScale };

	D3DXMatrixTransformation2D(&mMatrix, NULL, 0.0, &vScale, &vCenter, flRotation, &vLocation);
	m_pSprite->SetTransform(&mMatrix);

	HRESULT hResult = m_pSprite->Draw(m_pTextureAtlas, &rRect, NULL, NULL, cColor);
	m_pSprite->SetTransform(&mOldMatrix);
	return hResult;


}

HRESULT Render::DrawLine(D3DXVECTOR2 vLocationOne, D3DXVECTOR2 vLocationTwo, D3DCOLOR cColor, D3DCOLOR cColor2) {
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
HRESULT Render::DrawOutlinedRect(D3DXVECTOR4 vBounds, int iThickness, D3DCOLOR cOutline, D3DCOLOR cColor, D3DCOLOR cColor2, bool bVertical) {
	if (IsInitialized()) {
		DrawRectangle(vBounds, cOutline);
		vBounds.x += iThickness; vBounds.y += iThickness; vBounds.z -= iThickness * 2, vBounds.w -= iThickness * 2;
		DrawRectangle(vBounds, cColor, cColor2, bVertical);
		return S_OK;

	}
	return OLE_E_BLANK;
}
HRESULT Render::DrawRoundedRectangle(D3DXVECTOR4 vBounds, int iCornerSize, D3DCOLOR cColor, D3DCOLOR cColor2, bool bVertical) {

	if (IsInitialized()) {

		if (bVertical) {
			D3DXCOLOR cLerp;
			if (cColor2) {
				DrawFadingCircle({ vBounds.x + iCornerSize, vBounds.y + iCornerSize }, iCornerSize, 3, cColor, *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor, &(D3DXCOLOR)cColor2, iCornerSize * 2 / vBounds.w), bVertical, .25, 180);
				DrawFadingCircle({ vBounds.x + vBounds.z - iCornerSize, vBounds.y + iCornerSize }, iCornerSize, 3, cColor, *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor, &(D3DXCOLOR)cColor2, iCornerSize * 2 / vBounds.w), bVertical, .25, 270);
				DrawFadingCircle({ vBounds.x + iCornerSize, vBounds.y + vBounds.w - iCornerSize }, iCornerSize, 3, *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor2, &(D3DXCOLOR)cColor, iCornerSize * 2 / vBounds.w), cColor2, bVertical, .25, 90);
				DrawFadingCircle({ vBounds.x + vBounds.z - iCornerSize, vBounds.y + vBounds.w - iCornerSize }, iCornerSize, 3, *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor2, &(D3DXCOLOR)cColor, iCornerSize * 2 / vBounds.w), cColor2, bVertical, .25, 0);
				DrawRectangle({ vBounds.x, vBounds.y + iCornerSize, vBounds.z, vBounds.w - iCornerSize * 2 }, *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor, &(D3DXCOLOR)cColor2, iCornerSize / vBounds.w), *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor2, &(D3DXCOLOR)cColor, iCornerSize / vBounds.w), bVertical);

			}
			else {
				DrawCircle({ vBounds.x + iCornerSize, vBounds.y + iCornerSize }, iCornerSize, 3, cColor, .25, 180);
				DrawCircle({ vBounds.x + vBounds.z - iCornerSize, vBounds.y + iCornerSize }, iCornerSize, 3, cColor, .25, 270);
				DrawCircle({ vBounds.x + iCornerSize, vBounds.y + vBounds.w - iCornerSize }, iCornerSize, 3, cColor, .25, 90);
				DrawCircle({ vBounds.x + vBounds.z - iCornerSize, vBounds.y + vBounds.w - iCornerSize }, iCornerSize, 3, cColor, .25, 0);
				DrawRectangle({ vBounds.x, vBounds.y + iCornerSize, vBounds.z, vBounds.w - iCornerSize * 2 }, cColor, cColor2, bVertical);

			}
			DrawRectangle({ vBounds.x + iCornerSize, vBounds.y, vBounds.z - iCornerSize * 2, vBounds.w }, cColor, cColor2, bVertical);
		}
		else {
			D3DXCOLOR cLerp;
			DrawFadingCircle({ vBounds.x + iCornerSize, vBounds.y + iCornerSize }, iCornerSize, 3, cColor, *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor, &(D3DXCOLOR)cColor2, iCornerSize * 2 / vBounds.z), bVertical, .25, 180);
			DrawFadingCircle({ vBounds.x + vBounds.z - iCornerSize, vBounds.y + iCornerSize }, iCornerSize, 3, *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor2, &(D3DXCOLOR)cColor, iCornerSize * 2 / vBounds.z), cColor2, bVertical, .25, 270);
			DrawFadingCircle({ vBounds.x + iCornerSize, vBounds.y + vBounds.w - iCornerSize }, iCornerSize, 3, cColor, *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor, &(D3DXCOLOR)cColor2, iCornerSize * 2 / vBounds.z), bVertical, .25, 90);
			DrawFadingCircle({ vBounds.x + vBounds.z - iCornerSize, vBounds.y + vBounds.w - iCornerSize }, iCornerSize, 3, *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor2, &(D3DXCOLOR)cColor, iCornerSize * 2 / vBounds.z), cColor2, bVertical, .25, 0);

			DrawRectangle({ vBounds.x + iCornerSize, vBounds.y, vBounds.z - iCornerSize * 2, vBounds.w }, *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor, &(D3DXCOLOR)cColor2, iCornerSize / vBounds.z), *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor2, &(D3DXCOLOR)cColor, iCornerSize / vBounds.z), bVertical);
			DrawRectangle({ vBounds.x, vBounds.y + iCornerSize, vBounds.z, vBounds.w - iCornerSize * 2 }, cColor, cColor2, bVertical);
		}

		return S_OK;
	}
	return OLE_E_BLANK;



}
HRESULT Render::DrawFadingCircle(D3DXVECTOR2 vLocation, float flRadius, int iSides, D3DCOLOR cColor, D3DCOLOR cColor2, bool bVertical, float flFraction, float flRotation) {
	if (IsInitialized()) {
		ManageBatch(D3DPT_TRIANGLELIST, iSides + 2, iSides * 3);
		float flCos = cos(2 * PI / iSides * flFraction);
		float flSin = sin(2 * PI / iSides * flFraction);
		float flFracCos = cos(flRotation / 180 * PI);
		float flFracSin = sin(flRotation / 180 * PI);
		float flR = flRadius * flFracCos;
		float flD = flRadius * flFracSin;
		float flTemp;
		D3DXCOLOR cLerp;
		unsigned int iCenter = AddVertex(D3DPT_TRIANGLELIST, { vLocation.x, vLocation.y }, *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor, &(D3DXCOLOR)cColor2, .5));
		unsigned int iPrev = AddVertex(D3DPT_TRIANGLELIST, { vLocation.x + flR, vLocation.y + flD }, *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor, &(D3DXCOLOR)cColor2, (bVertical ? flD : flR) / flRadius + 1 / 2));


		for (int i = 0; i < iSides; i++) {
			flTemp = flR;
			flR = flCos * flR - flSin * flD;
			flD = flSin * flTemp + flCos * flD;

			unsigned int iTemp = AddVertex(D3DPT_TRIANGLELIST, { vLocation.x + flR, vLocation.y + flD }, *D3DXColorLerp(&cLerp, &(D3DXCOLOR)cColor, &(D3DXCOLOR)cColor2, (bVertical ? flD : flR) / flRadius + 1 / 2));
			AddTriangle(iTemp, iCenter, iPrev);

			iPrev = iTemp;

		}

		return S_OK;

	}
	return OLE_E_BLANK;
}

HRESULT Render::DrawCircleOutline(D3DXVECTOR2 vLocation, float flRadius, int iSides, D3DCOLOR cColor, float flFraction, float flRotation) {
	if (IsInitialized()) {
		ManageBatch(D3DPT_LINELIST, iSides + 1, iSides * 2);
		float flCos = cos(2 * PI / iSides * flFraction);
		float flSin = sin(2 * PI / iSides * flFraction);
		float flFracCos = cos(flRotation / 180 * PI);
		float flFracSin = sin(flRotation / 180 * PI);
		float flR = flRadius * flFracCos;
		float flD = flRadius * flFracSin;
		float flTemp;


		unsigned int iPrev = AddVertex(D3DPT_LINELIST, { vLocation.x + flR, vLocation.y + flD }, cColor);


		for (int i = 0; i < iSides; i++) {
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

HRESULT Render::DrawCircle(D3DXVECTOR2 vLocation, float flRadius, int iSides, D3DCOLOR cColor, float flFraction, float flRotation) {
	if (IsInitialized()) {
		ManageBatch(D3DPT_TRIANGLELIST, iSides + 2, iSides * 3);
		float flCos = cos(2 * PI / iSides * flFraction);
		float flSin = sin(2 * PI / iSides * flFraction);
		float flFracCos = cos(flRotation / 180 * PI);
		float flFracSin = sin(flRotation / 180 * PI);
		float flR = flRadius * flFracCos;
		float flD = flRadius * flFracSin;
		float flTemp;

		unsigned int iCenter = AddVertex(D3DPT_TRIANGLELIST, { vLocation.x, vLocation.y }, cColor);
		unsigned int iPrev = AddVertex(D3DPT_TRIANGLELIST, { vLocation.x + flR, vLocation.y + flD }, cColor);


		for (int i = 0; i < iSides; i++) {
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

HRESULT Render::DrawString(D3DXVECTOR2 vLocation, D3DCOLOR cColor, ID3DXFont* pFont, const char* szString, ...) {

	RECT pRect;

	pRect.left = (long)vLocation.x;
	pRect.top = (long)vLocation.y;
	return pFont->DrawTextA(m_pSprite, szString, -1, &pRect, DT_NOCLIP, cColor);
}
HRESULT Render::DrawString(D3DXVECTOR2 vLocation, D3DCOLOR cColor, ID3DXFont* pFont, const WCHAR* szString, ...) {

	RECT pRect;

	pRect.left = (long)vLocation.x;
	pRect.top = (long)vLocation.y;
	return pFont->DrawTextW(m_pSprite, szString, -1, &pRect, DT_NOCLIP, cColor);
}

D3DXVECTOR2 Render::GetStringSize(ID3DXFont* pFont, const WCHAR* szString, ...) {
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.bottom = 0;
	rect.right = 0;
	pFont->DrawTextW(NULL, szString, -1, &rect, DT_CALCRECT, 0xffffffff);
	return { (float)rect.right, (float)rect.bottom };
}
D3DXVECTOR2 Render::GetStringSize(ID3DXFont* pFont, const char* szString, ...) {

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.bottom = 0;
	rect.right = 0;
	pFont->DrawTextA(NULL, szString, -1, &rect, DT_CALCRECT, 0xffffffff);
	return { (float)rect.right, (float)rect.bottom };
}
