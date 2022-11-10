#ifndef _EULAROPERATION.H
#include"halfedge_structure.h"
#include<vector>
using namespace std;

Solid* mvfs(double point[3]) {		//生成一个点、一个面、一个空环和一个体
	Solid* s = new Solid();	
	Face* f = new Face();	
	Loop* lpout = new Loop();	
	Vertex* v = new Vertex(point[0], point[1], point[2]);	
	v->id = s->vnum;
	f->id = s->fnum;
	lpout->id = s->lnum;
	s->vnum += 1;
	s->fnum += 1;
	s->lnum += 1;
	s->firstv = v;
	s->facelist = f;	//将面加入到实体的面表中
	f->loopout = lpout;
	f->solid = s;
	lpout->face = f;
	return s;
}

HalfEdge* mev(Vertex* sv, double point[3], Loop* lp) {		//生成一个新点，同时生成一条连接新点与已有点的边
	HalfEdge* half_l = new HalfEdge();
	HalfEdge* half_r = new HalfEdge();
	Edge* edge = new Edge();
	Vertex* ev = new Vertex(point[0], point[1], point[2]);
	Solid* s = lp->face->solid;
	ev->id = s->vnum;
	edge->id = s->ednum;
	s->vnum += 1;
	s->ednum += 1;
	s->newv = ev;
	half_l->vertex = sv;
	half_r->vertex = ev;
	half_l->edge = edge;
	half_r->edge = edge;
	half_l->brother = half_r;
	half_r->brother = half_l;
	edge->lhe = half_l;
	edge->rhe = half_r;
	edge->solid = s;
	half_l->loop = lp;
	half_r->loop = lp;
	sv->next = ev;
	ev->pre = sv;
	//将半边加入到环的环表中
	if (lp->halfedgelist == NULL) {	//如果是空环的情况
		half_l->next = half_r;
		half_r->next = half_l;
		half_l->pre = half_r;
		half_r->pre = half_l;
		lp->halfedgelist = half_l;
	}
	else {	//如果是非空环的情况
		HalfEdge* half = lp->halfedgelist;
		while (half->next->vertex != sv)
			half = half->next;	//找到sv的前一条边
		half_r->next = half->next;
		half->next->pre = half_r;
		half->next = half_l;
		half_l->pre = half;
		half_l->next = half_r;
		half_r->pre = half_l;
	}
	//将边加入到实体的边表中
	Edge* e = s->edgelist;	
	if (e == NULL)
		s->edgelist = edge;
	else {
		while (e ->next!= NULL)
			e = e->next;
		e->next= edge;
		edge->pre = e;
	}
	return half_l;
}

Loop* mef(Vertex* sv, Vertex* ev, Loop* loop) {	//连接面上的两个点生成一条新边，并产生一个新面（同时生成一个新环）
	Solid* solid = loop->face->solid;
	Edge* edge = new Edge();
	HalfEdge* half_l = new HalfEdge();
	HalfEdge* half_r = new HalfEdge();
	Face* face = new Face();
	Loop* newLoop = new Loop();
	edge->id = solid->ednum;
	face->id = solid->fnum;
	newLoop->id = solid->lnum;
	solid->ednum += 1;
	solid->fnum += 1;
	solid->lnum += 1;
	half_l->vertex = sv;
	half_r->vertex = ev;
	half_r->brother = half_l;
	half_r->brother = half_r;
	half_l->edge = edge;
	half_r->edge = edge;
	edge->lhe = half_l;
	edge->rhe = half_r;
	face->solid = solid;
	edge->solid = solid;
	newLoop->face = face;
	sv->next = ev;
	ev->pre = sv;
	HalfEdge* half = loop->halfedgelist;	
	HalfEdge* halfa, * halfb;
	while (half->vertex != sv)
		half = half->next;
	halfa = half->pre;	//找到sv的前一条边
	while (half->vertex != ev)
		half = half->next;
	halfb = half->pre;	//找到ev的前一条边
	//将两个半边分别加入到内环和外环的环表中
	half_r->next = halfa->next;
	halfa->next->pre = half_r;
	halfa->next = half_l;
	half_l->pre = halfa;
	half_l->next = halfb->next;
	halfb->next->pre = half_l;
	halfb->next = half_r;
	half_r->pre = halfb;
	loop->halfedgelist = half_l;
	newLoop->halfedgelist = half_r;
	half_l->loop = loop;
	half_r->loop = newLoop;
	//将面加入到实体的面表中
	Face* f = solid->facelist;	
	if (f == NULL)
		solid->facelist = face;
	else {
		while (f->next != NULL)
			f = f->next;
		f->next = face;
		face->pre = f;
	}
	//将环加入到面的环表中
	if (face->loopout == NULL)	
		face->loopout = newLoop;
	else {
		Loop* l = face->loopinlist;
		if (l == NULL)
			face->loopinlist = newLoop;
		else {
			while (l->next != NULL)
				l = l->next;
			l->next = newLoop;
			newLoop->pre = l;
		}
	}
	//将边加入到体的边表中
	Edge* e = solid->edgelist;	
	if (e == NULL)
		solid->edgelist = edge;
	else {
		while (e->next != NULL)
			e = e->next;
		e->next = edge;
		edge->pre = e;
	}
	return newLoop;
}

Loop* kemr(Vertex* sv, Vertex* ev, Loop* loop) {	//消去环中的一条边，生成该边某一邻面上的新内环；sv必须属于外环
	HalfEdge* hea, * heb, * he;
	Face* face = loop->face;
	Solid* s = face->solid;
	Loop* loopin = new Loop();
	he = loop->halfedgelist;
	while (he->vertex != sv || he->next->vertex != ev)	//找到桥边的左半边
		he = he->next;
	hea = he;
	while (he->vertex != ev || he->next->vertex != sv)	//找到桥边的右半边
		he = he->next;
	heb = he;
	heb->pre->next = hea->next;
	hea->next->pre = heb->pre;
	hea->pre->next = heb->next;
	heb->next->pre = hea->pre;
	loopin->halfedgelist = heb->pre;
	loopin->id = s->lnum;
	loopin->face = face;
	s->lnum += 1;
	Edge* e = hea->edge;
	Loop* l = face->loopinlist;//将环加入到面中
	if (l == NULL)
		face->loopinlist = loopin;
	else {
		while (l->next != NULL)
			l = l->next;
			l->next = loopin;
			loopin->pre = l;
	}
	e->pre->next = e->next;
	e->next->pre = e->pre;
	delete e;
	delete hea;	//删除桥边
	delete heb;
	return loopin;
}

void kfmrh(Face* fa, Face* fb) {	//删除与面Fa相接触的一个面Fb，并将其定义为面F1上的一个内环，进而在体内构造一个通孔(通常之前存在一个内环与新生成的内环连通从而形成柄)；fa为外面，fb为内面
	Loop* loop = fb->loopout;
	Loop* lp = fa->loopinlist;		//将内环加到面fa上
	if (lp == NULL)
		fa->loopinlist = loop;
	else
	{
		while (lp->next != NULL)
			lp = lp->next;
		lp->next = loop;
		loop->pre = lp;
	}
	fa->solid->fnum -= 1;
	Solid* s = fa->solid;
	Face* face = s->facelist;
	if (face == fb)
		s->facelist = face->next;
	else {
		while (face != fb) {
			face=face->next;
		}
		face->pre->next = face->next;
	}
	delete fb;
}


void sweep(Face* face, double dir[3], double d) {

	Loop* lpout = face->loopout;
	Solid* solid = face->solid;
	Vertex* firstv, * nextv, * prevup, * up, * firstup;
	HalfEdge* he, * suphe, * uphe;
	double point[3] = { 0.0,0.0,0.0 };
	he = lpout->halfedgelist;
	firstv = he->vertex;
	point[0] = firstv->coord[0] + d * dir[0];
	point[1] = firstv->coord[1] + d * dir[1];
	point[2] = firstv->coord[2] + d * dir[2];
	suphe = mev(firstv, point, lpout);	//构造一条新边
	prevup = solid->newv;	//令prevup为新点
	firstup = solid->newv;
	he = he->next;
	nextv = he->vertex;
	while (nextv != firstv) {
		point[0] = nextv->coord[0] + d * dir[0];
		point[1] = nextv->coord[1] + d * dir[1];
		point[2] = nextv->coord[2] + d * dir[2];
		uphe = mev(nextv, point, lpout);
		mef(prevup, solid->newv, lpout);
		prevup = solid->newv;
		he = he->next;
		nextv = he->vertex;
	}
	mef(prevup, firstup, lpout);
	Loop* lpin = face->pre->loopout;
	he = lpin->halfedgelist;
	firstv = he->vertex;
	point[0] = firstv->coord[0] + d * dir[0];
	point[1] = firstv->coord[1] + d * dir[1];
	point[2] = firstv->coord[2] + d * dir[2];
	suphe = mev(firstv, point, lpin);	
	prevup = solid->newv;	
	firstup = solid->newv;
	he = he->next;
	nextv = he->vertex;
	while (nextv != firstv) {
		point[0] = nextv->coord[0] + d * dir[0];
		point[1] = nextv->coord[1] + d * dir[1];
		point[2] = nextv->coord[2] + d * dir[2];
		uphe = mev(nextv, point, lpin);
		mef(prevup, solid->newv, lpin);
		prevup = solid->newv;
		he = he->next;
		nextv = he->vertex;
	}
	mef(prevup, firstup, lpin);
}
#endif