#include "IValueElement.h"

IValueElement::IValueElement(D3DXVECTOR4 vBounds, IGUIElement* pParent) : IGUIElement(vBounds, pParent) {
    return;
}

IValueElement::~IValueElement(void) {
    return;
}

void IValueElement::SetValuePointer(void* pValue){

    return;
}

void* IValueElement::GetValuePointer(void){
    return nullptr;
}
