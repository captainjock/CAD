#ifndef _HALFEDGE_STRUCTUR_H
#define _HALFEDGE_STRUCTUR_H

class Solid;
class Face;
class Loop;
class HalfEdge;
class Edge;
class Vertex;


class Vertex {
public:
	Vertex(double x, double y, double z) :id(0), next(NULL), pre(NULL), halfedge(NULL) {
		coord[0] = x;
		coord[1] = y;
		coord[2] = z;
	}
	int id;	//点的编号
	double coord[3];	//点的坐标
	Vertex* pre;	
	Vertex* next;	
	HalfEdge* halfedge;	//属于哪个半边
};

class HalfEdge {
public:
	HalfEdge() :edge(NULL), pre(NULL), next(NULL), brother(NULL), loop(NULL), vertex(NULL) {}
	Edge* edge;	//该半边属于哪个物理边
	HalfEdge* brother;	//该半边的对半边
	HalfEdge* pre;	
	HalfEdge* next;		
	Loop* loop;	//该半边属于哪个环
	Vertex* vertex;	//该半边的起始点
};

class Edge {
public:
	Edge() :id(0), pre(NULL), next(NULL), lhe(NULL), rhe(NULL) {}
	int id;	//物理边的编号
	Edge* pre;	
	Edge* next;		
	HalfEdge* lhe;	//左半边
	HalfEdge* rhe;	//右半边
	Solid* solid;	//该边属于哪个体
};

class Loop {
public:
	Loop() :id(0), pre(NULL), next(NULL), halfedgelist(NULL), face(NULL) {}
	int id;	//环的编号
	Loop* pre;	
	Loop* next;		
	HalfEdge* halfedgelist;		//构建环的半边表
	Face* face;		//该环属于哪个面
};

class Face {
public:
	Face() :id(0), next(NULL), pre(NULL), solid(NULL), loopout(NULL), loopinlist(NULL) {}
	int id;	//面的编号
	Face* next;	
	Face* pre;	
	Solid* solid;	//该面属于哪个体
	Loop* loopout;	//构建该面的外环
	Loop* loopinlist;	//构建该面的内环表
};

class Solid {
public:
	Solid() : vnum(0), fnum(0), lnum(0), ednum(0), facelist(NULL), edgelist(NULL), firstv(NULL), newv(NULL),next(NULL), pre(NULL) {}
	void wirepaint()	//绘制线框
	{
		glBegin(GL_LINES);	//多组双顶点线段
		Edge* tmp;
		tmp = this->edgelist;
		while (tmp)
		{
			glVertex3f(tmp->lhe->vertex->coord[0], tmp->lhe->vertex->coord[1], tmp->lhe->vertex->coord[2]);	//定义一个点，一般与glBegin()、glEnd()函数连用来绘制图像
			glVertex3f(tmp->rhe->vertex->coord[0], tmp->rhe->vertex->coord[1], tmp->rhe->vertex->coord[2]);
			tmp = tmp->next;
			if (tmp == NULL)
			{
				break;
			}
		}
		glEnd();
	}
	
	void solidpaint()	//绘制实体
	{
		Face* tmp;
		Loop* lp;
		GLfloat colors[][3] = { {1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},
		{0.0,1.0,1.0},{1.0,0.0,1.0},{1.0,1.0,0.0} };
		int Coptions;
		tmp = this->facelist;
		HalfEdge* he;
		Coptions = 0;
		while (tmp)
		{
			glBegin(GL_POLYGON);	//单个简单填充凸多边形
			lp = tmp->loopout;
			he = lp->halfedgelist;
			Coptions = Coptions % 6;
			glColor3fv(colors[Coptions]);	//表示颜色
			while (he)
			{
				glVertex3f(he->vertex->coord[0], he->vertex->coord[1], he->vertex->coord[2]);//定义一个点，一般与glBegin()、glEnd()函数连用来绘制图像
				he = he->next;
				if (he == lp->halfedgelist)
				{
					break;
				}
			}
			Coptions++;
			glEnd();
			tmp = tmp->next;
			if (tmp == this->facelist)
			{
				break;
			}
		}
	}
	Face* facelist;	//构建该实体的面表，用来绘制线框图
	Edge* edgelist;	//构建该实体的边表，用来绘制实体图
	Vertex* firstv;	//构建实体的第一个点
	Vertex* newv;	//通过mev构建实体的新点
	Solid* next;	
	Solid* pre;	
	int fnum;	//构建该实体的面、环、点、边的个数,用来给它们编号
	int lnum;
	int vnum;
	int ednum;
};

static Solid* solidlist = NULL;
void  AddToSolidList(Solid* s)
{
	if (solidlist == NULL)
	{
		solidlist = s;
	}
	else
	{
		s->next = solidlist;
		s->pre = solidlist->pre;
		solidlist->pre->next = s;
		solidlist->pre = s;
	}
}
#endif


