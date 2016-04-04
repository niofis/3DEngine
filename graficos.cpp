#define TRANSPARENT_FILL	0
#define OPAQUE_FILL			1


void Borrar(HDC dc, RECT area, COLORREF color)
{
//	int x,y;
//	for(y=area.top;y<=area.bottom;y++)
//		for(x=area.left;x<=area.right;x++)
//			SetPixel(dc,x,y,color);
	area.bottom++;
	area.right++;
	HBRUSH brush,abrush;
	brush=CreateSolidBrush(0);
	abrush=(HBRUSH)SelectObject(dc,brush);
	FillRect(dc,&area,brush);
	SelectObject(dc,abrush);
	DeleteObject(brush);
	DeleteObject(abrush);
}

double Distancia(int x1, int y1, int x2, int y2)
{
	return (pow(pow(x1-x2,2)+pow(y1-y2,2),0.5));
}


BOOL Dentro(RECT rct, POINT pt)
{
	if(pt.x>=rct.left && pt.x<=rct.right && pt.y>=rct.top && pt.y<=rct.bottom)
		return true;
	else
		return false;
}

BOOL Colision(RECT rc1, RECT rc2)
{
	POINT pt1,pt2,pt3,pt4;
	
	pt1.x=rc1.left;
	pt1.y=rc1.top;
	pt2.x=rc1.left;
	pt2.y=rc1.bottom;
	pt3.x=rc1.right;
	pt3.y=rc1.top;
	pt4.x=rc1.right;
	pt4.y=rc1.bottom;

	if(Dentro(rc2,pt1))
		return true;
	if(Dentro(rc2,pt2))
		return true;
	if(Dentro(rc2,pt3))
		return true;
	if(Dentro(rc2,pt4))
		return true;

	pt1.x=rc2.left;
	pt1.y=rc2.top;
	pt2.x=rc2.left;
	pt2.y=rc2.bottom;
	pt3.x=rc2.right;
	pt3.y=rc2.top;
	pt4.x=rc2.right;
	pt4.y=rc2.bottom;

	if(Dentro(rc1,pt1))
		return true;
	if(Dentro(rc1,pt2))
		return true;
	if(Dentro(rc1,pt3))
		return true;
	if(Dentro(rc1,pt4))
		return true;

	return false;
}


