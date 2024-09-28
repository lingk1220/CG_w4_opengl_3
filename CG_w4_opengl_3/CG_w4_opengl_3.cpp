#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>
#include <math.h>
#define RECTINITSIZE 0.2


GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void draw_rect(int index);
void clamp_pos(GLfloat* input_pos);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

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

		r = (float)(rand() % viewport_width) / viewport_width;
		g = (float)(rand() % viewport_width) / viewport_width;
		b = (float)(rand() % viewport_width) / viewport_width;
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
	std::cout << "??\n";
	GLfloat input_pos[2] = { x, y };
	clamp_pos(input_pos);
	if (state == GLUT_DOWN) {
		if (rectangles.empty()) return;
		for (int i = rectangles.size() - 1; i >= 0; i--) {
			if (rectangles[i].x1 <= input_pos[0] && input_pos[0] <= rectangles[i].x2) {
				if (rectangles[i].y1 <= input_pos[1] && input_pos[1] <= rectangles[i].y2) {
					select_index = i;
					std::cout << select_index << std::endl;
					break;
				}
			}
		}
		glutPostRedisplay();
	}
	else {
		select_index = -1;
	}

}

void Motion(int x, int y)
{
	if (select_index == -1) return;
	GLfloat input_pos[2] = { x, y };
	clamp_pos(input_pos);
	rectangles[select_index].x1 = input_pos[0] - RECTINITSIZE / 2;
	rectangles[select_index].x2 = input_pos[0] + RECTINITSIZE / 2;
	rectangles[select_index].y1 = input_pos[1] - RECTINITSIZE / 2;
	rectangles[select_index].y2 = input_pos[1] + RECTINITSIZE / 2;


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