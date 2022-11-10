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
	glClearColor(0.0, 0.0, 0.0, 0.0);	//清空颜色
	glShadeModel(GL_FLAT);	//设置着色模式，为颜色非过渡
	glClearDepth(1.0);	//指定深度缓冲区的清楚值，遮挡是否可见
	glEnable(GL_DEPTH_TEST);	//开启更新深度缓冲区的功能
	glDepthFunc(GL_LEQUAL);	//指定深度缓冲区比较值
	glClearDepth(1.0f);	//指定深度缓冲区的清楚值
	Solid *s = mvfs(points_first);	//创建一个实体
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清空颜色
	glColor3f(1.0, 1.0, 1.0);	//颜色设定，白色
	glLoadIdentity();	//复位
	glTranslatef(-2.0, 0.0, -10.0);	//复位
	glRotatef(xrot + xrotr, 1.0, 0.0, 0.0);	//设置旋转方式
	glRotatef(yrot + yrotr, 0.0, 1.0, 0.0);	//设置旋转方式
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
		glTranslatef(3.0, 0, 0);	//复位
	}
	glutSwapBuffers();	//使绘制的东西同时出现在屏幕上，解决因频繁刷新导致画面闪烁问题
}

void reshape(int w, int h)
{
	glViewport(0,0,w,h);	//定义视口位置、宽度和高度
	glMatrixMode(GL_PROJECTION);	//将当前矩阵指定为投影矩阵，与glLoadldentity一同使用
	glLoadIdentity();	//把矩阵设为单位矩阵，将当前用户坐标系的原点移动屏幕中心（复位）
	gluPerspective(45,(double)w/(double)h,0.1,100.0);	//透视投影，参数1为视野睁开的幅度，参数2表示裁剪面的宽高比，参数3为近裁剪面到眼睛的距离
	glMatrixMode(GL_MODELVIEW);	//把当前矩阵指定为模型视图
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
	glutInit(&argc, argv);	//初始化GLUT库
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);	//设置显示模式
	glutInitWindowPosition(200, 150);	//设置窗口位置
	glutInitWindowSize(500, 500);	//设置窗口大小
	glutCreateWindow("Euler");	//创建窗口并定义窗口名称
	init();	//构建实体的几何拓扑关系
	glutDisplayFunc(display);	//调用display函数进行绘制
	glutReshapeFunc(reshape);	//调整窗口大小，保持视图宽高比
	glutMouseFunc(mouse);	//鼠标点击操作
	glutMotionFunc(move);	//鼠标移动操作
	glCreateMenu();	//创建添加菜单函数
	glutMainLoop();	//进入主循环
	return 0;
}
