#include <windows.h>
#include "ddraw.h"
#include <math.h>
#include <stdio.h>

#define ANCHO	640
#define ALTO	480
#define BITS	32
#define PASOS_DONA  70
#define GRADOS_LUZ	10

LPDIRECTDRAW		lpDD;
LPDIRECTDRAWSURFACE	lpDDSPrimary;
LPDIRECTDRAWSURFACE lpDDSBack;
DDSURFACEDESC		ddsd;
DDSCAPS				ddscaps;
HDC	dcsurface;
HWND hWnd;
HBRUSH brsh;

#define asm3dnow 0
#define asmsse	0
#define normal 1

#include "clases3d.cpp"

bool Inicializar();
void Terminar();
bool Principal();
void Dibujar(HDC dc);
void ObtenerTeclas(char *boton);
COLORREF Intensidad(COLORREF color, float factor);

//Variables Globales de la Camara y los Puntos que forman el plano
Punto3D po(-50,-50,-100),pu(50,-50,-100),pv(-50,50,-100);
Punto3D por(-75,100,-100),pe(0,0,0),pl(0,75,-75);
Punto3D pod(0,0,0);
Camara camara(po,pu,pv);
Luz luz(por,pe,pl);
Cubo cubo;
Dona dona(po,25,10,PASOS_DONA,PASOS_DONA);
//Teclas presionadas
char teclas[256];
char texto1[80];
char texto[512];
float tics[3];
float fps;
long cuadros=0;
bool mostrar_ayuda=false;
int tipo_dibujo=1;//0->Cubo 1->Dona
int tipo_dibujo_a=-1;
int tipo_proyec=1;//0->Paralela 1->Perspectiva
long pasos_dona=PASOS_DONA;
long pasos_dona_a=-1;
Triad *trds;
long cant;
int activo=0;
RECT pantalla;
int fsaa=0;

long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SetCursor(NULL);
	switch(message)
	{
		case WM_SETCURSOR:
			SetCursor(NULL);
			break;
		case WM_CREATE:

			break;
		case WM_KEYDOWN:
			ObtenerTeclas(teclas);
			//Movimiento
			if(teclas[VK_UP])
				camara.Mover(camara.v.x/20,camara.v.y/20,camara.v.z/20);
			if(teclas[VK_DOWN])
				camara.Mover(-camara.v.x/20,-camara.v.y/20,-camara.v.z/20);
			if(teclas[VK_LEFT])
				camara.Mover(-camara.u.x/20,-camara.u.y/20,-camara.u.z/20);
			if(teclas[VK_RIGHT])
				camara.Mover(camara.u.x/20,camara.u.y/20,camara.u.z/20);
			//if(teclas[0x41])
			//	camara.Mover(camara.wn.x,camara.wn.y,camara.wn.z);
			//if(teclas[0x5A])
			//	camara.Mover(-camara.wn.x,-camara.wn.y,-camara.wn.z);
			//Rotacion
			if(teclas['W'])
				camara.Rotar(R_EJEU,1);
			if(teclas['S'])
				camara.Rotar(R_EJEU,-1);
			if(teclas['A'])
				camara.Rotar(R_EJEV,1);
			if(teclas['D'])
				camara.Rotar(R_EJEV,-1);
			if(teclas['Q'])
				camara.Rotar(R_EJEW,1);
			if(teclas['E'])
				camara.Rotar(R_EJEW,-1);
			if(teclas[VK_F1])
				mostrar_ayuda=(mostrar_ayuda)?false:true;
			if(teclas[VK_F2])
				tipo_dibujo=(tipo_dibujo)?0:1;
			if(teclas[VK_F3])
				tipo_proyec=(tipo_proyec)?0:1;
			if(teclas[VK_F4])
				if(pasos_dona>=15)
				{
					pasos_dona-=10;
					dona.Crear(pod,25,10,pasos_dona,pasos_dona);
				}
			if(teclas[VK_F5])
			{
				pasos_dona+=10;
				dona.Crear(pod,25,10,pasos_dona,pasos_dona);
			}
			if(teclas[VK_F6])
			{
				fsaa=(fsaa)?0:1;
			}
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_PAINT:
			break;
		case WM_ACTIVATE:
			if(LOWORD(wParam)==WA_INACTIVE)
				activo=2;
			else
			{
				if(activo==2)
				{
					lpDDSPrimary->Restore();
					lpDDSBack->Restore();
				}
				activo=1;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			break;
	}
	//camara.Rotar(R_EJEV,-1); //Rotacion automática
	return DefWindowProc(hWnd,message,wParam,lParam);
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)

{
	MSG msg;
	Distancia3D(1,2,3);
	if(!Inicializar())
		return 1;
	
	while(1)
	{
		if(GetAsyncKeyState(VK_ESCAPE))
			break;
		PeekMessage(&msg,hWnd,0,0,PM_REMOVE);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(activo==1)
		if(!Principal())
		{
			return 0;
		}

	}
	delete [] trds;
	Terminar();
	return 0;
}

void ObtenerTeclas(char *boton)
{
	int a;
	GetKeyboardState((unsigned char*)boton);
	for(a=0;a<256;a++)
	{
		boton[a]&=0x80;
	}
}

bool Inicializar()
{
	int res;
	
	LOGBRUSH lgb;
	lgb.lbColor=RGB(0,0,0);
	lgb.lbHatch=NULL;
	lgb.lbStyle=BS_SOLID;
	brsh=CreateBrushIndirect(&lgb);
	

	WNDCLASSEX wc;
	wc.cbSize=sizeof(wc);
	wc.style=0;
	wc.lpfnWndProc=&WindowProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=0;
	wc.hbrBackground=brsh;
	wc.hIcon=NULL;
	wc.hCursor=NULL;
	wc.lpszMenuName=NULL;
	wc.lpszClassName="RV";
	wc.hIconSm=NULL;

	RegisterClassEx(&wc);

	hWnd=CreateWindowEx(WS_EX_APPWINDOW | WS_EX_TOPMOST,"RV",NULL,WS_POPUP,0,0,ANCHO,ALTO,NULL,NULL,NULL,NULL);
	if(!hWnd) 
	{
		MessageBox(NULL,"Error al crear la ventana","Error",0);
		return false;
	}

	res=DirectDrawCreate(NULL,&lpDD,NULL);
	if(res!=DD_OK)
	{
		MessageBox(NULL,"Error al iniciar DirectDraw","Error",0);
		return false;	
	}

	res=lpDD->SetCooperativeLevel(hWnd,DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
	if(res!=DD_OK)
	{
		MessageBox(NULL,"Error al resevar la pantalla completa","Error",0);
		return false;
	}

	res=lpDD->SetDisplayMode(ANCHO,ALTO,BITS);
	if(res!=DD_OK)
	{	
		MessageBox(NULL,"Error al establecer el tamaño de la pantalla","Error",0);
		return false;
	}

	ddsd.dwSize=sizeof(ddsd);
	ddsd.dwFlags=DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE | DDSCAPS_VIDEOMEMORY | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount=1;
	res=lpDD->CreateSurface(&ddsd,&lpDDSPrimary,NULL);
	if(res!=DD_OK)
	{
		MessageBox(NULL,"Error al crear la superficie primaria","Error",0);
		return false;
	}

	ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
	res=lpDDSPrimary->GetAttachedSurface(&ddscaps,&lpDDSBack);
	if(res!=DD_OK)
	{
		MessageBox(NULL,"Error al crear la superficie secundaria","Error",0);
		return false;
	}

	cubo.color[0]=RGB(255,0,0);
	cubo.color[1]=RGB(0,255,0);
	cubo.color[2]=RGB(0,0,255);
	cubo.color[3]=RGB(255,255,0);
	cubo.color[4]=RGB(113,132,142);
	cubo.color[5]=RGB(0,255,255);
	cubo.escala=20;
	cubo.Calcular();
	dona.color=RGB(0,255,0);
	dona.Calcular();

	pantalla.left=0;
	pantalla.top=0;
	pantalla.bottom=ALTO;
	pantalla.right=ANCHO;
	return true;
}

void Terminar()
{
	if (lpDD!=NULL)
	{
		if(lpDDSPrimary!=NULL)
		{
			lpDDSPrimary->Release();
			lpDDSPrimary=NULL;
		}
		lpDD->Release();
		lpDD=NULL;
	}
	
}

bool Principal()
{
	HBRUSH	hbrs;

	luz.Rotar(GRADOS_LUZ);

	lpDDSBack->GetDC(&dcsurface);

	SetTextColor(dcsurface,RGB(255,255,0));
	SetBkMode(dcsurface, TRANSPARENT);

	if(cuadros==0)tics[0]=(float)GetTickCount();

	hbrs=CreateSolidBrush(0);
	FillRect(dcsurface,&pantalla,hbrs);
	DeleteObject(hbrs);

	Dibujar(dcsurface);
	cuadros++;

	tics[3]=(float)GetTickCount()-(float)tics[0];
	if(tics[3]>1000)
	{
		fps=(float)cuadros/(float)tics[3];
		fps*=1000;
		sprintf(texto1,"%f FPS   Dona: %i pasos   <F1> Para ayuda",fps,pasos_dona);
		cuadros=0;
	}
	TextOut(dcsurface,0,0,texto1,strlen(texto1));
	if(mostrar_ayuda)
	{
		sprintf(texto,"<F2> Cambiar entre Cubo y Dona");
		TextOut(dcsurface,0,16,texto,strlen(texto));
		sprintf(texto,"<F3> Cambiar Entre Proyeccion Perspectiva y Paralela");
		TextOut(dcsurface,0,32,texto,strlen(texto));
		sprintf(texto,"<F4> Disminuye Resolucion de la Dona");
		TextOut(dcsurface,0,48,texto,strlen(texto));
		sprintf(texto,"<F5> Aumenta Resolucion de la Dona");
		TextOut(dcsurface,0,64,texto,strlen(texto));
		sprintf(texto,"<F6> Activar/Desactivar FSAA");
		TextOut(dcsurface,0,80,texto,strlen(texto));
		sprintf(texto,"<Teclado Numérico> Rotar Camara");
		TextOut(dcsurface,0,96,texto,strlen(texto));
		sprintf(texto,"<Teclas de Direccion,Teclas AyZ> Mover Foco de la Camara");
		TextOut(dcsurface,0,112,texto,strlen(texto));
		sprintf(texto,"<Esc> Salir");
		TextOut(dcsurface,0,128,texto,strlen(texto));
	}
	//AntialiasA(dcsurface,260,180,380,300);
	if(fsaa)
		Antialias(dcsurface,0,0,ANCHO,ALTO);
	lpDDSBack->ReleaseDC(dcsurface);

	lpDDSPrimary->Flip(NULL,DDFLIP_NOVSYNC | DDFLIP_WAIT);

	return true;
}

void Dibujar(HDC dc)
{
	int a,b=0;
	Punto2D proy;
	POINT puntos[3];
	HRGN region;
	HBRUSH brush;
	float prod;

	if(tipo_dibujo!=tipo_dibujo_a || pasos_dona!=pasos_dona_a)
	{
		if(tipo_dibujo_a!=-1)
			delete [] trds;
		if(tipo_dibujo==1)
		{
		cant=dona.numTrds;
		trds=new Triad[cant];
		dona.ObtenerTriads(trds);
		}
		if(tipo_dibujo==0)
		{
			cant=cubo.numTrds;
			trds=new Triad[cant];
			cubo.ObtenerTriads(trds);
		}
		tipo_dibujo_a=tipo_dibujo;
		pasos_dona_a=pasos_dona;
	}
//	if(tipo_dibujo==1)
//		Ordenar_Triads(trds,cant,camara.pc);

	long cont=0;
	if(tipo_dibujo==1)
	{
		cont=Buscar(trds,cant,camara.pc);
	}
	for(a=0;a<cant;a++)
	{
		for(b=0;b<3;b++)
		{
			if(tipo_proyec)
			{
				if(!camara.PPerspectiva(trds[cont].vertices[b],proy)) 
					goto continuar;
			}
			else
			{
				if(!camara.PParalela(trds[cont].vertices[b],proy)) 
					goto continuar;
			}
			#if normal
			puntos[b].x=proy.x*ANCHO;
			puntos[b].y=(1-proy.y)*ALTO;
			#endif
			#if asm3dnow
				float tx=1,ty;//,ta=ANCHO,tal=ALTO;
				_asm
				{
					femms
					movd mm0,proy.x
					
					//movd mm1,tx
					mov eax,1
					movd mm1,eax
					pi2fd mm1,mm1

					movd mm2,proy.y
					//movd mm3,ta
					mov eax,ANCHO
					movd mm3,eax
					pi2fd mm3,mm3

					//movd mm4,tal
					mov eax, ALTO
					movd mm4,eax
					pi2fd mm4,mm4

					pfsub mm1,mm2
					pfmul mm0,mm3
					pfmul mm1,mm4
					movd tx,mm0
					movd ty,mm1
					femms
				}
				puntos[b].x=tx;
				puntos[b].y=ty;
			#endif
			#if asmsse
				float tx=1,ty,ta=ANCHO,tal=ALTO;
				_asm
				{
					movss xmm0,proy.x
					movss xmm1,tx
					movss xmm2,proy.y
					movss xmm3,ta
					movss xmm4,tal
					subss xmm1,xmm2
					mulss xmm0,xmm3
					mulss xmm1,xmm4
					movss tx,xmm0
					movss ty,xmm1
				}
				puntos[b].x=tx;
				puntos[b].y=ty;
			#endif
		}
		prod=trds[cont].wn*camara.ref;

		if(prod>=-1 && prod<=0)
		{
			prod=-(luz.luz*trds[cont].wn);
			if(prod<0)prod=0;
			
			brush=CreateSolidBrush(Intensidad(trds[cont].color,prod));
			region=CreatePolygonRgn(puntos,b,WINDING);
			FillRgn(dc,region,brush);
			DeleteObject(brush);
			DeleteObject(region);
		}
continuar:
		b=0;
		cont++;
		if(cont==cant)cont=0;
	}

}


COLORREF Intensidad(COLORREF color, float factor)
{
/*	int ib=(color&0xFF0000)>>16;
	int ig=(color&0x00FF00)>>8;
	int ir=color&0x0000FF;
	float dr=(float)ir;
	float dg=(float)ig;
	float db=(float)ib;
	dr*=factor;
	dg*=factor;
	db*=factor;
	ir=(int)dr;
	ig=(int)dg;
	ib=(int)db;
	return RGB(ir,ig,ib);*/
	return RGB((int)((float)factor*(float)(color&0x0000FF)),(int)((float)factor*(float)((color&0x00FF00)>>8)),(int)((float)factor*(float)((color&0xFF0000)>>16)));
	//return RGB((int)((float)factor*(float)(rand()%256)),(int)((float)factor*(float)(rand()%256)),(int)((float)factor*(float)(rand()%256)));
	//return RGB((int)((float)factor*(float)(0&0x0000FF)),(int)((float)factor*(float)((0&0x00FF00)>>8)),(int)((float)factor*(float)(rand()%256)));
}

