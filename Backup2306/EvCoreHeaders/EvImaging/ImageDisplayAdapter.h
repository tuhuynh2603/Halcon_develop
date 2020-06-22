#pragma once

#include "DIBSectionLite.h"

class CDisplayAdapter : public CDIBSectionLite  
{
public:
	BYTE m_byteStrict,m_byteLoose,m_byteNormal;
	void ChangeToGrayPalette();
	void ChangeToBinaryPalette(int nThreshold);
	void ChangeToColorPalette();
	BOOL Initialize(int nImgWidth, int nImgHeight, BOOL bColor);

	CDC* GetDrawContext();
	void ReleaseDrawContext();
	
	void InitializeColorTable(RGBQUAD *RGB);
	CDisplayAdapter();
	virtual ~CDisplayAdapter();

protected:
	bool m_bBinaryMode;
	int m_nBinaryThreshold;
	RGBQUAD* m_pRGB;
};
