//**************************************************************************
// Práctica 2
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>

const float AXIS_SIZE = 5000;
typedef enum
{
       POINTS,
       EDGES,
       SOLID,
       SOLID_COLORS,
       SOLID_COLORS_GOURAUD,
       SOLID_PHONG_FLAT,
       SOLID_PHONG_GOURAUD
} _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:
       _puntos3D();
       void draw_puntos(float r, float g, float b, int grosor);

       vector<_vertex3f> vertices;
       vector<_vertex3f> colores_vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D : public _puntos3D
{
public:
       _triangulos3D();
       void draw_aristas(float r, float g, float b, int grosor);
       void draw_solido(float r, float g, float b);
       void draw_solido_colores();
       void draw_solido_colores_vertices();
       void draw_solid_phong_flat();
       void draw_solid_phong_gouraud();
       void draw(_modo modo, float r, float g, float b, float grosor);

       /* asignación de colores */
       void colors_random();
       void colors_chess(float r1, float g1, float b1, float r2, float g2, float b2);
       /**
        * 'r' -> rojo
        * 'b' -> azul
        * 'g' -> verde
        * 'w' -> grey
        */
       void draw_gama_color(char color = 'w');
       void draw_cuadrantes();
       void colors_diffuse_flat(float kr, float kg, float kb,
                                float lpx, float lpy, float lpz);

       void colors_diffuse_gouraud(float kr, float kg, float kb,
                                   float lpx, float lpy, float lpz);
       /* calcular normales */

       void calcular_normales_caras();
       void calcular_normales_vertices();

       vector<_vertex3i> caras;
       vector<_vertex3f> colores_caras;
       vector<_vertex3f> colores_vertices;
       vector<_vertex3f> normales_caras;
       vector<_vertex3f> normales_vertices;

       int calculadas_normales_caras;

       // material
       _vertex4f ambiente;
       _vertex4f difuso;
       _vertex4f especular;
       float brillo;
};

//*************************************************************************
// objetos o modelos
//*************************************************************************

//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo : public _triangulos3D
{
public:
       _cubo(float tam = 1);
       float tam_lado;
};

//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide : public _triangulos3D
{
public:
       _piramide(float tam = 0.5, float al = 1.0);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply : public _triangulos3D
{
public:
       _objeto_ply();

       void parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************

class _rotacion : public _triangulos3D
{
public:
       _rotacion();

       void parametros(vector<_vertex3f> perfil, int num,
                       int tapa_in, int tapa_su, int tipo);
       void calcular_normales_esfera();
};

// tapa_in=0 sin tapa, tapa_in=1 con tapa
// tapa_su=0 sin tapa, tapa_su=1 con tapa
// tipo=0 revolucion normal
// tipo=1 esfera
// tipo=2 cono

//************************************************************************
// objeto por extrusión
//************************************************************************

class _extrusion : public _triangulos3D
{
public:
       _extrusion(vector<_vertex3f> poligono, float x, float y, float z);
};

//************************************************************************
// objeto por revolución especiales
//************************************************************************

//************************************************************************
// cilindro
//************************************************************************

class _cilindro : public _rotacion
{
public:
       _cilindro(float radio = 1.0, float altura = 2.0, int num = 12);
};

//************************************************************************
// cono
//************************************************************************

class _cono : public _rotacion
{
public:
       _cono(float radio = 1.0, float altura = 2.0, int num = 12);
};

//************************************************************************
// esfera
//************************************************************************

class _esfera : public _rotacion
{
public:
       _esfera(float radio = 1.0, int latitud = 6, int longitud = 6);
};

//************************************************************************
// rotacion archivo PLY
//************************************************************************

class _rotacion_PLY : public _rotacion
{
public:
       _rotacion_PLY();
       void parametros_PLY(char *archivo, int num);
};

//************************************************************************
// objeto montaña fractal
//************************************************************************

class _montana : public _triangulos3D
{
public:
       _montana(int nivelmax, float sigma, float h);
};

// nivelmax, ha de ser par es el tamaño de la cuadrícula
// sigma, se relaciona con la altura máxima de la montaña
// h 0<=h<=1, dimensión de Hausdorff indica la forma del relieve, más suave 1

//************************************************************************
// práctica 3, objeto jerárquico articulado excavadora
//************************************************************************

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// pala
//************************************************************************

class _pala : public _triangulos3D
{
public:
       _pala(float radio = 1.0, float ancho = 2.0, int num = 8);
};

//************************************************************************
// brazo
//************************************************************************

class _brazo : public _triangulos3D
{
public:
       _brazo();
       void draw(_modo modo, float r, float g, float b, float grosor);

       float ancho;
       float alto;
       float fondo;

protected:
       _cubo cubo;
};

//************************************************************************
// cabina
//************************************************************************

class _cabina : public _triangulos3D
{
public:
       _cabina();
       void draw(_modo modo, float r, float g, float b, float grosor);

       float ancho;
       float alto;
       float fondo;

protected:
       _cubo cubo;
};

//************************************************************************
// sustentación
//************************************************************************

class _sustentacion : public _triangulos3D
{
public:
       _sustentacion();
       void draw(_modo modo, float r, float g, float b, float grosor);

       float ancho;
       float alto;
       float fondo;

       float radio;

protected:
       _cilindro rueda;
       _cubo base;
};

//************************************************************************
// excavadora (montaje del objeto final)
//************************************************************************

class _excavadora : public _triangulos3D
{
public:
       _excavadora();

       void draw(_modo modo, float r, float g, float b, float grosor);

       float giro_cabina;
       float giro_primer_brazo;
       float giro_segundo_brazo;
       float giro_pala;

       float giro_primer_brazo_max;
       float giro_primer_brazo_min;
       float giro_segundo_brazo_max;
       float giro_segundo_brazo_min;
       float giro_pala_max;
       float giro_pala_min;

       float tamanio_pala;

protected:
       _pala pala;
       _brazo brazo;
       _cabina cabina;
       _sustentacion sustentacion;
};

//************************************************************************
// práctica 3, objeto jerárquico articulado WALL-E
//************************************************************************

//************************************************************************
// ojo
//************************************************************************

class _ojo : public _triangulos3D
{
public:
	_ojo();
	void draw(_modo modo, float r, float g, float b, float grosor);

	float radio_mayor;
	float radio_menor;
	float altura;

	_cilindro ojo;
	_cilindro iris;
	_cilindro pupila;
};

//************************************************************************
// ojos
//************************************************************************

class _ojos : public _triangulos3D
{
public:
	_ojos();
	void draw(_modo modo, float r, float g, float b, float grosor);

	_ojo ojo1;
	_ojo ojo2;
};

//************************************************************************
// cuello
//************************************************************************

class _cuello : public _triangulos3D
{
public:
	_cuello();
	void draw(_modo modo, float r, float g, float b, float grosor);

	float alto;

	_cilindro cuello;
	_cubo soporte_cuello;
};

//************************************************************************
// dedo
//************************************************************************

class _dedo : public _triangulos3D
{
public:
	_dedo();
	void draw(_modo modo, float r, float g, float b, float grosor);

	float ancho;
	float alto;
	float fondo;

	_cubo dedo;

};

//************************************************************************
// brazo
//************************************************************************

class _brazo_e : public _triangulos3D
{
public:
	_brazo_e();
	void draw(_modo modo, float r, float g, float b, float grosor);

	float ancho;
	float alto;
	float fondo;

	float giro_brazo;

	_dedo dedo1;
	_dedo dedo2;
	_dedo dedo3;
	_cilindro cilindro_brazo;
	_cilindro cilindro_anclaje;
	_cubo brazo;
};

//************************************************************************
// brazo
//************************************************************************

class _brazos_e : public _triangulos3D{
public:
	_brazos_e();
	void draw(_modo modo, float r, float g, float b, float grosor);
	
	_brazo_e brazo1;
	_brazo_e brazo2;
};

//************************************************************************
// compuerta
//************************************************************************

class _compuerta : public _triangulos3D
{
public:
	_compuerta();

	float ancho;
	float alto;
};

//************************************************************************
// cuerpo
//************************************************************************

class _cuerpo : public _triangulos3D
{
public:
	_cuerpo();
	void draw(_modo modo, float r, float g, float b, float grosor);

	float ancho;
	float alto;
	float fondo;

	float giro_compuerta;

	_compuerta compuerta;
	_cubo cuerpo;
};

//************************************************************************
// rueda
//************************************************************************

class _rueda : public _triangulos3D
{
public:
	_rueda();
	void draw(_modo modo, float r, float g, float b, float grosor);

	float r_mayor;
	float r_menor;
	_cilindro rueda;
};

//************************************************************************
// ruedas
//************************************************************************

class _ruedas : public _triangulos3D
{
public:
	_ruedas();
	void draw(_modo modo, float r, float g, float b, float grosor);

	float giro_ruedas;
	_rueda rueda1, rueda2;
};

//************************************************************************
// WALL-E
//************************************************************************

class _wall_e : public _triangulos3D
{
public:
	_wall_e();
	void draw(_modo modo, float r, float g, float b, float grosor);

	float giro_ojos;
	float giro_cuello;
	float giro_cuerpo;
	float pos_wall_e;
	float movimiento_brazos;

	_ruedas ruedas;
	_cuerpo cuerpo;
	_ojos ojos;
	_cuello cuello;
	_brazos_e brazos;
};