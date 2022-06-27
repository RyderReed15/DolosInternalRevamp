#pragma once

#ifndef VALUE_ELEMENT_H
#define VALUE_ELEMENT_H

#include "IGUIElement.h"

class IValueElement : public IGUIElement {
public:

					IValueElement(D3DXVECTOR4 vBounds, IGUIElement* pParent = nullptr);
	virtual			~IValueElement(void);

	virtual void	SetValuePointer(void* pValue);
	virtual void *	GetValuePointer(void);

};



#endif // !VALUE_ELEMENT_H
