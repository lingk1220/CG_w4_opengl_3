#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>
#include <math.h>
#define RECTINITSIZE 0.2

GLfloat	dx[4] = { 1.0f, -1.0f, -1.0f, 1.0f };
GLfloat dy[4] = { 1.0f, 1.0f, -1.0f, -1.0f };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void draw_rect(int index);
void clamp_pos(GLfloat* input_pos);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
int rect_find_top(GLfloat* input_pos);
int rect_find_overlap();
void rect_merge();

std::vector<struct rect> rectangles;

int select_index = -1;

struct rect {
	GLfloat x1;
	GLfloat y1;
	GLfloat x2;
	GLfloat y2;

	GLclampf r;
	GLclampf g;
	GLclampf b;
	GLclampf a = 0;


	rect() {

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		int viewport_width = viewport[2];
		int viewport_height = viewport[3];
		x1 = (float)(rand() % viewport_width) / viewport_width * 2 - 1.0f;
		x2 = x1 + RECTINITSIZE;
		y1 = (float)(rand() % viewport_height) / viewport_height * 2 - 1.0f;
		y2 = y1 + RECTINITSIZE;

		r = (float)(rand() % 256) / 256;
		g = (float)(rand() % 256) / 256;
		b = (float)(rand() % 256) / 256;
	}

	rect(GLfloat ix1, GLfloat iy1, GLfloat ix2, GLfloat iy2) {
		x1 = ix1;
		x2 = ix2;
		y1 = iy1;
		y2 = iy2;

		r = (float)(rand() % 256) / 256;
		g = (float)(rand() % 256) / 256;
		b = (float)(rand() % 256) / 256;
	}
}rect;

void main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("CG_4w_opengl_3");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		exit(EXIT_FAILURE);
	}
	else
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}


GLvoid drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (!rectangles.empty()) {
		for (int i = 0; i < rectangles.size(); i++) {
			draw_rect(i);
		}
	}
	glutSwapBuffers();
}


GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	struct rect rect_new;
	switch (key) {
	case 'a':
		rectangles.push_back(rect_new);
		break;
	case 'q':
		glutLeaveMainLoop();
		break;

	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	GLfloat input_pos[2] = { x, y };
	clamp_pos(input_pos);
	if (state == GLUT_DOWN) {
		select_index = rect_find_top(input_pos);
		glutPostRedisplay();
	}
	else {
		rect_merge();
		select_index = -1;
	}

}

void Motion(int x, int y)
{
	if (select_index == -1) return;
	GLfloat rect_width = rectangles[select_index].x2 - rectangles[select_index].x1;
	GLfloat rect_height = rectangles[select_index].y2 - rectangles[select_index].y1;
	GLfloat input_pos[2] = { x, y };
	clamp_pos(input_pos);
	rectangles[select_index].x1 = input_pos[0] - rect_width / 2;
	rectangles[select_index].x2 = input_pos[0] + rect_width / 2;
	rectangles[select_index].y1 = input_pos[1] - rect_height / 2;
	rectangles[select_index].y2 = input_pos[1] + rect_height / 2;

	
	glutPostRedisplay();
}


void clamp_pos(GLfloat* input_pos) {
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int viewport_width = viewport[2];
	int viewport_height = viewport[3];
	input_pos[0] = (input_pos[0] / viewport_width) * 2 - 1.0f;
	input_pos[1] = -1 * ((input_pos[1] / viewport_height) * 2 - 1.0f);
}

void draw_rect(int index) {
	glColor3f(rectangles[index].r, rectangles[index].g, rectangles[index].b);
	glRectf(rectangles[index].x1, rectangles[index].y1, rectangles[index].x2, rectangles[index].y2);
}

int rect_find_top(GLfloat* input_pos) {
	if (rectangles.empty()) return -1;
	for (int i = rectangles.size() - 1; i >= 0; i--) {
		if (rectangles[i].x1 <= input_pos[0] && input_pos[0] <= rectangles[i].x2) {
			if (rectangles[i].y1 <= input_pos[1] && input_pos[1] <= rectangles[i].y2) {
				if(select_index != i) return i;
				
			}
		}
	}
}

int rect_find_overlap() {

	if (rectangles.empty()) return -1;
	for (int i = rectangles.size() - 1; i >= 0; i--) {
		if (i == select_index) continue;
		if ((rectangles[i].x1 <= rectangles[select_index].x1 && rectangles[select_index].x1 <= rectangles[i].x2)||
			(rectangles[i].x1 >= rectangles[select_index].x1 && rectangles[select_index].x2 >= rectangles[i].x1)) {

			if ((rectangles[i].y1 <= rectangles[select_index].y1 && rectangles[select_index].y1 <= rectangles[i].y2) ||
				(rectangles[i].y1 >= rectangles[select_index].y1 && rectangles[select_index].y2 >= rectangles[i].y1)) {

				
				return i;
			}
		}
	}

	return -1;
	//if (rectangles.empty()) return -1;
	//int ret_index = -1;
	//GLfloat vert[2];
	//GLfloat rect_width = rectangles[select_index].x2 - rectangles[select_index].x1;
	//GLfloat rect_height = rectangles[select_index].y2 - rectangles[select_index].y1;
	//GLfloat vert_Mid[2] = { rectangles[select_index].x1 + rect_width/2, rectangles[select_index].y1 + rect_height/2};


	//
	//for (int i = 0; i < 4; i++) {
	//	vert[0] = vert_Mid[0] + (rect_width/2 * dx[i]);
	//	vert[1] = vert_Mid[1] + (rect_height/2 * dy[i]);
	//	int cmp_index = rect_find_top(vert);
	//	//if (cmp_index == select_index) continue;
	//	ret_index = ret_index < cmp_index ? cmp_index : ret_index;
	//}

	//return ret_index;
}

void rect_merge() {

	//std::cout << rectangles.size() << std::endl;
	if (select_index == -1) return;
	int merge_index = -1;

	merge_index = rect_find_overlap();

	std::cout << merge_index << " ";
	std::cout << select_index << std::endl;

	if (merge_index != -1) {
		GLfloat ix1 = rectangles[select_index].x1 < rectangles[merge_index].x1 ? rectangles[select_index].x1 : rectangles[merge_index].x1;
		GLfloat iy1 = rectangles[select_index].y1 < rectangles[merge_index].y1 ? rectangles[select_index].y1 : rectangles[merge_index].y1;
		GLfloat ix2 = rectangles[select_index].x2 > rectangles[merge_index].x2 ? rectangles[select_index].x2 : rectangles[merge_index].x2;
		GLfloat iy2 = rectangles[select_index].y2 > rectangles[merge_index].y2 ? rectangles[select_index].y2 : rectangles[merge_index].y2;

		//std::cout << rectangles[select_index].x1 << std::endl;

		
		struct rect rect_new(ix1, iy1, ix2, iy2);
		rectangles.push_back(rect_new);
		
		rectangles.erase(rectangles.begin() + merge_index);
		if(merge_index < select_index) rectangles.erase(rectangles.begin() + select_index - 1);
		else rectangles.erase(rectangles.begin() + select_index);
		
	}
}