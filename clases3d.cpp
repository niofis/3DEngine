#define FLT_EPSILON 1.192092896e-07F 

#define R_CENTRO	1
#define	R_EJEU		1<<1
#define R_EJEV		1<<2
#define	R_EJEW		1<<3
const float GRADOS=3.1415926535897932384626433832795/180;


class Punto2D
{
public:
	float x,y;

	Punto2D(float x, float y){x=x;y=y;};
	Punto2D(){x=0;y=0;};
	Punto2D operator = (const Punto2D& pt);
};



class Punto3D
{
public:
	float x,y,z;

	Punto3D(float xe, float ye, float ze){x=xe;y=ye;z=ze;};
	Punto3D(){x=0;y=0;z=0;};
	Punto3D operator = (const Punto3D& pt);
};


class Vector
{
public:
	float x,y,z;
	float norma;

	Vector(float x, float y, float z);
	Vector(Punto3D pt1, Punto3D pt2);
	Vector();
	Vector(Punto3D pt);
	void Crear(Punto3D pt1, Punto3D pt2);
	float Norma();
	void Normalizar();
	Vector operator = (const Vector& Vec);
	Vector operator += (const Vector& Vec);
	float operator * (const Vector& Vec);
	Vector operator ^ (const Vector& Vec);
};

void Escala(Vector &mov, Punto3D &punto)
{
	punto.x*=mov.x;
	punto.y*=mov.y;
	punto.z*=mov.z;
}

void Traslacion(Vector &mov, Punto3D &punto)
{
	punto.x+=mov.x;
	punto.y+=mov.y;
	punto.z+=mov.z;
}

void Rotacion(Vector &vpt, Vector &eje, float grados)
{
	#if normal
		Vector t1,t2,t3;
		float dv;

		t1=vpt;
		dv=cos(grados);
		t1.x*=dv;
		t1.y*=dv;
		t1.z*=dv;

		t2=eje;
		dv=eje*vpt;
		dv*=1-cos(grados);
		t2.x*=dv;
		t2.y*=dv;
		t2.z*=dv;

		t3=vpt^eje;
		dv=sin(grados);
		t3.x*=dv;
		t3.y*=dv;
		t3.z*=dv;

		t1+=t2;
		t1+=t3;

		vpt=t1;
	#endif
	#if asm3dnow 

		Vector t1=vpt,t2=eje,t3=t1^t2;
		float dv=eje*vpt;

		_asm
		{
			movd mm0,dv
			femms
			fld grados
			fcos
			fstp dv
			movd mm1,dv

			//mov eax,-1
			//movd mm2,eax
			//pi2fd mm2,mm2
			mov eax,1
			movd mm3,eax
			pi2fd mm3,mm3
	

			//pfmul mm1,mm2
			pfsub mm3,mm1
			pfmul mm0,mm3

			movd mm1,t2.x
			movd mm2,t2.y
			movd mm3,t2.z
			pfmul mm1,mm0
			pfmul mm2,mm0
			pfmul mm3,mm0
			movd t2.x,mm1
			movd t2.y,mm2
			movd t2.z,mm3

			femms
			fld grados
			fcos
			fstp dv

			movd mm0,t1.x
			movd mm1,t1.y
			movd mm2,t1.z
			movd mm3,dv
			pfmul mm0,mm3
			pfmul mm1,mm3
			pfmul mm2,mm3
			movd t1.x,mm0
			movd t1.y,mm1
			movd t1.z,mm2

			femms
			fld grados
			fsin
			fstp dv
			movd mm0,dv
			movd mm1,t3.x
			movd mm2,t3.y
			movd mm3,t3.z
			pfmul mm1,mm0
			pfmul mm2,mm0
			pfmul mm3,mm0
			movd t3.x,mm1
			movd t3.y,mm2
			movd t3.z,mm3	

			femms
		}
		t1+=t2;
		t1+=t3;

		vpt=t1;
	#endif
	#if asmsse

		Vector t1=vpt,t2=eje,t3=t1^t2;
		float dv=eje*vpt;

		_asm
		{
			movss xmm0,dv
			femms
			fld grados
			fcos
			fstp dv
			movss xmm1,dv
			
			movss xmm3,xmm1
			divss xmm3,xmm3
			
			subss xmm3,xmm1
			mulss xmm0,xmm3

			movss xmm1,t2.x
			movss xmm2,t2.y
			movss xmm3,t2.z
			mulss xmm1,xmm0
			mulss xmm2,xmm0
			mulss xmm3,xmm0
			movss t2.x,xmm1
			movss t2.y,xmm2
			movss t2.z,xmm3


			femms
			fld grados
			fcos
			fstp dv

			movss xmm0,t1.x
			movss xmm1,t1.y
			movss xmm2,t1.z
			movss xmm3,dv
			mulss xmm0,xmm3
			mulss xmm1,xmm3
			mulss xmm2,xmm3
			movss t1.x,xmm0
			movss t1.y,xmm1
			movss t1.z,xmm2




			femms
			fld grados
			fsin
			fstp dv
			movss xmm0,dv
			movss xmm1,t3.x
			movss xmm2,t3.y
			movss xmm3,t3.z
			mulss xmm1,xmm0
			mulss xmm2,xmm0
			mulss xmm3,xmm0
			movss t3.x,xmm1
			movss t3.y,xmm2
			movss t3.z,xmm3	

			femms
		}
		t1+=t2;
		t1+=t3;

		vpt=t1;
	#endif

}

class Triad
{
public:
	Punto3D vertices[3];
	COLORREF color;
	Vector u,v,w;
	Vector un,vn,wn;
	Triad(Punto3D pt1,Punto3D pt2,Punto3D pt3);
	Triad();
	void Vertices(Punto3D pt1,Punto3D pt2,Punto3D pt3);
	Triad operator=(const Triad& trd);
	void Calcular();
};

class Luz
{
public:
	Punto3D po,pe,pl;
	Vector eje,luz;
	Luz(Punto3D origen,Punto3D peje,Punto3D pluz);
	void Crear(Punto3D origen,Punto3D peje,Punto3D pluz);
	void Calcular();
	void Rotar(float grados);
};

void Luz::Rotar(float grados)
{
	
	Vector tl(po,pl);
	grados*=GRADOS;

	Rotacion(tl,eje,grados);

	pl.x=po.x+tl.x;
	pl.y=po.y+tl.y;
	pl.z=po.z+tl.z;
	
	Calcular();	
}

void Luz::Calcular()
{
	Vector t1(po,pe),t2(po,pl);
	eje=t1;
	luz=t2;
	eje.Normalizar();
	luz.Normalizar();
}

void Luz::Crear(Punto3D origen,Punto3D peje,Punto3D pluz)
{
	po=origen;
	pe=peje;
	pl=pluz;
	Calcular();
}

Luz::Luz(Punto3D origen,Punto3D peje,Punto3D pluz)
{
	po=origen;
	pe=peje;
	pl=pluz;
	Calcular();
}

void Triad::Calcular()
{
	u.Crear(vertices[0],vertices[1]);
	v.Crear(vertices[0],vertices[2]);
	w=u^v;
	u.Norma();
	v.Norma();
	w.Norma();

	un=u;
	vn=v;
	wn=w;

	un.Normalizar();
	vn.Normalizar();
	wn.Normalizar();
}

Triad Triad::operator =(const Triad& trd)
{
	vertices[0]=trd.vertices[0];
	vertices[1]=trd.vertices[1];
	vertices[2]=trd.vertices[2];
	color=trd.color;
	Calcular();
	return (*this);
}

Triad::Triad()
{
	color=0;
	vertices[0].x=0;
	vertices[0].y=0;
	vertices[0].z=0;
	vertices[1].x=0;
	vertices[1].y=0;
	vertices[1].z=0;
	vertices[2].x=0;
	vertices[2].y=0;
	vertices[2].z=0;
}

Triad::Triad(Punto3D pt1,Punto3D pt2,Punto3D pt3)
{
	vertices[0]=pt1;
	vertices[1]=pt2;
	vertices[2]=pt3;
	Calcular();
}

void Triad::Vertices(Punto3D pt1,Punto3D pt2,Punto3D pt3)
{
	vertices[0]=pt1;
	vertices[1]=pt2;
	vertices[2]=pt3;
	Calcular();
}

class Quad
{
public:
	Punto3D vertices[3];
	Triad triads[2];
	COLORREF color;
	Quad(Punto3D pt1,Punto3D pt2,Punto3D pt3,Punto3D pt4);
	Quad();
	void Vertices(Punto3D pt1,Punto3D pt2,Punto3D pt3,Punto3D pt4);
};

Quad::Quad()
{
	color=0;
	vertices[0].x=0;
	vertices[0].y=0;
	vertices[0].z=0;
	vertices[1].x=0;
	vertices[1].y=0;
	vertices[1].z=0;
	vertices[2].x=0;
	vertices[2].y=0;
	vertices[2].z=0;
}

Quad::Quad(Punto3D pt1,Punto3D pt2,Punto3D pt3,Punto3D pt4)
{
	vertices[0]=pt1;
	vertices[1]=pt2;
	vertices[2]=pt3;
	vertices[3]=pt4;
	triads[0].Vertices(pt1,pt2,pt3);
	triads[1].Vertices(pt2,pt3,pt4);
	triads[0].color=color;
	triads[1].color=color;
}

void Quad::Vertices(Punto3D pt1,Punto3D pt2,Punto3D pt3,Punto3D pt4)
{
	vertices[0]=pt1;
	vertices[1]=pt2;
	vertices[2]=pt3;
	vertices[3]=pt4;
	triads[0].color=color;
	triads[1].color=color;
	triads[0].Vertices(pt1,pt2,pt3);
	triads[1].Vertices(pt3,pt4,pt1);
}

class Cubo
{
public:
	Quad lados[6];
	Cubo();
	float escala;
	COLORREF color[6];
	Punto3D eje;
	Punto3D centro;
	void Calcular();
	void ObtenerTriads(Triad *buffer);
	float numTrds;
	void Reset();

protected:
	void Puntos();
	Vector ejeu,ejev,ejew;
	Punto3D puntos[2][4];
	void Convertir();
};

void Cubo::Reset()
{
	int a;
	for(a=0;a<6;a++)
	{
		color[a]=RGB(255,255,255);
		lados[a].color=color[a];
	}

	escala=1;
	eje.x=0;
	eje.y=0;
	eje.z=0;
	centro=eje;
	ejeu.x=1;
	ejev.y=1;
	ejew.z=1;
	numTrds=12;
	Puntos();
	Convertir();
}

void Cubo::ObtenerTriads(Triad *buffer)
{
	int a,b=0;

	for(a=0;a<6;a++)
	{
		
		buffer[b].vertices[0]=lados[a].triads[0].vertices[0];
		buffer[b].vertices[1]=lados[a].triads[0].vertices[1];
		buffer[b].vertices[2]=lados[a].triads[0].vertices[2];
		buffer[b].color=lados[a].color;
		buffer[b].Calcular();
		b++;
		buffer[b].vertices[0]=lados[a].triads[1].vertices[0];
		buffer[b].vertices[1]=lados[a].triads[1].vertices[1];
		buffer[b].vertices[2]=lados[a].triads[1].vertices[2];
		buffer[b].color=lados[a].color;
		buffer[b].Calcular();
		b++;
	}
}

void Cubo::Calcular()
{
	int a,b;

	for(a=0;a<2;a++)
		for(b=0;b<4;b++)
		{
			puntos[a][b].x*=escala;
			puntos[a][b].y*=escala;
			puntos[a][b].z*=escala;
		}
	Convertir();
}

void Cubo::Puntos()
{

	puntos[0][0].x=0.5;
	puntos[0][0].y=0.5;
	puntos[0][0].z=0.5;

	puntos[0][1].x=-0.5;
	puntos[0][1].y=0.5;
	puntos[0][1].z=0.5;

	puntos[0][2].x=-0.5;
	puntos[0][2].y=-0.5;
	puntos[0][2].z=0.5;

	puntos[0][3].x=0.5;
	puntos[0][3].y=-0.5;
	puntos[0][3].z=0.5;

	puntos[1][0].x=0.5;
	puntos[1][0].y=0.5;
	puntos[1][0].z=-0.5;

	puntos[1][1].x=-0.5;
	puntos[1][1].y=0.5;
	puntos[1][1].z=-0.5;

	puntos[1][2].x=-0.5;
	puntos[1][2].y=-0.5;
	puntos[1][2].z=-0.5;

	puntos[1][3].x=0.5;
	puntos[1][3].y=-0.5;
	puntos[1][3].z=-0.5;
}

void Cubo::Convertir()
{
	lados[0].color=color[0];
	lados[1].color=color[1];
	lados[2].color=color[2];
	lados[3].color=color[3];
	lados[4].color=color[4];
	lados[5].color=color[5];


	lados[0].Vertices(puntos[0][0],puntos[0][1],puntos[0][2],puntos[0][3]);
	lados[1].Vertices(puntos[0][0],puntos[1][0],puntos[1][1],puntos[0][1]);
	lados[2].Vertices(puntos[1][0],puntos[1][3],puntos[1][2],puntos[1][1]);
	lados[3].Vertices(puntos[0][3],puntos[0][2],puntos[1][2],puntos[1][3]);
	lados[4].Vertices(puntos[1][0],puntos[0][0],puntos[0][3],puntos[1][3]);
	lados[5].Vertices(puntos[0][1],puntos[1][1],puntos[1][2],puntos[0][2]);
}

Cubo::Cubo()
{
	int a;
	for(a=0;a<6;a++)
	{
		lados[a].color=color[a]=RGB(255,255,255);
	}
	escala=1;
	eje.x=0;
	eje.y=0;
	eje.z=0;
	centro=eje;
	ejeu.x=1;
	ejev.y=1;
	ejew.z=1;
	numTrds=12;
	Puntos();
	Convertir();
}

class Camara
{
public:
	Punto3D po,pu,pv,pc;
	float a,b,c,d;
	Vector u,v,w;
	Vector un,vn,wn;
	Vector ejeu,ejev,ejec;
	Vector ref;
	Punto3D foco;
	float df;

	void Calcular();
	Camara();
	Camara(Punto3D poe,Punto3D pue,Punto3D pve);
	bool PParalela(Punto3D pt,Punto2D &pt2d);
	bool PPerspectiva(Punto3D pt,Punto2D &pt2d);
	void Vertices(Punto3D poe,Punto3D pue,Punto3D pve);
	void Mover(float magx, float magy, float magz);
	void Rotar(int tipo,float grados);
	void Ejes();
};


Punto2D	Punto2D::operator = (const Punto2D& pt)
{
	x=pt.x;
	y=pt.y;
	return(*this);
}


Punto3D	Punto3D::operator = (const Punto3D& pt)
{
	x=pt.x;
	y=pt.y;
	z=pt.z;
	return(*this);
}



Vector::Vector()
{
	x=0;
	y=0;
	z=0;
	norma=0;
}

Vector::Vector(float x, float y, float z)
{
	x=x;
	y=y;
	z=z;
}

Vector::Vector(Punto3D pt1, Punto3D pt2)
{
	x=pt2.x-pt1.x;
	y=pt2.y-pt1.y;
	z=pt2.z-pt1.z;
}

void Vector::Crear(Punto3D pt1, Punto3D pt2)
{
	x=pt2.x-pt1.x;
	y=pt2.y-pt1.y;
	z=pt2.z-pt1.z;
}


Vector::Vector(Punto3D pt)
{
	x=pt.x;
	y=pt.y;
	z=pt.z;
}

float Vector::Norma()
{
	#if normal
		norma=sqrt(x*x+y*y+z*z);
		return norma;
	#endif
	#if asm3dnow
		_asm
		{
			femms
			movd mm0,dword ptr [ecx]
			movd mm1,dword ptr [ecx+4]
			movd mm2,dword ptr [ecx+8]
			pfmul mm0,mm0
			pfmul mm1,mm1
			pfmul mm2,mm2
			pfadd mm0,mm1
			pfadd mm0,mm2
			movd dword ptr [ecx+12],mm0
			femms
			fld dword ptr [ecx+12]
			fsqrt
			fstp dword ptr [ecx+12]
		}
		return norma;
	#endif
	#if asmsse
		_asm
		{
			movss  xmm0,dword ptr [ecx]
			movss  xmm1,dword ptr [ecx+4]
			movss  xmm2,dword ptr [ecx+8]
			mulss xmm0,xmm0
			mulss xmm1,xmm1
			mulss xmm2,xmm2
			addss xmm0,xmm1
			addss xmm0,xmm2
			sqrtss xmm1,xmm0
			movss dword ptr [ecx+12],xmm1
		}
		return norma;
	#endif
}

void Vector::Normalizar()
{
	Norma();
	#if normal
		x/=norma;
		y/=norma;
		z/=norma;
	#endif
	#if asm3dnow
		float tn=1/norma;
		_asm
		{
			femms
			movd mm0,dword ptr [ecx]
			movd mm1,dword ptr [ecx+4]
			movd mm2,dword ptr [ecx+8]
			movd mm3,tn
			pfmul mm0,mm3
			pfmul mm1,mm3
			pfmul mm2,mm3
			movd dword ptr [ecx],mm0
			movd dword ptr [ecx+4],mm1
			movd dword ptr [ecx+8],mm2
			femms
		}
	#endif
	#if asmsse
		_asm
		{
			movss xmm0,dword ptr [ecx]			//x
			movss xmm1,dword ptr [ecx+4]		//y
			movss xmm2,dword ptr [ecx+8]		//z
			movss xmm3,dword ptr [ecx+12]		//norma
			divss xmm0,xmm3
			divss xmm1,xmm3
			divss xmm2,xmm3
			movss dword ptr [ecx],xmm0
			movss dword ptr [ecx+4],xmm1
			movss dword ptr [ecx+8],xmm2
		}
	#endif
}

Vector Vector::operator += (const Vector& Vec)
{
	x+=Vec.x;
	y+=Vec.y;
	z+=Vec.z;
	Norma();
	return(*this);
}

Vector Vector::operator = (const Vector& Vec)
{
	x=Vec.x;
	y=Vec.y;
	z=Vec.z;
	Norma();
	return(*this);
}


float Vector::operator * (const Vector& Vec)
{
#if asm3dnow
	float total;
	_asm
	{
		femms
		movd mm0,dword ptr [ecx]
		movd mm1,dword ptr [ecx+4]
		movd mm2,dword ptr [ecx+8]
		mov eax,dword ptr [esp+8]
		movd mm3,dword ptr [eax]	//Vec.x
 		movd mm4,dword ptr [eax+4]	//Vec.y
 		movd mm5,dword ptr [eax+8]	//Vec.z
		pfmul mm0,mm3
		pfmul mm1,mm4
		pfmul mm2,mm5
		pfadd mm0,mm1
		pfadd mm0,mm2
		movd total,mm0
		femms
	}
	return total;
#endif
#if asmsse
	float total;
	_asm
	{
		movss  xmm0,dword ptr [ecx]
		movss  xmm1,dword ptr [ecx+4]
		movss  xmm2,dword ptr [ecx+8]
		mov eax,dword ptr [esp+8]
		movss xmm3,dword ptr [eax]
		movss xmm4,dword ptr [eax+4]
		movss xmm5,dword ptr [eax+8]
		mulss xmm0,xmm3
		mulss xmm1,xmm4
		mulss xmm2,xmm5
		addss xmm0,xmm1
		addss xmm0,xmm2
		movss  total,xmm0
	}
	return total;
#endif
#if normal
	return x*Vec.x+y*Vec.y+z*Vec.z;
#endif
}



Vector Vector::operator ^ (const Vector& Vec)
{
	
	
	#if normal
		Vector temp;
		temp.x=(y*Vec.z)-(z*Vec.y);
		temp.y=(z*Vec.x)-(x*Vec.z);
		temp.z=(x*Vec.y)-(y*Vec.x);
	#endif
	#if asm3dnow
		_asm push ecx
		Vector temp;
		_asm
		{
			pop ecx
			movd mm0,dword ptr [ecx]	//x
			movd mm1,dword ptr [ecx+4]	//y
			movd mm2,dword ptr [ecx+8]	//z
			mov eax,dword ptr [ebp+0Ch]
			movd mm3,dword ptr [eax]	//Vec.x
 			movd mm4,dword ptr [eax+4]	//Vec.y
 			movd mm5,dword ptr [eax+8]	//Vec.z

			movq mm6,mm1
			movq mm7,mm2
			pfmul mm6,mm5
			pfmul mm7,mm4
			pfsub mm6,mm7
			movd temp.x,mm6

			movq mm6,mm2
			movq mm7,mm0
			pfmul mm6,mm3
			pfmul mm7,mm5
			pfsub mm6,mm7
			movd temp.y,mm6

			movq mm6,mm0
			movq mm7,mm1
			pfmul mm6,mm4
			pfmul mm7,mm3
			pfsub mm6,mm7
			movd temp.z,mm6
			femms
		}
	#endif
	#if asmsse
		_asm push ecx
		Vector temp;
		_asm
		{
			pop ecx

			movss xmm0,dword ptr [ecx]		//x
			movss xmm1,dword ptr [ecx+4]	//y
			movss xmm2,dword ptr [ecx+8]	//z
			mov eax,dword ptr [ebp+0Ch]
			movss xmm3,dword ptr [eax]		//Vec.x
 			movss xmm4,dword ptr [eax+4]	//Vec.y
 			movss xmm5,dword ptr [eax+8]	//Vec.z

			movss xmm6,xmm1
			movss xmm7,xmm2
			mulss xmm6,xmm5
			mulss xmm7,xmm4
			subss xmm6,xmm7
			movss temp.x,xmm6

			movss xmm6,xmm2
			movss xmm7,xmm0
			mulss xmm6,xmm3
			mulss xmm7,xmm5
			subss xmm6,xmm7
			movss temp.y,xmm6

			movss xmm6,xmm0
			movss xmm7,xmm1
			mulss xmm6,xmm4
			mulss xmm7,xmm3
			subss xmm6,xmm7
			movss temp.z,xmm6
		}	
	#endif
	return temp;
}

void Camara::Ejes()
{
	Punto3D pc=po,pcu=po,pcv=po;
	Vector mov;

	mov=u;
	mov.x*=0.5;
	mov.y*=0.5;
	mov.z*=0.5;

	pc.x+=mov.x;
	pc.y+=mov.y;
	pc.z+=mov.z;

	pcu=pc;

	mov=v;
	mov.x*=0.5;
	mov.y*=0.5;
	mov.z*=0.5;

	pcv.x+=mov.x;
	pcv.y+=mov.y;
	pcv.z+=mov.z;

	pc.x+=mov.x;
	pc.y+=mov.y;
	pc.z+=mov.z;

	ejeu.x=pcu.x;
	ejeu.y=pcu.y;
	ejeu.z=pcu.z;

	ejev.x=pcv.x;
	ejev.y=pcv.y;
	ejev.z=pcv.z;

	ejec.x=pc.x;
	ejec.y=pc.y;
	ejec.z=pc.z;

	ejeu.Normalizar();
	ejev.Normalizar();
	ejec.Normalizar();
}

Camara::Camara()
{
	a=0;
	b=0;
	c=0;
	d=0;
	df=1000;
}

Camara::Camara(Punto3D poe, Punto3D pue, Punto3D pve)
{
	po=poe;
	pu=pue;
	pv=pve;
	df=1000;
	Calcular();
	foco=pc;
	foco.x-=wn.x*df;
	foco.y-=wn.y*df;
	foco.z-=wn.z*df;
}

void Camara::Vertices(Punto3D poe, Punto3D pue, Punto3D pve)
{
	po=poe;
	pu=pue;
	pv=pve;
	Calcular();
	foco=pc;
	foco.x-=wn.x*df;
	foco.y-=wn.y*df;
	foco.z-=wn.z*df;
}

void Camara::Calcular()
{

	u.x=pu.x-po.x;
	u.y=pu.y-po.y;
	u.z=pu.z-po.z;

	v.x=pv.x-po.x;
	v.y=pv.y-po.y;
	v.z=pv.z-po.z;
	
	u.Norma();
	v.Norma();

	un=u;
	vn=v;
	un.Normalizar();
	vn.Normalizar();

	w=u^v;
	wn=w;
	w.Norma();
	wn.Normalizar();

	a=wn.x;
	b=wn.y;
	c=wn.z;
	d=-((a*po.x)+(b*po.y)+(c*po.z));

	pc=po;

	Vector tu=u,tv=v;

	tu.x*=0.5;
	tu.y*=0.5;
	tu.z*=0.5;

	tv.x*=0.5;
	tv.y*=0.5;
	tv.z*=0.5;

	pc.x+=tu.x;
	pc.y+=tu.y;
	pc.z+=tu.z;

	pc.x+=tv.x;
	pc.y+=tv.y;
	pc.z+=tv.z;
}

bool Camara::PPerspectiva(Punto3D pt,Punto2D &pt2d)
{
	float t,r;
	Vector nPt;

	t=(a*foco.x)+(b*foco.y)+(c*foco.z)+d;
	r=(a*(foco.x-pt.x))+(b*(foco.y-pt.y))+(c*(foco.z-pt.z));
	t/=r;

	nPt.x=foco.x+(t*(pt.x-foco.x));
	nPt.y=foco.y+(t*(pt.y-foco.y));
	nPt.z=foco.z+(t*(pt.z-foco.z));

	nPt.x-=po.x;
	nPt.y-=po.y;
	nPt.z-=po.z;

	pt2d.x=nPt*un;
	pt2d.y=nPt*vn;
	
	pt2d.x/=u.norma;
	pt2d.y/=v.norma;

	ref.x=-foco.x;
	ref.y=-foco.y;
	ref.z=-foco.z;
	ref.Normalizar();

	if(t<0 || t>1) return false;

	return true;
}

bool Camara::PParalela(Punto3D pt, Punto2D &pt2d)
{
	Punto3D pt3d;
	Vector	vec(pt,po),temp,temp2;
	float dist;
	int res;


	dist=fabs(vec*wn);

	//dist=sqrt(dist*dist);

	temp.x=wn.x*dist;
	temp.y=wn.y*dist;
	temp.z=wn.z*dist;
	pt3d.x=pt.x-temp.x;
	pt3d.y=pt.y-temp.y;
	pt3d.z=pt.z-temp.z;

	res=(a*pt3d.x)+(b*pt3d.y)+(c*pt3d.z)+d;


	if(res)
	{
		return false;
	}

	pt3d.x-=po.x;
	pt3d.y-=po.y;
	pt3d.z-=po.z;

	temp2.x=pt3d.x;
	temp2.y=pt3d.y;
	temp2.z=pt3d.z;

	pt2d.x=temp2*un;
	pt2d.y=temp2*vn;
	
	pt2d.x/=u.norma;
	pt2d.y/=v.norma;
	
	ref=wn;

	return true;
}

void Camara::Mover(float magx, float magy, float magz)
{
/*	po.x+=magx;
	pu.x+=magx;
	pv.x+=magx;

	po.y+=magy;
	pu.y+=magy;
	pv.y+=magy;

	po.z+=magz;
	pu.z+=magz;
	pv.z+=magz;*/
	foco.x+=magx;
	foco.y+=magy;
	foco.z+=magz;

//	Vertices(po,pu,pv);
	Calcular();
}

void Camara::Rotar(int tipo,float grados)
{
	int opcion;
	Vector eje;
	
//	Ejes();
	Calcular();
//	Vector to(pc,po),tu(pc,pu),tv(pc,pv);
	Vector to(po),tu(pu),tv(pv);
	Vector tf(po,foco);

	if(!tipo)return;

	grados*=GRADOS;
	opcion=tipo;
	switch(opcion)
	{
		case R_EJEU:
			eje=un;
			break;
		case R_EJEV:
			eje=vn;
			break;
		case R_EJEW:
			eje=wn;
			break;
	}

	Rotacion(to,eje,grados);
	Rotacion(tu,eje,grados);
	Rotacion(tv,eje,grados);
	Rotacion(tf,eje,grados);

/*	po.x=pc.x+to.x;
	po.y=pc.y+to.y;
	po.z=pc.z+to.z;
	
	pu.x=pc.x+tu.x;
	pu.y=pc.y+tu.y;
	pu.z=pc.z+tu.z;

	pv.x=pc.x+tv.x;
	pv.y=pc.y+tv.y;
	pv.z=pc.z+tv.z;*/
	
	po.x=to.x;
	po.y=to.y;
	po.z=to.z;
	
	pu.x=tu.x;
	pu.y=tu.y;
	pu.z=tu.z;

	pv.x=tv.x;
	pv.y=tv.y;
	pv.z=tv.z;


	foco=po;
	foco.x+=tf.x;
	foco.y+=tf.y;
	foco.z+=tf.z;

	Calcular();	
}


class Dona
{
public:
	Punto3D pc;
	float radio;
	float ancho;
	long p_dona;
	long p_or;
	COLORREF color;
	long numTrds;

	Dona(Punto3D tpc,float tradio,float tancho,long tdona,long tor);
	void Crear(Punto3D tpc,float tradio,float tancho,long tdona,long tor);
	void Calcular();
	void ObtenerTriads(Triad *buffer);
	~Dona();
protected:
	Punto3D **puntos;
	Punto3D pv,pu,pr;
	Vector u,v,w;
	Vector vn,wn;
	Quad *quads;
};

Dona::~Dona()
{
	int a;
	for(a=0;a<p_or;a++)
	{
		delete [] puntos[a];
	}
	delete [] puntos;
	delete [] quads;
}

void Dona::ObtenerTriads(Triad *buffer)
{
	int a,b=0;

	for(a=0;a<(p_or*p_dona);a++)
	{
		
		buffer[b].vertices[0]=quads[a].triads[0].vertices[0];
		buffer[b].vertices[1]=quads[a].triads[0].vertices[1];
		buffer[b].vertices[2]=quads[a].triads[0].vertices[2];
		buffer[b].color=quads[a].color;
		buffer[b].Calcular();
		b++;
		buffer[b].vertices[0]=quads[a].triads[1].vertices[0];
		buffer[b].vertices[1]=quads[a].triads[1].vertices[1];
		buffer[b].vertices[2]=quads[a].triads[1].vertices[2];
		buffer[b].color=quads[a].color;
		buffer[b].Calcular();
		b++;
	}
}

void Dona::Calcular()
{
	int a,b,c,y,z;
	float grados;

	Vector t1(pc,pu),t2(pc,pv);
	Vector temp(pu,pr);
	Vector *temps;
	temps=new Vector[p_or];
	u=t1;
	v=t2;
	w=u^v;
	
	vn=v;
	wn=w;

	vn.Normalizar();
	wn.Normalizar();

	grados=(float)360/(float)p_or;
	grados*=GRADOS;



	for(a=0;a<p_or;a++)
	{
		puntos[0][a].x=pu.x+temp.x;
		puntos[0][a].y=pu.y+temp.y;
		puntos[0][a].z=pu.z+temp.z;
		Rotacion(temp,wn,grados);
	}

	for(b=0;b<p_or;b++)
	{
		temps[b].x=puntos[0][b].x-pc.x;
		temps[b].y=puntos[0][b].y-pc.y;
		temps[b].z=puntos[0][b].z-pc.z;
	}

	grados=(float)360/(float)p_dona;
	grados*=GRADOS;
	for(a=0;a<p_dona;a++)
	{
		for(b=0;b<p_or;b++)
		{
			puntos[a][b].x=pc.x+temps[b].x;
			puntos[a][b].y=pc.y+temps[b].y;
			puntos[a][b].z=pc.z+temps[b].z;
			Rotacion(temps[b],vn,grados);
		}
	}

	c=0;
	for(a=0;a<p_dona;a++)
		for(b=0;b<p_or;b++)
		{
			y=a+1;
			z=b+1;
			if(y>=p_dona)y=0;
			if(z>=p_or)z=0;
			quads[c].color=color;
			quads[c].Vertices(puntos[a][b],puntos[y][b],puntos[y][z],puntos[a][z]);
			c++;
		}

	delete [] temps;
}

Dona::Dona(Punto3D tpc,float tradio,float tancho,long tdona,long tor)
{
	Crear(tpc,tradio,tancho,tdona,tor);
}

void Dona::Crear(Punto3D tpc,float tradio,float tancho,long tdona,long tor)
{
	int a;

	if(tdona<3 || tor<3)return;
	if(tdona>tor)return;

	if(p_or)
	{
		for(a=0;a<p_or;a++)
		{
			delete [] puntos[a];
		}
		delete [] puntos;
		delete [] quads;
	}

	tpc=pc;
	radio=tradio;
	ancho=tancho;
	p_dona=tdona;
	p_or=tor;
	pu=pc;
	pv=pc;
	pu.x+=radio;
	pv.y+=radio;
	pr=pu;
	pr.y+=ancho;



	color=RGB(0,255,0);
	
	numTrds=p_dona*p_or*2;

	puntos=(Punto3D **)new Punto3D[p_dona];
	quads=(Quad *)new Quad[p_dona*p_or];
	for(a=0;a<p_or;a++)
	{
		puntos[a]=new Punto3D[p_or];
	}

	Calcular();
}

float Distancia3D(float x, float y, float z)
{
	#if normal
		return sqrt((x*x)+(y*y)+(z*z));
	#endif
	#if asm3dnow
		float total;
		_asm
		{
			femms
			movd mm0,x
			movd mm1,y
			movd mm2,z

			pfmul mm0,mm0
			pfmul mm1,mm1
			pfmul mm2,mm2

			pfadd mm0,mm1
			pfadd mm0,mm2

			movd total,mm0

			femms
			fld total
			fsqrt
			fstp total
			femms

		}
		return total;
	#endif
	#if asmsse
		float total;
		_asm
		{
			movss xmm0,x
			movss xmm1,y
			movss xmm2,z

			mulss xmm0,xmm0
			mulss xmm1,xmm1
			mulss xmm2,xmm2

			addss xmm0,xmm1
			addss xmm0,xmm2
			
			sqrtss xmm0,xmm0

			movss total,xmm0
		}
		return total;

	#endif
}


void Ordenar_Triads(Triad *pTriads,unsigned long cantidad, Punto3D punto)
{
	unsigned long a,i,j;
	float *distancias;
	distancias=new float[cantidad];
	for(a=0;a<cantidad;a++)
		distancias[a]=Distancia3D(pTriads[a].vertices[0].x-punto.x,pTriads[a].vertices[0].y-punto.y,pTriads[a].vertices[0].z-punto.z);
	//Metodo de la Burbuja
	int f=1;
	float aux;
	Triad temp;
	/*for(j=cantidad-1;j>=0;j--)
	{
		if(!f)
   		break;
		f=0;
		for(i=0;i<j;i++)
		if(distancias[i]>distancias[i+1])
   		{
			aux=distancias[i];
			distancias[i]=distancias[i+1];
			distancias[i+1]=aux;
			temp=pTriads[i];
			pTriads[i]=pTriads[i+1];
			pTriads[i+1]=temp;
			f=1;
		}
	}*/
	for(j=0;j<cantidad;j++)
	{
		if(!f)
   		break;
		f=0;
		for(i=0;i<cantidad-j-1;i++)
			if(distancias[i]<distancias[i+1])
			{
				aux=distancias[i];
				distancias[i]=distancias[i+1];
				distancias[i+1]=aux;
				temp=pTriads[i];
				pTriads[i]=pTriads[i+1];
				pTriads[i+1]=temp;
				f=1;
		}
	}
	//Termina el metodo de la burbuja
}

long Buscar(Triad *pTriads,long cantidad, Punto3D punto)
{
	long a,b=0;
	float *distancias;
	distancias=new float[cantidad];
	for(a=0;a<cantidad;a++)
	{	
		distancias[a]=Distancia3D(pTriads[a].vertices[0].x-punto.x,pTriads[a].vertices[0].y-punto.y,pTriads[a].vertices[0].z-punto.z);
		if(distancias[a]>distancias[b])
			b=a;
	}
	//char texto[80];
	b-=cantidad/4;
	if(b<0)
		b=cantidad+b;
	
//	sprintf(texto,"%d",cantidad%4);
//	MessageBox(NULL,texto,NULL,NULL);
	return b;
}

void rRGB(COLORREF color,int &r,int &g, int &b)
{
	r=color&0x0000FF;
	g=(color&0x00FF00)>>8;
	b=(color&0xFF0000)>>16;
}


void Antialias(HDC imagen,long ix,long iy,long tx,long ty)
{
	long a,b,x;
	int rs[5],gs[5],bs[5];
	int tr,tg,tb;
	COLORREF color;
	int n_p;

	HDC bdc;
	HBITMAP bmp;
	bdc=CreateCompatibleDC(imagen);
	bmp=CreateCompatibleBitmap(imagen,tx-ix+1,ty-iy+1);
	SelectObject(bdc,bmp);
	BitBlt(bdc,0,0,tx-ix+1,ty-iy+1,imagen,ix,iy,SRCCOPY);

	for(b=iy;b<ty-1;b++)
		for(a=ix;a<tx-1;a++)
		{
			n_p=0;
			color=GetPixel(imagen,a,b);
			rRGB(color,rs[n_p],gs[n_p],bs[n_p]);
			n_p++;
			if(b>iy && a>ix)
			{
				color=GetPixel(imagen,a-1,b-1);
				rRGB(color,rs[n_p],gs[n_p],bs[n_p]);
				n_p++;
			}
			
			if(b>iy && a<tx)
			{
				color=GetPixel(imagen,a+1,b-1);
				rRGB(color,rs[n_p],gs[n_p],bs[n_p]);
				n_p++;
			}
			
			if(b<ty && a>ix)
			{
				color=GetPixel(imagen,a-1,b+1);
				rRGB(color,rs[n_p],gs[n_p],bs[n_p]);
				n_p++;
			}

			if(b<ty && a<tx)
			{
				color=GetPixel(imagen,a+1,b+1);
				rRGB(color,rs[n_p],gs[n_p],bs[n_p]);
				n_p++;
			}
			tr=0;
			tg=0;
			tb=0;
			for(x=0;x<n_p;x++)
			{
				tr+=rs[x];
				tg+=gs[x];
				tb+=bs[x];
			}
			tr/=n_p;
			tg/=n_p;
			tb/=n_p;
			SetPixel(bdc,a-ix,b-iy,RGB(tr,tg,tb));
		}
		BitBlt(imagen,ix,iy,tx,ty,bdc,0,0,SRCCOPY);
		DeleteObject(bmp);
		ReleaseDC(NULL,bdc);
		DeleteObject(bdc);
}
/*
class Esfera
{
public:
	Punto3D pc;
	float radio;
	long p_esf,p_t;
	COLORREF color;
	long numTrds;

	Esfera(Punto3D tpc,float tradio,long tesf);
	void Crear(Punto3D tpc,float tradio,long tesf);
	void Calcular();
	void ObtenerTriads(Triad *buffer);
	~Esfera();
protected:
	Punto3D **puntos;
	Punto3D pv,pu,pr;
	Vector u,v,w;
	Vector vn,wn;
	Quad *quads;
};

Esfera::~Esfera()
{
	int a;
	for(a=0;a<p_t;a++)
	{
		delete [] puntos[a];
	}
	delete [] puntos;
	delete [] quads;
}

void Esfera::ObtenerTriads(Triad *buffer)
{
	int a,b=0;

	for(a=0;a<(p_esf*p_esf/2);a++)
	{
		
		buffer[b].vertices[0]=quads[a].triads[0].vertices[0];
		buffer[b].vertices[1]=quads[a].triads[0].vertices[1];
		buffer[b].vertices[2]=quads[a].triads[0].vertices[2];
		buffer[b].color=quads[a].color;
		buffer[b].Calcular();
		b++;
		buffer[b].vertices[0]=quads[a].triads[1].vertices[0];
		buffer[b].vertices[1]=quads[a].triads[1].vertices[1];
		buffer[b].vertices[2]=quads[a].triads[1].vertices[2];
		buffer[b].color=quads[a].color;
		buffer[b].Calcular();
		b++;
	}
}

void Esfera::Calcular()
{
	int a,b,c,y,z;
	float grados;

	Vector t1(pc,pu),t2(pc,pv);
	Vector temp(pu,pr);
	Vector *temps;
	temps=new Vector[p_or];
	u=t1;
	v=t2;
	w=u^v;
	
	vn=v;
	wn=w;

	vn.Normalizar();
	wn.Normalizar();

	grados=(float)360/(float)p_or;
	grados*=GRADOS;



	for(a=0;a<p_or;a++)
	{
		puntos[0][a].x=pu.x+temp.x;
		puntos[0][a].y=pu.y+temp.y;
		puntos[0][a].z=pu.z+temp.z;
		Rotacion(temp,wn,grados);
	}

	for(b=0;b<p_or;b++)
	{
		temps[b].x=puntos[0][b].x-pc.x;
		temps[b].y=puntos[0][b].y-pc.y;
		temps[b].z=puntos[0][b].z-pc.z;
	}

	grados=(float)360/(float)p_dona;
	grados*=GRADOS;
	for(a=0;a<p_dona;a++)
	{
		for(b=0;b<p_or;b++)
		{
			puntos[a][b].x=pc.x+temps[b].x;
			puntos[a][b].y=pc.y+temps[b].y;
			puntos[a][b].z=pc.z+temps[b].z;
			Rotacion(temps[b],vn,grados);
		}
	}

	c=0;
	for(a=0;a<p_dona;a++)
		for(b=0;b<p_or;b++)
		{
			y=a+1;
			z=b+1;
			if(y>=p_dona)y=0;
			if(z>=p_or)z=0;
			quads[c].color=color;
			quads[c].Vertices(puntos[a][b],puntos[y][b],puntos[y][z],puntos[a][z]);
			c++;
		}

	delete [] temps;
}

Esfera::Esfera(Punto3D tpc,float tradio,long tesf)
{
	Crear(tpc,tradio,tancho,tdona,tor);
}

void Esfera::Esfera(Punto3D tpc,float tradio,long tesf)
{
	int a;

	if(tdona<3 || tor<3)return;
	if(tdona>tor)return;

	if(p_or)
	{
		for(a=0;a<p_or;a++)
		{
			delete [] puntos[a];
		}
		delete [] puntos;
		delete [] quads;
	}

	tpc=pc;
	radio=tradio;
	ancho=tancho;
	p_dona=tdona;
	p_or=tor;
	pu=pc;
	pv=pc;
	pu.x+=radio;
	pv.y+=radio;
	pr=pu;
	pr.y+=ancho;



	color=RGB(0,255,0);
	
	numTrds=p_dona*p_or*2;

	puntos=(Punto3D **)new Punto3D[p_dona];
	quads=(Quad *)new Quad[p_dona*p_or];
	for(a=0;a<p_or;a++)
	{
		puntos[a]=new Punto3D[p_or];
	}

	Calcular();
}*/