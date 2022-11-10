#include<iostream>
#include<glut.h>
#include"halfedge_structure.h"
#include"eularoperation.h"
#define PI 3.1415926536
using namespace std;
double xrot = 0.0;
double yrot = 0.0;
double xrotr = 0.0;
double yrotr = 0.0;
double points_first[] = { 1.0, 1.0, 0.0 };
double points_second[] = { -1.0, 1.0, 0.0 }; 
double points_third[] = { -1.0, -1.0, 0.0 }; 
double points_forth[] = { 1.0, -1.0, 0.0 }; 
double points_in_first[] = { 0.5, 0.5, 0.0 };
double points_in_second[] = { -0.5, 0.5, 0.0 };
double points_in_third[] = { -0.5, -0.5, 0.0 };
double points_in_forth[] = { 0.5, -0.5, 0.0 };
double dir[] = { 0.0,0.0, 1.0 };
double d = 2.0;
bool displaymode = 0;
int xp, yp;
bool bdown = false;

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);	//�����ɫ
	glShadeModel(GL_FLAT);	//������ɫģʽ��Ϊ��ɫ�ǹ���
	glClearDepth(1.0);	//ָ����Ȼ����������ֵ���ڵ��Ƿ�ɼ�
	glEnable(GL_DEPTH_TEST);	//����������Ȼ������Ĺ���
	glDepthFunc(GL_LEQUAL);	//ָ����Ȼ������Ƚ�ֵ
	glClearDepth(1.0f);	//ָ����Ȼ����������ֵ
	Solid *s = mvfs(points_first);	//����һ��ʵ��
	Loop* lp_out = s->facelist->loopout;
	mev(s->firstv, points_second, lp_out);
	mev(s->newv, points_third, lp_out);
	mev(s->newv, points_forth, lp_out);
	Loop* lp=mef(s->newv, s->firstv, lp_out);
	mev(s->firstv, points_in_first, lp);
	HalfEdge* he = mev(s->newv, points_in_second, lp);
	Vertex* v = he->vertex;
	mev(s->newv, points_in_third, lp);
	mev(s->newv, points_in_forth, lp);
	Loop* lp_in=mef(s->newv,v , lp);
	kemr(s->firstv, v, lp);
	kfmrh(lp_out->face,lp_in->face);
	AddToSolidList(s);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//�����ɫ
	glColor3f(1.0, 1.0, 1.0);	//��ɫ�趨����ɫ
	glLoadIdentity();	//��λ
	glTranslatef(-2.0, 0.0, -10.0);	//��λ
	glRotatef(xrot + xrotr, 1.0, 0.0, 0.0);	//������ת��ʽ
	glRotatef(yrot + yrotr, 0.0, 1.0, 0.0);	//������ת��ʽ
	Solid* tmp;
	tmp = solidlist;
	while (tmp) {
		if (!displaymode)
			tmp->wirepaint();
		else
			tmp->solidpaint();
		tmp = tmp->next;
		if (tmp == solidlist)
			break;
		glTranslatef(3.0, 0, 0);	//��λ
	}
	glutSwapBuffers();	//ʹ���ƵĶ���ͬʱ��������Ļ�ϣ������Ƶ��ˢ�µ��»�����˸����
}

void reshape(int w, int h)
{
	glViewport(0,0,w,h);	//�����ӿ�λ�á���Ⱥ͸߶�
	glMatrixMode(GL_PROJECTION);	//����ǰ����ָ��ΪͶӰ������glLoadldentityһͬʹ��
	glLoadIdentity();	//�Ѿ�����Ϊ��λ���󣬽���ǰ�û�����ϵ��ԭ���ƶ���Ļ���ģ���λ��
	gluPerspective(45,(double)w/(double)h,0.1,100.0);	//͸��ͶӰ������1Ϊ��Ұ�����ķ��ȣ�����2��ʾ�ü���Ŀ�߱ȣ�����3Ϊ���ü��浽�۾��ľ���
	glMatrixMode(GL_MODELVIEW);	//�ѵ�ǰ����ָ��Ϊģ����ͼ
	glLoadIdentity();
}
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			bdown = true;
			xp = x;
			yp = y;
		}
		else
		{
			bdown = false;
			xrot += xrotr;
			yrot += yrotr;
		}
	}
}
void move(int x, int y)
{
	int deltx, delty;
	if (bdown)
	{
		deltx = x - xp;
		delty = y - yp;
		yrotr = ((double)deltx / 10.0 * 180.0 / PI * 0.04);
		xrotr = ((double)delty / 10.0 * 180.0 / PI * 0.04);
	}
	glutPostRedisplay();
}
void processMenuEvent(int option)
{
	switch (option)
	{
	case 1:
		displaymode = false;
		glutPostRedisplay();
		break;
	case 2:
		displaymode = true;
		glutPostRedisplay();
		break;
	case 3:
		sweep(solidlist->facelist->next, dir, d);
		glutPostRedisplay();
	default:break;
	}
}
void glCreateMenu(void)
{
	int menu;
	menu = glutCreateMenu(processMenuEvent);
	glutAddMenuEntry("wire cube", 1);
	glutAddMenuEntry("solid cube", 2);
	glutAddMenuEntry("sweep model", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);	//��ʼ��GLUT��
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);	//������ʾģʽ
	glutInitWindowPosition(200, 150);	//���ô���λ��
	glutInitWindowSize(500, 500);	//���ô��ڴ�С
	glutCreateWindow("Euler");	//�������ڲ����崰������
	init();	//����ʵ��ļ������˹�ϵ
	glutDisplayFunc(display);	//����display�������л���
	glutReshapeFunc(reshape);	//�������ڴ�С��������ͼ��߱�
	glutMouseFunc(mouse);	//���������
	glutMotionFunc(move);	//����ƶ�����
	glCreateMenu();	//������Ӳ˵�����
	glutMainLoop();	//������ѭ��
	return 0;
}
