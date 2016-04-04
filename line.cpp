class cLine
{
public:
	HDC mdc;
	int mx1;
	int my1;
	int mx2;
	int my2;
	COLORREF mcolor_linea;
	RECT mrct;
	int mz;

	int Crear(HDC dc, int x1, int y1, int x2, int y2, RECT *rct, COLORREF line_color, int z=0);
	int Dibujar(HDC dc);
	int Destruir();
	void Line_Nor(HDC dc,int x1, int y1, int x2, int y2, COLORREF line_color);
	void Line_Rec(HDC dc, int x1, int y1, int x2, int y2, RECT rct, COLORREF line_color);
private:
	int creado;

};


int cLine::Destruir()
{
	mdc=NULL;
	mx1=0;
	my1=0;
	mx2=0;
	my2=0;
	mcolor_linea=0;
	mrct.left=0;
	mrct.right=0;
	mrct.bottom=0;
	mrct.top=0;
	mz=0;
	creado=0;
	return 0;
}

int cLine::Crear(HDC dc, int x1, int y1, int x2, int y2, RECT *rct, COLORREF line_color, int z)
{
	mdc=dc;
	mx1=x1;
	my1=y1;
	mx2=x2;
	my2=y2;
	mz=z;
	if(rct!=NULL)
	{
		mrct.left=rct->left;
		mrct.right=rct->right;
		mrct.top=rct->top;
		mrct.bottom=rct->bottom;
	}
	else
	{
		mrct.left=0;
		mrct.right=0;
		mrct.top=0;
		mrct.bottom=0;

	}
	mcolor_linea=line_color;
	creado=1;
	return 0;
}

int cLine::Dibujar(HDC dc)
{
	mdc=dc;
	if(mdc==NULL)return 0;
	if(mrct.left && mrct.right && mrct.top && mrct.bottom)
	{
		Line_Rec(mdc,mx1,my1,mx2,my2,mrct,mcolor_linea);
	}
	else
		Line_Nor(mdc,mx1,my1,mx2,my2,mcolor_linea);
	return 0;
}



void cLine::Line_Nor(HDC dc,int x1, int y1, int x2, int y2, COLORREF line_color)
{
	bool yLonger=false;
	int incrementVal, endVal;

	int shortLen=y2-y1;
	int longLen=x2-x1;
	if (abs(shortLen)>abs(longLen)) {
		int swap=shortLen;
		shortLen=longLen;
		longLen=swap;
		yLonger=true;
	}
	
	endVal=longLen;
	if (longLen<0) {
		incrementVal=-1;
		longLen=-longLen;
	} else incrementVal=1;

	double decInc;
	if (longLen==0) decInc=(double)shortLen;
	else decInc=((double)shortLen/(double)longLen);
	double j=0.0;
	if (yLonger) {
		for (int i=0;i!=endVal;i+=incrementVal) {
			SetPixel(dc,x1+(int)j,y1+i,line_color);
			j+=decInc;
		}
	} else {
		for (int i=0;i!=endVal;i+=incrementVal) {
			SetPixel(dc,x1+i,y1+(int)j,line_color);
			j+=decInc;
		}
	}

}

void cLine::Line_Rec(HDC dc, int x1, int y1, int x2, int y2, RECT rct, COLORREF line_color)
{
	//Recorte
	POINT Ln[4],Pt[4];
	double m=0,dist[2];
	int ps[2]={0,1},bound[2]={0,0};
	int temp=0,i,z=0;
	Pt[0].x=x1;
	Pt[0].y=y1;
	Pt[1].x=x2;
	Pt[1].y=y2;

	m=(double)(Pt[0].y-Pt[1].y)/(double)(Pt[0].x-Pt[1].x);
	Ln[0].x=(double)Pt[1].x-((double)(Pt[1].y-rct.top)/m);
	Ln[0].y=rct.top;
	Ln[1].x=(double)Pt[1].x-((double)(Pt[1].y-rct.bottom)/m);
	Ln[1].y=rct.bottom;
	Ln[2].x=rct.left;
	Ln[2].y=(double)Pt[1].y-(m*(double)(Pt[1].x-rct.left));
	Ln[3].x=rct.right;
	Ln[3].y=(double)Pt[1].y-(m*(double)(Pt[1].x-rct.right));
				
	for(i=0;i<2;i++)
		if(Ln[i].x>=rct.left && Ln[i].x<=rct.right)
			bound[temp++]=i;

	if(temp<2)
		for(i=2;i<4;i++)
			if(Ln[i].y>=rct.top && Ln[i].y<=rct.bottom)
				bound[temp++]=i;
	Pt[2].x=Ln[bound[0]].x;
	Pt[2].y=Ln[bound[0]].y;
	Pt[3].x=Ln[bound[1]].x;
	Pt[3].y=Ln[bound[1]].y;

	for(i=0;i<2;i++)
	{
		if(Pt[i].x<rct.left || Pt[i].x>rct.right || Pt[i].y<rct.top || Pt[i].y>rct.bottom)
		{
				dist[0]=Distancia(Pt[i].x,Pt[i].y,Pt[2].x,Pt[2].y);
				dist[1]=Distancia(Pt[i].x,Pt[i].y,Pt[3].x,Pt[3].y);
				ps[i]=2;
				if(dist[0]>dist[1])
					ps[i]=3;
			}
		}

	Line_Nor(dc,Pt[ps[0]].x,Pt[ps[0]].y,Pt[ps[1]].x,Pt[ps[1]].y,line_color);
}