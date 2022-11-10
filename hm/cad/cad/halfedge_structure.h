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
	int id;	//��ı��
	double coord[3];	//�������
	Vertex* pre;	
	Vertex* next;	
	HalfEdge* halfedge;	//�����ĸ����
};

class HalfEdge {
public:
	HalfEdge() :edge(NULL), pre(NULL), next(NULL), brother(NULL), loop(NULL), vertex(NULL) {}
	Edge* edge;	//�ð�������ĸ������
	HalfEdge* brother;	//�ð�ߵĶ԰��
	HalfEdge* pre;	
	HalfEdge* next;		
	Loop* loop;	//�ð�������ĸ���
	Vertex* vertex;	//�ð�ߵ���ʼ��
};

class Edge {
public:
	Edge() :id(0), pre(NULL), next(NULL), lhe(NULL), rhe(NULL) {}
	int id;	//����ߵı��
	Edge* pre;	
	Edge* next;		
	HalfEdge* lhe;	//����
	HalfEdge* rhe;	//�Ұ��
	Solid* solid;	//�ñ������ĸ���
};

class Loop {
public:
	Loop() :id(0), pre(NULL), next(NULL), halfedgelist(NULL), face(NULL) {}
	int id;	//���ı��
	Loop* pre;	
	Loop* next;		
	HalfEdge* halfedgelist;		//�������İ�߱�
	Face* face;		//�û������ĸ���
};

class Face {
public:
	Face() :id(0), next(NULL), pre(NULL), solid(NULL), loopout(NULL), loopinlist(NULL) {}
	int id;	//��ı��
	Face* next;	
	Face* pre;	
	Solid* solid;	//���������ĸ���
	Loop* loopout;	//����������⻷
	Loop* loopinlist;	//����������ڻ���
};

class Solid {
public:
	Solid() : vnum(0), fnum(0), lnum(0), ednum(0), facelist(NULL), edgelist(NULL), firstv(NULL), newv(NULL),next(NULL), pre(NULL) {}
	void wirepaint()	//�����߿�
	{
		glBegin(GL_LINES);	//����˫�����߶�
		Edge* tmp;
		tmp = this->edgelist;
		while (tmp)
		{
			glVertex3f(tmp->lhe->vertex->coord[0], tmp->lhe->vertex->coord[1], tmp->lhe->vertex->coord[2]);	//����һ���㣬һ����glBegin()��glEnd()��������������ͼ��
			glVertex3f(tmp->rhe->vertex->coord[0], tmp->rhe->vertex->coord[1], tmp->rhe->vertex->coord[2]);
			tmp = tmp->next;
			if (tmp == NULL)
			{
				break;
			}
		}
		glEnd();
	}
	
	void solidpaint()	//����ʵ��
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
			glBegin(GL_POLYGON);	//���������͹�����
			lp = tmp->loopout;
			he = lp->halfedgelist;
			Coptions = Coptions % 6;
			glColor3fv(colors[Coptions]);	//��ʾ��ɫ
			while (he)
			{
				glVertex3f(he->vertex->coord[0], he->vertex->coord[1], he->vertex->coord[2]);//����һ���㣬һ����glBegin()��glEnd()��������������ͼ��
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
	Face* facelist;	//������ʵ���������������߿�ͼ
	Edge* edgelist;	//������ʵ��ı߱���������ʵ��ͼ
	Vertex* firstv;	//����ʵ��ĵ�һ����
	Vertex* newv;	//ͨ��mev����ʵ����µ�
	Solid* next;	
	Solid* pre;	
	int fnum;	//������ʵ����桢�����㡢�ߵĸ���,���������Ǳ��
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


