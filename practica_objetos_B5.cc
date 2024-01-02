//**************************************************************************
// Práctica 3
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B5.h"
#include <unistd.h>

using namespace std;

// tipos
typedef enum
{
    CUBO,
    PIRAMIDE,
    OBJETO_PLY,
    PEON_PLY,
    ROTACION,
    CILINDRO,
    CONO,
    ESFERA,
    EXTRUSION,
    MONTANA,
    EXCAVADORA,
    WALL_E
} _tipo_objeto;
_tipo_objeto t_objeto = WALL_E;
_modo modo = SOLID_COLORS;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x, Size_y, Front_plane, Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x = 50, Window_y = 50, Window_width = 650, Window_high = 650;

// objetos
_cubo cubo;
_piramide piramide(0.85, 1.3);
_objeto_ply ply;
_rotacion_PLY peon_ply;
_rotacion rotacion;
_cilindro cilindro(1, 2, 6);
_cono cono(1, 2, 6);
_esfera esfera(1, 6, 6);
_excavadora excavadora;
_extrusion *extrusion;
_montana montana(6, 0.9, 0.8);

_wall_e wall_e;

int estadoRaton, xc, yc; // Práctica 5
int cambio = 0, ancho, alto;

float izquierda = -5.0,
      derecha = 5.0,
      abajo = -5.0,
      arriba = 5.0,
      factor = 1;

float giro1 = 0.0, giro2 = 0.0, giro3 = 0.0;
int pulsar = 0.0;
int paso = 0;

//**************************************************************************
//
//***************************************************************************

void clean_window()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void perspectiva()
{

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, ancho, alto);

    // formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
    //  plano_delantero>0  plano_trasero>PlanoDelantero)
    glFrustum(-Size_x, Size_x, -Size_y, Size_y, Front_plane, Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{
    // posicion del observador
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -Observer_distance);
    glRotatef(Observer_angle_x, 1, 0, 0);
    glRotatef(Observer_angle_y, 0, 1, 0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{

    glDisable(GL_LIGHTING);
    glLineWidth(2);
    glBegin(GL_LINES);
    // eje X, color rojo
    glColor3f(1, 0, 0);
    glVertex3f(-AXIS_SIZE, 0, 0);
    glVertex3f(AXIS_SIZE, 0, 0);
    // eje Y, color verde
    glColor3f(0, 1, 0);
    glVertex3f(0, -AXIS_SIZE, 0);
    glVertex3f(0, AXIS_SIZE, 0);
    // eje Z, color azul
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, -AXIS_SIZE);
    glVertex3f(0, 0, AXIS_SIZE);
    glEnd();
}

//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects()
{

    switch (t_objeto)
    {
    case CUBO:
        cubo.draw(modo, 1.0, 0.0, 0.0, 5);
        break;
    case PIRAMIDE:
        piramide.draw(modo, 1.0, 0.0, 0.0, 5);
        break;
    case OBJETO_PLY:
        ply.draw(modo, 1.0, 0.6, 0.0, 3);
        break;
    case ROTACION:
        rotacion.draw(modo, 1.0, 0.0, 0.0, 5);
        break;
    case PEON_PLY:
        peon_ply.draw(modo, 1.0, 0.0, 0.0, 5);
        break;
    case CILINDRO:
        cilindro.draw(modo, 1.0, 0.0, 0.0, 5);
        break;
    case CONO:
        cono.draw(modo, 1.0, 0.0, 0.0, 5);
        break;
    case ESFERA:
        esfera.draw(modo, 1.0, 0.0, 0.0, 5);
        break;
    case EXCAVADORA:
        excavadora.draw(modo, 1.0, 0.8, 0.2, 5);
        break;
    case EXTRUSION:
        extrusion->draw(modo, 1.0, 0.8, 0.2, 5);
        break;
    case MONTANA:
        montana.draw(modo, 0.2, 0.7, 0.0, 1);
        break;
    case WALL_E:
        wall_e.draw(modo, 0.2, 0.3, 0.1, 5);
        break;
    }
}

//**************************************************************************
//  luces
//***************************************************************************

float giro = 0.0;

void luces()
{

    // Luz Puntual
    GLfloat luz_ambiental[] = {0.4532, 0.2435, 0.865, 1.0},
            luz_difusa[] = {0.655, 0.243, 0.6354, 1.0},
            luz_especular[] = {0.3245, 0.76545, 0.325, 1.0},
            luz_posicion[] = {0.0, 2.0, 10.0, 0.0};

    // Luz Direccional
    GLfloat luz_ambiental2[] = {1.0, 1.0, 1.0, 1.0},
            luz_difusa2[] = {1.0, 1.0, 1.0, 1.0},
            luz_especular2[] = {1.0, 1.0, 1.0, 1.0},
            luz_posicion2[] = {0.0, 2.0, 10.0, 0.0};

    glLightfv(GL_LIGHT1, GL_AMBIENT, luz_ambiental);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_difusa);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luz_especular);
    glLightfv(GL_LIGHT1, GL_POSITION, luz_posicion);

    glLightfv(GL_LIGHT2, GL_AMBIENT, luz_ambiental2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, luz_difusa2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, luz_especular2);

    glPushMatrix();
    glRotatef(giro, 0, 1, 0);
    glLightfv(GL_LIGHT2, GL_POSITION, luz_posicion2);
    glPopMatrix();

    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
}

//**************************************************************************

void vistas_multiples()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, alto / 2, ancho / 2, alto / 2);
    glOrtho(izquierda * factor, derecha * factor, abajo * factor, arriba * factor, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw_axis();
    draw_objects();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, ancho / 2, alto / 2);
    glOrtho(izquierda * factor, derecha * factor, abajo * factor, arriba * factor, -100, 100);
    glRotatef(90, 1, 0, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw_axis();
    draw_objects();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(ancho / 2, alto / 2, ancho / 2, alto / 2);
    glOrtho(izquierda * factor, derecha * factor, abajo * factor, arriba * factor, -100, 100);
    glRotatef(90, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw_axis();
    draw_objects();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(ancho / 2, 0, ancho / 2, alto / 2);
    glOrtho(izquierda * factor, derecha * factor, abajo * factor, arriba * factor, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    change_observer();

    draw_axis();
    draw_objects();
}

void vistas_multiples_seleccion()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, alto / 2, ancho / 2, alto / 2);
    glOrtho(izquierda * factor, derecha * factor, abajo * factor, arriba * factor, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    wall_e.seleccion();
    draw_axis();
    draw_objects();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, ancho / 2, alto / 2);
    glOrtho(izquierda * factor, derecha * factor, abajo * factor, arriba * factor, -100, 100);
    glRotatef(90, 1, 0, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    wall_e.seleccion();
    draw_axis();
    draw_objects();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(ancho / 2, alto / 2, ancho / 2, alto / 2);
    glOrtho(izquierda * factor, derecha * factor, abajo * factor, arriba * factor, -100, 100);
    glRotatef(90, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    wall_e.seleccion();
    draw_axis();
    draw_objects();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(ancho / 2, 0, ancho / 2, alto / 2);
    glOrtho(izquierda * factor, derecha * factor, abajo * factor, arriba * factor, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    change_observer();
    wall_e.seleccion();
    draw_axis();
    draw_objects();
}

//********************************************

void draw(void)
{
    clean_window();
    if (cambio == 0)
    {
        perspectiva();
        change_observer();
        draw_axis();
        draw_objects();
    }
    else
        vistas_multiples();

    luces();
    glutSwapBuffers();

    // if (t_objeto == EXCAVADORA)
    // {
    //     glDrawBuffer(GL_BACK);
    //     clean_window();
    //     if (cambio == 0)
    //     {
    //         perspectiva();
    //         change_observer();
    //         excavadora.seleccion();
    //     }
    //     else
    //         vistas_multiples_seleccion();
    // }

    if (t_objeto == WALL_E)
    {
        glDrawBuffer(GL_BACK);
        clean_window();
        if (cambio == 0)
        {
            perspectiva();
            change_observer();
            wall_e.seleccion();
        }
        else
            vistas_multiples_seleccion();
    }

    glFlush();
}

//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1, int Alto1)
{
    float Aspect_ratio;

    Aspect_ratio = (float)Alto1 / (float)Ancho1;
    Size_y = Size_x * Aspect_ratio;
    perspectiva();
    glViewport(0, 0, Ancho1, Alto1);
    ancho = Ancho1;
    alto = Alto1;
    glutPostRedisplay();
}

//***************************************************************************
// Funcion encargada de la animacion del objeto jerarquico WALL_E
//***************************************************************************

bool animation = false;
bool animacion_A, animacion_B, animacion_compuerta, wall_e_animated, segundaParte_animacion, terceraParte_animacion, cuartaParte_animacion, quintaParte_animacion, sextaParte_animacion;
bool primeraParte_animacion = true;

void animacion_wall_e()
{
    if (primeraParte_animacion)
    {
        if (Observer_angle_y != 360)
            Observer_angle_y++;
        else
        {
            primeraParte_animacion = false;
            segundaParte_animacion = true;
        }
    }
    else if (segundaParte_animacion)
    {
        if (!animacion_B)
        {
            if (!animacion_A)
            {
                wall_e.giro_ojos = (wall_e.giro_ojos < 20) ? wall_e.giro_ojos + 0.5 : wall_e.giro_ojos;
                animacion_A = (wall_e.giro_ojos == 20) ? true : animacion_A;
                if (wall_e.giro_ojos == 0)
                    animacion_B = true;
            }
            else
            {
                wall_e.giro_ojos = (wall_e.giro_ojos > -30) ? wall_e.giro_ojos - 0.5 : wall_e.giro_ojos;
                animacion_A = (wall_e.giro_ojos == -30) ? false : animacion_A;
            }
        }
        else
        {
            if (!animacion_A)
            {
                wall_e.giro_cuello = (wall_e.giro_cuello < 90) ? wall_e.giro_cuello + 1 : wall_e.giro_cuello;
                animacion_A = (wall_e.giro_cuello == 90) ? true : animacion_A;
                if (wall_e.giro_cuello == 0)
                {
                    segundaParte_animacion = false;
                    terceraParte_animacion = true;
                }
            }
            else
            {
                wall_e.giro_cuello = (wall_e.giro_cuello > -90) ? wall_e.giro_cuello - 1 : wall_e.giro_cuello;
                animacion_A = (wall_e.giro_cuello == -90) ? false : animacion_A;
            }
        }
    }
    else if (terceraParte_animacion)
    {
        wall_e.cuerpo.giro_compuerta += animacion_compuerta ? -0.5 : 0.5;
        animacion_compuerta = (wall_e.cuerpo.giro_compuerta == 90) ? true : animacion_compuerta;
        terceraParte_animacion = !(wall_e.cuerpo.giro_compuerta == 0 && animacion_compuerta);
        cuartaParte_animacion = !terceraParte_animacion;
    }
    else if (cuartaParte_animacion)
    {
        if (animacion_B)
        {
            if (!animacion_A)
            {
                wall_e.movimiento_brazos = (wall_e.movimiento_brazos < 1.3) ? wall_e.movimiento_brazos + 0.05 : wall_e.movimiento_brazos;
                animacion_A = (fabs(wall_e.movimiento_brazos - 1.3) < 0.0001) ? true : animacion_A;
            }
            else
            {
                wall_e.movimiento_brazos = (wall_e.movimiento_brazos > 0.0) ? wall_e.movimiento_brazos - 0.05 : wall_e.movimiento_brazos;
                if (fabs(wall_e.movimiento_brazos - 0.0) < 0.0001)
                {
                    animacion_A = false;
                    animacion_B = false;
                }
            }
        }
        else
        {
            if (!animacion_A)
            {
                wall_e.brazos.brazo1.giro_brazo = (wall_e.brazos.brazo1.giro_brazo > -25) ? wall_e.brazos.brazo1.giro_brazo - 0.5 : wall_e.brazos.brazo1.giro_brazo;
                wall_e.brazos.brazo2.giro_brazo = (wall_e.brazos.brazo2.giro_brazo < 25) ? wall_e.brazos.brazo2.giro_brazo + 0.5 : wall_e.brazos.brazo2.giro_brazo;
                animacion_A = (wall_e.brazos.brazo1.giro_brazo == -25) ? true : animacion_A;
                if (wall_e.brazos.brazo1.giro_brazo == 0)
                {
                    cuartaParte_animacion = false;
                    quintaParte_animacion = true;
                }
            }
            else
            {
                wall_e.brazos.brazo1.giro_brazo = (wall_e.brazos.brazo1.giro_brazo < 25) ? wall_e.brazos.brazo1.giro_brazo + 0.5 : wall_e.brazos.brazo1.giro_brazo;
                wall_e.brazos.brazo2.giro_brazo = (wall_e.brazos.brazo2.giro_brazo > -25) ? wall_e.brazos.brazo2.giro_brazo - 0.5 : wall_e.brazos.brazo2.giro_brazo;
                animacion_A = (wall_e.brazos.brazo1.giro_brazo == 25) ? false : animacion_A;
            }
        }
    }
    else if (quintaParte_animacion)
    {
        if (!wall_e_animated)
        {
            sleep(1);
            Observer_angle_x = 0;
            Observer_angle_y = -90;
            Observer_distance = 34.8;
            wall_e.pos_wall_e = -12;
            wall_e.giro_cuerpo = 15;
            wall_e.giro_cuello = 60;
            wall_e_animated = true;
        }
        else
        {
            if (wall_e.pos_wall_e != 12)
            {
                wall_e.pos_wall_e += 0.125;
                wall_e.ruedas.giro_ruedas++;
            }
        }
        if (wall_e.pos_wall_e == 12)
        {
            quintaParte_animacion = false;
            sextaParte_animacion = true;
            sleep(1);
        }
    }
    else if (sextaParte_animacion)
    {
        Observer_distance = 10.5;
        Observer_angle_x = 9;
        Observer_angle_y = 0;
        wall_e.pos_wall_e = 0;
        wall_e.giro_cuerpo = 0;
        wall_e.giro_cuello = 0;
        quintaParte_animacion = false;
    }

    glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char Tecla1, int x, int y)
{
    switch (toupper(Tecla1))
    {
    case 'Q':
        exit(0);
    case '1':
        modo = POINTS;
        break;
    case '2':
        modo = EDGES;
        break;
    case '3':
        modo = SOLID;
        break;
    case '4':
        modo = SOLID_COLORS;
        break;
    case '5':
        modo = SOLID_COLORS_GOURAUD;
        break;
    case '6':
        modo = SOLID_PHONG_FLAT;
        break;
    case '7':
        modo = SOLID_PHONG_GOURAUD;
        break;
    case 'P':
        t_objeto = PIRAMIDE;
        break;
    case 'C':
        t_objeto = CUBO;
        break;
    case 'O':
        t_objeto = OBJETO_PLY;
        break;
    case 'R':
        t_objeto = ROTACION;
        break;
    case 'I':
        t_objeto = PEON_PLY;
        break;
    case 'L':
        t_objeto = CILINDRO;
        break;
    case 'N':
        t_objeto = CONO;
        break;
    case 'E':
        t_objeto = ESFERA;
        break;
    case 'A':
        t_objeto = EXCAVADORA;
        break;
    case 'X':
        t_objeto = EXTRUSION;
        break;
    case 'M':
        t_objeto = MONTANA;
        break;
    case 'W':
        t_objeto = WALL_E;
        break;
    case '+':
        wall_e.pos_wall_e += 0.2;
        wall_e.ruedas.giro_ruedas += 2;
        break;
    case '-':
        wall_e.pos_wall_e -= 0.2;
        wall_e.ruedas.giro_ruedas -= 2;
        break;
    case '9':
        if (wall_e.movimiento_brazos < 1.3)
            wall_e.movimiento_brazos += 0.1;
        break;
    case '0':
        if (wall_e.movimiento_brazos > 0)
            wall_e.movimiento_brazos -= 0.1;
        break;
    case '8':
        wall_e.ruedas.giro_ruedas++;
        break;
    case 'D':
        if (!animation)
        {
            animation = true;
            glutIdleFunc(animacion_wall_e);
        }
        else
        {
            animation = false;
            glutIdleFunc(NULL);
        }
        break;
    case 'S':
        if (pulsar == 0)
        {
            giro1 = 1.0;
            giro2 = 1.0;
            giro3 = 0.25;
            pulsar = 1;
        }
        else
        {
            giro1 = 0.0;
            giro2 = 0.0;
            giro3 = 0.0;
            pulsar = 0;
        }
        break;
    case 'T':
        giro++;
        break;
    case ',':
        cambio = 1;
        break;
    case '.':
        cambio = 0;
        break;
    }
    glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int Tecla1, int x, int y)
{

    switch (Tecla1)
    {
    case GLUT_KEY_LEFT:
        Observer_angle_y--;
        break;
    case GLUT_KEY_RIGHT:
        Observer_angle_y++;
        break;
    case GLUT_KEY_UP:
        Observer_angle_x--;
        break;
    case GLUT_KEY_DOWN:
        Observer_angle_x++;
        break;
    case GLUT_KEY_PAGE_UP:
        Observer_distance *= 1.2;
        break;
    case GLUT_KEY_PAGE_DOWN:
        Observer_distance /= 1.2;
        break;

    case GLUT_KEY_F1:
        excavadora.giro_cabina += 5;
        break;
    case GLUT_KEY_F2:
        excavadora.giro_cabina -= 5;
        break;
    case GLUT_KEY_F3:
        excavadora.giro_primer_brazo += 1;
        if (excavadora.giro_primer_brazo > excavadora.giro_primer_brazo_max)
            excavadora.giro_primer_brazo = excavadora.giro_primer_brazo_max;
        break;
    case GLUT_KEY_F4:
        excavadora.giro_primer_brazo -= 1;
        if (excavadora.giro_primer_brazo < excavadora.giro_primer_brazo_min)
            excavadora.giro_primer_brazo = excavadora.giro_primer_brazo_min;
        break;
    case GLUT_KEY_F5:
        excavadora.giro_segundo_brazo += 1;
        if (excavadora.giro_segundo_brazo > excavadora.giro_segundo_brazo_max)
            excavadora.giro_segundo_brazo = excavadora.giro_segundo_brazo_max;
        break;
    case GLUT_KEY_F6:
        excavadora.giro_segundo_brazo -= 1;
        if (excavadora.giro_segundo_brazo < excavadora.giro_segundo_brazo_min)
            excavadora.giro_segundo_brazo = excavadora.giro_segundo_brazo_min;
        break;
    case GLUT_KEY_F7:
        excavadora.giro_pala += 1;
        if (excavadora.giro_pala > excavadora.giro_pala_max)
            excavadora.giro_pala = excavadora.giro_pala_max;
        break;
    case GLUT_KEY_F8:
        excavadora.giro_pala -= 1;
        if (excavadora.giro_pala < excavadora.giro_pala_min)
            excavadora.giro_pala = excavadora.giro_pala_min;
        break;
    }
    glutPostRedisplay();
}

void special_key_wall_e(int Tecla1, int x, int y)
{
    switch (Tecla1)
    {
    case GLUT_KEY_LEFT:
        Observer_angle_y--;
        break;
    case GLUT_KEY_RIGHT:
        Observer_angle_y++;
        break;
    case GLUT_KEY_UP:
        Observer_angle_x--;
        break;
    case GLUT_KEY_DOWN:
        Observer_angle_x++;
        break;
    case GLUT_KEY_PAGE_UP:
        Observer_distance *= 1.2;
        break;
    case GLUT_KEY_PAGE_DOWN:
        Observer_distance /= 1.2;
        break;
    case GLUT_KEY_F1:
        if (wall_e.giro_ojos > -20)
            wall_e.giro_ojos -= 0.5;
        break;
    case GLUT_KEY_F2:
        if (wall_e.giro_ojos < 20)
            wall_e.giro_ojos += 0.5;
        break;
    case GLUT_KEY_F3:
        if (wall_e.giro_cuello < 90)
            wall_e.giro_cuello += 5;
        break;
    case GLUT_KEY_F4:
        if (wall_e.giro_cuello > -90)
            wall_e.giro_cuello -= 5;
        break;
    case GLUT_KEY_F5:
        if (wall_e.brazos.brazo1.giro_brazo < 25)
            wall_e.brazos.brazo1.giro_brazo++;
        break;
    case GLUT_KEY_F6:
        if (wall_e.brazos.brazo1.giro_brazo > -25)
            wall_e.brazos.brazo1.giro_brazo--;
        break;
    case GLUT_KEY_F7:
        if (wall_e.brazos.brazo2.giro_brazo > -25)
            wall_e.brazos.brazo2.giro_brazo--;
        break;
    case GLUT_KEY_F8:
        if (wall_e.brazos.brazo2.giro_brazo < 25)
            wall_e.brazos.brazo2.giro_brazo++;
        break;
    case GLUT_KEY_F9:
        if (wall_e.cuerpo.giro_compuerta < 90.0)
            wall_e.cuerpo.giro_compuerta += 1.0;
        break;
    case GLUT_KEY_F10:
        if (wall_e.cuerpo.giro_compuerta > 0.0)
            wall_e.cuerpo.giro_compuerta -= 1.0;
        break;
    case GLUT_KEY_F11:
        if (wall_e.giro_cuerpo < 15)
            wall_e.giro_cuerpo += 0.5;
        break;
    case GLUT_KEY_F12:
        if (wall_e.giro_cuerpo > 0)
            wall_e.giro_cuerpo -= 0.5;
        break;
    }
    glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se selecciona una opción del menú
//
// el evento manda a la funcion:
// entero que representa una entrada del menú
//***************************************************************************

void menu(int key)
{

    if (key == 0)
    {
        excavadora.giro_pala += 4;
        if (excavadora.giro_pala > excavadora.giro_pala_max)
            excavadora.giro_pala = excavadora.giro_pala_max;
    }
    if (key == 1)
    {
        excavadora.giro_pala -= 4;
        if (excavadora.giro_pala < excavadora.giro_pala_min)
            excavadora.giro_pala = excavadora.giro_pala_min;
    }
    if (key == 2)
    {
        giro1 = 1.0;
        giro2 = 1.0;
        giro3 = 0.25;
    }
    if (key == 3)
    {
        giro1 = 0.0;
        giro2 = 0.0;
        giro3 = 0.0;
    }
}

//***************************************************************************
// Funcion de animación automática
//***************************************************************************

void animacion()
{
    switch (paso)
    {
    case 0:
        excavadora.giro_cabina -= giro1;
        if (excavadora.giro_cabina < -45)
            paso = 1;
        break;
    case 1:
        excavadora.giro_primer_brazo -= giro3;
        if (excavadora.giro_primer_brazo < -80)
            paso = 2;
        break;
    case 2:
        excavadora.giro_pala -= giro2;
        if (excavadora.giro_pala < excavadora.giro_pala_min)
        {
            excavadora.giro_pala = excavadora.giro_pala_min;
            paso = 0;
            excavadora.giro_cabina = 0.0;
            excavadora.giro_primer_brazo = 0.0;
            excavadora.giro_pala = 0.0;
        }
        break;
    }

    glutPostRedisplay();
}

//***************************************************************************
// Funcion de incializacion
//***************************************************************************

//***************************************************************************
// Funciones para la selección por color
//***************************************************************************

void procesar_color(unsigned char color[3])
{
    int i;
    if (t_objeto == EXCAVADORA)
    {
        for (i = 0; i < excavadora.piezas; i++)
        {
            if (color[0] == excavadora.color_select[i].r &&
                color[1] == excavadora.color_select[i].g &&
                color[2] == excavadora.color_select[i].r)
            {
                if (excavadora.activo[i] == 0)
                {
                    excavadora.activo[i] = 1;
                }
                else
                {
                    excavadora.activo[i] = 0;
                }
                glutPostRedisplay();
            }
        }
    }

    else if (t_objeto == WALL_E)
    {
        for (i = 0; i < wall_e.piezas; i++)
        {
            if (color[0] == wall_e.color_select[i].r &&
                color[1] == wall_e.color_select[i].g &&
                color[2] == wall_e.color_select[i].r)
            {
                if (wall_e.activo[i] == 0)
                {
                    wall_e.activo[i] = 1;
                }
                else
                {
                    wall_e.activo[i] = 0;
                }
                glutPostRedisplay();
            }
        }
    }
}

//***********************************************************

void pick_color(int x, int y)
{
    unsigned char pixel[3];

    glReadBuffer(GL_BACK);
    glReadPixels(x, alto - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte *)&pixel[0]);
    printf(" valor x %d, valor y %d, color %d, %d, %d \n", x, y, pixel[0], pixel[1], pixel[2]);

    procesar_color(pixel);
}

//***************************************************************************
// Funciones para manejo de eventos del ratón
//***************************************************************************

void clickRaton(int boton, int estado, int x, int y)
{
    if (boton == GLUT_RIGHT_BUTTON)
    {
        if (estado == GLUT_DOWN)
        {
            estadoRaton = 1;
            xc = x;
            yc = y;
        }
        else
            estadoRaton = 0;
    }
    if (boton == GLUT_LEFT_BUTTON)
    {
        if (estado == GLUT_DOWN)
        {
            estadoRaton = 2;
            xc = x;
            yc = y;
            pick_color(xc, yc);
        }
    }
    if (boton == 3)
    {
        factor *= 0.9;
        glutPostRedisplay();
    }
    if (boton == 4)
    {
        factor *= 1.1;
        glutPostRedisplay();
    }
}

//*************************************************************************/

void RatonMovido(int x, int y)
{
    if (estadoRaton == 1)
    {
        Observer_angle_y = Observer_angle_y - (x - xc);
        Observer_angle_x = Observer_angle_x + (y - yc);
        xc = x;
        yc = y;
        glutPostRedisplay();
    }
}

void initialize(void)
{
    // se inicalizan la ventana y los planos de corte
    Size_x = 0.5;
    Size_y = 0.5;
    Front_plane = 1;
    Back_plane = 1000;

    // se incia la posicion del observador, en el eje z
    Observer_distance = 11.5 * Front_plane;
    Observer_angle_x = 0;
    Observer_angle_y = 0;

    // se indica el color para limpiar la ventana	(r,v,a,al)
    // blanco=(1,1,1,1) rojo=(1,0,0,1), ...
    glClearColor(0.5098, 0.6313, 0.3843, 1);

    // se habilita el z-bufer
    glEnable(GL_DEPTH_TEST);
    perspectiva();
    glViewport(0, 0, Window_width, Window_high);
}

//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************

int main(int argc, char *argv[])
{
    // perfil

    vector<_vertex3f> perfil, poligono;
    _vertex3f aux;

    aux.x = 1.0;
    aux.y = -1.0;
    aux.z = 0.0;
    perfil.push_back(aux);
    aux.x = 1.5;
    aux.y = 0.0;
    aux.z = 0.0;
    perfil.push_back(aux);
    aux.x = 1.0;
    aux.y = 1.0;
    aux.z = 0.0;
    perfil.push_back(aux);
    aux.x = 1.5;
    aux.y = 1.2;
    aux.z = 0.0;
    perfil.push_back(aux);

    rotacion.parametros(perfil, 6, 1, 1, 0);

    aux.x = 1.0;
    aux.y = 0.0;
    aux.z = 1.0;
    poligono.push_back(aux);
    aux.x = 1.0;
    aux.y = 0.0;
    aux.z = -1.0;
    poligono.push_back(aux);
    aux.x = -1.2;
    aux.y = 0.0;
    aux.z = -1.0;
    poligono.push_back(aux);
    aux.x = -0.8;
    aux.y = 0.0;
    aux.z = 0.0;
    poligono.push_back(aux);
    aux.x = -1.2;
    aux.y = 0.0;
    aux.z = 1.0;
    poligono.push_back(aux);

    extrusion = new _extrusion(poligono, 0.25, 1.0, 0.25);

    // se llama a la inicialización de glut
    glutInit(&argc, argv);

    // se indica las caracteristicas que se desean para la visualización con OpenGL
    // Las posibilidades son:
    // GLUT_SIMPLE -> memoria de imagen simple
    // GLUT_DOUBLE -> memoria de imagen doble
    // GLUT_INDEX -> memoria de imagen con color indizado
    // GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
    // GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
    // GLUT_DEPTH -> memoria de profundidad o z-bufer
    // GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    // posicion de la esquina inferior izquierdad de la ventana
    glutInitWindowPosition(Window_x, Window_y);

    // tamaño de la ventana (ancho y alto)
    glutInitWindowSize(Window_width, Window_high);

    // llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
    // al bucle de eventos)
    glutCreateWindow("PRACTICA - 5");

    // asignación de la funcion llamada "dibujar" al evento de dibujo
    glutDisplayFunc(draw);
    // asignación de la funcion llamada "change_window_size" al evento correspondiente
    glutReshapeFunc(change_window_size);
    // asignación de la funcion llamada "normal_key" al evento correspondiente
    glutKeyboardFunc(normal_key);
    // asignación de la funcion llamada "tecla_Especial" al evento correspondiente
    glutSpecialFunc(special_key_wall_e);

    // Creamos menú
    // glutCreateMenu(menu);
    // glutAddMenuEntry("Girar pala positivo", 0);
    // glutAddMenuEntry("Girar pala negativo", 1);
    // glutAddMenuEntry("Activar animación", 2);
    // glutAddMenuEntry("Desactivar animación", 3);
    // glutAttachMenu(GLUT_LEFT_BUTTON);

    glutIdleFunc(animacion);

    // funcion de inicialización
    initialize();

    // creación del objeto ply
    ply.parametros(argv[1]);
    peon_ply.parametros_PLY(argv[2], 30);

    // eventos ratón
    glutMouseFunc(clickRaton);
    glutMotionFunc(RatonMovido);

    // inicio del bucle de eventos
    glutMainLoop();
    return 0;
}
