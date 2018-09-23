# -*- coding: utf-8 -*-
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import sys

ESCAPE = '\033'
window = 0

#Угол вращения для треугольника и квадрата
rtri = 0.0
rquad = 0.0

#Инициализация OpenGl
def InitGL(Width, Height):
    glClearColor(0.0, 0.0, 0.0, 0.0)
    glClearDepth(1.0)
    glDepthFunc(GL_LESS)
    glEnable(GL_DEPTH_TEST)
    glShadeModel(GL_SMOOTH)

    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(45.0, float(Width) / float(Height), 0.1, 100.0)

    glMatrixMode(GL_MODELVIEW)


def ReSizeGLScene(Width, Height):
    if Height == 0:
        Height = 1

    glViewport(0, 0, Width, Height)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(45.0, float(Width) / float(Height), 0.1, 100.0)
    glMatrixMode(GL_MODELVIEW)



def DrawGLScene():
    global rtri, rquad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()
    glTranslatef(-1.5, 0.0, -6.0)

    # Рисуем треугольник
    glRotatef(rtri, 0.0, 1.0, 0.0)
    glBegin(GL_POLYGON)
    glColor3f(1.0, 0.0, 0.0)
    glVertex3f(0.0, 1.0, 0.0)
    glColor3f(0.0, 1.0, 0.0)
    glVertex3f(1.0, -1.0, 0.0)
    glColor3f(0.0, 0.0, 1.0)
    glVertex3f(-1.0, -1.0, 0.0)
    glEnd()
    #Заканчиваем "вращение" по оси оу относительно треугольника
    glLoadIdentity()

    glTranslatef(1.5, 0.0, -6.0)

    # Рисуем квадрат
    glRotatef(rquad, 1.0, 0.0, 0.0)
    glBegin(GL_QUADS)
    glColor3f(0.3, 0.5, 1.0)
    glVertex3f(-1.0, 1.0, 0.0)
    glVertex3f(1.0, 1.0, 0.0)
    glVertex3f(1.0, -1.0, 0.0)
    glVertex3f(-1.0, -1.0, 0.0)
    glEnd()
    rtri = rtri -0.1
    rquad = rquad - 0.1
    # Отображаем
    glutSwapBuffers()


# Выход по нажатию любой клавиши
def keyPressed(*args):
    # If escape is pressed, kill everything.
    if args[0] == ESCAPE:
        glutDestroyWindow(window)
        sys.exit()


def main():
    global window
    glutInit(())

    # Выбор отображения:
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH)

    glutInitWindowSize(640, 480)

    glutInitWindowPosition(0, 0)


    window = glutCreateWindow("Lesson 4")


    glutDisplayFunc(DrawGLScene)

    # Для полного экрана
    # glutFullScreen()


    glutIdleFunc(DrawGLScene)
    glutReshapeFunc(ReSizeGLScene)
    glutKeyboardFunc(keyPressed)

    # Инициализация
    InitGL(640, 480)

    # Запуск
    glutMainLoop()


main()