# -*- coding: utf-8 -*-
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
from image import *
import sys

ESCAPE = '\033'
window = 0

#Угол вращения для куба
xrot = yrot = zrot = 0.0
texture = 0


def LoadTextures():
    # Выбираем картинку
    #image = open("Arve.bmp")

    ix = 64
    iy = 64
    #image = image.tostring("raw", "RGBX", 0, -1)

    # Создание тексстуры
    glGenTextures(1, texture)
    glBindTexture(GL_TEXTURE_2D, texture)

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
    glTexImage2D(GL_TEXTURE_2D, 0, 3, ix, iy, 0, GL_RGBA, GL_UNSIGNED_BYTE, None)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL)

#Инициализация OpenGl
def InitGL(Width, Height):
    LoadTextures()
    glEnable(GL_TEXTURE_2D)
    glClearColor(0.0, 1.0, 1.0, 1.0)
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
    global xrot, yrot, zrot, texture

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()
    glTranslatef(0.0, 0.0, -5.0)

    #перемещение по осям
    glRotatef(xrot, 1.0, 0.0, 0.0)
    glRotatef(yrot, 0.0, 1.0, 0.0)
    glRotatef(zrot, 0.0, 0.0, 1.0)


    # glBindTexture(GL_TEXTURE_2D,texture)	# Вращение по оси У

    glBegin(GL_QUADS)  # Start Drawing The Cube

    # Передняя сторона
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1.0, -1.0, 1.0)
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1.0, -1.0, 1.0)
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0)
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0)

    # Задняя сторона
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-1.0, -1.0, -1.0)
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0)
    glTexCoord2f(0.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0)
    glTexCoord2f(0.0, 0.0);
    glVertex3f(1.0, -1.0, -1.0)

    # Верхняя сторона
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0)
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1.0, 1.0, 1.0)
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1.0, 1.0, 1.0)
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0)

    # Нижняя сторона
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-1.0, -1.0, -1.0)
    glTexCoord2f(0.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0)
    glTexCoord2f(0.0, 0.0);
    glVertex3f(1.0, -1.0, 1.0)
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-1.0, -1.0, 1.0)

    # Правая сторона
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1.0, -1.0, -1.0)
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1.0, 1.0, -1.0)
    glTexCoord2f(0.0, 1.0);
    glVertex3f(1.0, 1.0, 1.0)
    glTexCoord2f(0.0, 0.0);
    glVertex3f(1.0, -1.0, 1.0)

    # Левая сторона
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1.0, -1.0, -1.0)
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-1.0, -1.0, 1.0)
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-1.0, 1.0, 1.0)
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0)

    glEnd();

    xrot = xrot + 0.1
    yrot = yrot + 0.1
    zrot = zrot + 0.1

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


    window = glutCreateWindow("Lesson 5")


    glutDisplayFunc(DrawGLScene)

    # Для полного экрана
    # glutFullScreen()


    glutIdleFunc(DrawGLScene)
    glutReshapeFunc(ReSizeGLScene)
    glutKeyboardFunc(keyPressed)

    # Инициализация окна
    InitGL(640, 480)

    # Запуск
    glutMainLoop()


main()