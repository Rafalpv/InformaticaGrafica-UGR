//**************************************************************************
// Práctica 3
//**************************************************************************

#include "objetos_B3.h"
#include "file_ply_stl.hpp"
#include <time.h>
#include <random>

//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
	//**** usando vertex_array ****
	glPointSize(grosor);
	glColor3f(r, g, b);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glDrawArrays(GL_POINTS, 0, vertices.size());

	/*int i;
	glPointSize(grosor);
	glColor3f(r,g,b);
	glBegin(GL_POINTS);
	for (i=0;i<vertices.size();i++){
		glVertex3fv((GLfloat *) &vertices[i]);
		}
	glEnd();*/
}

//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
	calculadas_normales_caras = 0;
	ambiente = _vertex4f(0.1, 0.18725, 0.8, 0.8);
	difuso = _vertex4f(0.369, 0.74151, 0.69102, 0.8);
	especular = _vertex4f(0.297254, 0.30829, 0.306678, 0.8);
	brillo = 12.8;
}

//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
	//**** usando vertex_array ****
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(grosor);
	glColor3f(r, g, b);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glDrawElements(GL_TRIANGLES, caras.size() * 3, GL_UNSIGNED_INT, &caras[0]);

	/*int i;
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glLineWidth(grosor);
	glColor3f(r,g,b);
	glBegin(GL_TRIANGLES);
	for (i=0;i<caras.size();i++){
		glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
		}
	glEnd();*/
}

//*************************************************************************
// dibujar en modo sólido con un único color
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
	size_t i;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < caras.size(); i++)
	{
		glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
	}
	glEnd();
}

//*************************************************************************
// dibujar en modo sólido con colores diferentes para cada cara
//*************************************************************************

void _triangulos3D::draw_solido_colores()
{
	size_t i;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < caras.size(); i++)
	{
		glColor3f(colores_caras[i].r, colores_caras[i].g, colores_caras[i].b);
		glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
	}
	glEnd();
}

//*************************************************************************
// dibujar en modo sólido con colores diferentes para cada vertice
//*************************************************************************

void _triangulos3D::draw_solido_colores_vertices()
{
	size_t i;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < caras.size(); i++)
	{
		glColor3f(colores_vertices[caras[i]._0].r, colores_vertices[caras[i]._0].g, colores_vertices[caras[i]._0].b);
		glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
		glColor3f(colores_vertices[caras[i]._1].r, colores_vertices[caras[i]._1].g, colores_vertices[caras[i]._1].b);
		glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
		glColor3f(colores_vertices[caras[i]._2].r, colores_vertices[caras[i]._2].g, colores_vertices[caras[i]._2].b);
		glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
	}
	glEnd();
	glShadeModel(GL_FLAT);
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r, float g, float b, float grosor)
{
	switch (modo)
	{
	case POINTS:
		draw_puntos(r, g, b, grosor);
		break;
	case EDGES:
		draw_aristas(r, g, b, grosor);
		break;
	case SOLID:
		draw_solido(r, g, b);
		break;
	case SOLID_COLORS:
		draw_solido_colores();
		break;
	case SOLID_COLORS_GOURAUD:
		draw_solido_colores_vertices();
		break;
	case SOLID_PHONG_FLAT:
		draw_solid_phong_flat();
		break;
	case SOLID_PHONG_GOURAUD:
		draw_solid_phong_gouraud();
		break;
	}
}

//*************************************************************************
// normales
//*************************************************************************

void _triangulos3D::calcular_normales_caras()
{
	int i, n_c;
	_vertex3f va, vb;
	float modulo;

	n_c = caras.size();
	normales_caras.resize(n_c);
	for (i = 0; i < n_c; i++)
	{
		va = vertices[caras[i]._1] - vertices[caras[i]._0];
		vb = vertices[caras[i]._2] - vertices[caras[i]._1];
		normales_caras[i].x = va.y * vb.z - va.z * vb.y;
		normales_caras[i].y = va.z * vb.x - va.x * vb.z;
		normales_caras[i].z = va.x * vb.y - va.y * vb.x;
		modulo = sqrt(normales_caras[i].x * normales_caras[i].x +
					  normales_caras[i].y * normales_caras[i].y +
					  normales_caras[i].z * normales_caras[i].z);
		normales_caras[i].x /= modulo;
		normales_caras[i].y /= modulo;
		normales_caras[i].z /= modulo;
	}
	calculadas_normales_caras = 1;
}

//*************************************************************************

void _triangulos3D::calcular_normales_vertices()
{
	int i, n_c, n_v;
	float modulo;

	n_v = vertices.size();
	normales_vertices.resize(n_v);

	n_c = caras.size();

	for (i = 0; i < n_v; i++)
		normales_vertices[i] = _vertex3f(0.0, 0.0, 0.0);

	for (i = 0; i < n_c; i++)
	{
		normales_vertices[caras[i]._0] += normales_caras[i];
		normales_vertices[caras[i]._1] += normales_caras[i];
		normales_vertices[caras[i]._2] += normales_caras[i];
	}

	for (i = 0; i < n_v; i++)
	{
		modulo = sqrt(normales_vertices[i].x * normales_vertices[i].x +
					  normales_vertices[i].y * normales_vertices[i].y +
					  normales_vertices[i].z * normales_vertices[i].z);
		normales_vertices[i].x /= modulo;
		normales_vertices[i].y /= modulo;
		normales_vertices[i].z /= modulo;
	}
}

//*************************************************************************
// asignación colores
//*************************************************************************

void _triangulos3D::colors_random()
{
	int i, n_c;
	n_c = caras.size();
	colores_caras.resize(n_c);
	srand(time(NULL));
	for (i = 0; i < n_c; i++)
	{
		colores_caras[i].r = rand() % 1000 / 1000.0;
		colores_caras[i].g = rand() % 1000 / 1000.0;
		colores_caras[i].b = rand() % 1000 / 1000.0;
	}
}

void _triangulos3D::draw_gama_color(char color){
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<double> dis(0.0, 1.0);
	double shade_of_color;

	colores_caras.resize(caras.size());
	for (size_t i = 0; i < colores_caras.size(); i++)
	{
		shade_of_color = dis(gen);
		colores_caras[i].r = 0.0;
		colores_caras[i].g = 0.0;
		colores_caras[i].b = 0.0;

		switch (color)
		{
		case 'r':
			colores_caras[i].r = shade_of_color;
			break;
		case 'b':
			colores_caras[i].b = shade_of_color;
			break;
		case 'g':
			colores_caras[i].g = shade_of_color;
			break;
		default:
			colores_caras[i].r = colores_caras[i].g = colores_caras[i].b = shade_of_color;
			break;
		}
	}
}

void _triangulos3D::draw_cuadrantes()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<double> dis(0.0, 1.0);
	double eje_x, eje_y, shade_color;

	colores_caras.resize(caras.size());
	for (size_t i = 0; i < colores_caras.size(); i++)
	{
		shade_color = dis(gen);
		eje_x = (vertices[caras[i]._0].x + vertices[caras[i]._1].x + vertices[caras[i]._2].x) / 3;
		eje_y = (vertices[caras[i]._0].y + vertices[caras[i]._1].y + vertices[caras[i]._2].y) / 3;

		if (eje_x > 0.0 && eje_y > 0.0) // 1º Cuadrante
		{
			colores_caras[i].r = 0.4;
			colores_caras[i].g = shade_color;
			colores_caras[i].b = 0.3;
		}
		else if (eje_x < 0.0 && eje_y > 0.0) // 2º Cuadrante
		{
			colores_caras[i].r = shade_color;
			colores_caras[i].g = 0.4;
			colores_caras[i].b = 0.2;
		}
		else if (eje_x > 0.0 && eje_y < 0.0) // 3º Cuadrante
		{
			colores_caras[i].r = 0.5;
			colores_caras[i].g = 0.3;
			colores_caras[i].b = shade_color;
		}
		else // 4º Cuadrante
		{
			colores_caras[i].r = shade_color;
			colores_caras[i].g = shade_color;
			colores_caras[i].b = shade_color;
		}
	}
}

void _triangulos3D::colors_chess(float r1, float g1, float b1, float r2, float g2, float b2)
{
	int i, n_c;
	n_c = caras.size();
	colores_caras.resize(n_c);
	for (i = 0; i < n_c; i++)
	{
		if (i % 2 == 0)
		{
			colores_caras[i].r = r1;
			colores_caras[i].g = g1;
			colores_caras[i].b = b1;
		}
		else
		{
			colores_caras[i].r = r2;
			colores_caras[i].g = g2;
			colores_caras[i].b = b2;
		}
	}
}

//*************************************************************************

void _triangulos3D::colors_diffuse_flat(float kr, float kg, float kb, float lpx, float lpy, float lpz)
{
	int i, n_c;
	float modulo, escalar;
	_vertex3f l;

	n_c = caras.size();
	colores_caras.resize(n_c);
	for (i = 0; i < n_c; i++)
	{
		l.x = lpx - vertices[caras[i]._0].x;
		l.y = lpy - vertices[caras[i]._0].y;
		l.z = lpz - vertices[caras[i]._0].z;
		modulo = sqrt(l.x * l.x + l.y * l.y + l.z * l.z);
		l.x /= modulo;
		l.y /= modulo;
		l.z /= modulo;

		escalar = l.x * normales_caras[i].x + l.y * normales_caras[i].y + l.z * normales_caras[i].z;
		if (escalar > 0.0)
		{
			colores_caras[i].r = kr * escalar;
			colores_caras[i].g = kg * escalar;
			colores_caras[i].b = kb * escalar;
		}
		else
		{
			colores_caras[i].r = 0.0;
			colores_caras[i].g = 0.0;
			colores_caras[i].b = 0.0;
		}
	}
}

void _triangulos3D::colors_diffuse_gouraud(float kr, float kg, float kb, float lpx, float lpy, float lpz)
{
	int i, n_v;
	float modulo, escalar;
	_vertex3f l;

	n_v = vertices.size();
	colores_vertices.resize(n_v);
	for (i = 0; i < n_v; i++)
	{
		l.x = lpx - vertices[i].x;
		l.y = lpy - vertices[i].y;
		l.z = lpz - vertices[i].z;
		modulo = sqrt(l.x * l.x + l.y * l.y + l.z * l.z);
		l.x /= modulo;
		l.y /= modulo;
		l.z /= modulo;

		escalar = l.x * normales_vertices[i].x + l.y * normales_vertices[i].y + l.z * normales_vertices[i].z;
		if (escalar > 0.0)
		{
			colores_vertices[i].r = kr * escalar;
			colores_vertices[i].g = kg * escalar;
			colores_vertices[i].b = kb * escalar;
		}
		else
		{
			colores_vertices[i].r = 0.0;
			colores_vertices[i].g = 0.0;
			colores_vertices[i].b = 0.0;
		}
	}
}

void _triangulos3D::draw_solid_phong_flat()
{
	size_t i;
	glShadeModel(GL_FLAT);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)&ambiente);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)&difuso);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < caras.size(); i++)
	{
		glNormal3f(normales_caras[i].x, normales_caras[i].y, normales_caras[i].z);
		glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
	}
	glEnd();

	glDisable(GL_LIGHTING);
}

void _triangulos3D::draw_solid_phong_gouraud()
{
	size_t i;
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)&ambiente);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)&difuso);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < caras.size(); i++)
	{
		glNormal3f(normales_vertices[caras[i]._0].x, normales_vertices[caras[i]._0].y, normales_vertices[caras[i]._0].z);
		glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
		glNormal3f(normales_vertices[caras[i]._1].x, normales_vertices[caras[i]._1].y, normales_vertices[caras[i]._1].z);
		glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
		glNormal3f(normales_vertices[caras[i]._2].x, normales_vertices[caras[i]._2].y, normales_vertices[caras[i]._2].z);
		glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
	}
	glEnd();

	glDisable(GL_LIGHTING);
}

//*************************************************************************
// objetos o modelos
//*************************************************************************

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
	tam_lado = tam;
	
	// vertices
	vertices.resize(8);
	vertices[0].x = -tam;
	vertices[0].y = -tam;
	vertices[0].z = tam;
	vertices[1].x = tam;
	vertices[1].y = -tam;
	vertices[1].z = tam;
	vertices[2].x = tam;
	vertices[2].y = tam;
	vertices[2].z = tam;
	vertices[3].x = -tam;
	vertices[3].y = tam;
	vertices[3].z = tam;
	vertices[4].x = -tam;
	vertices[4].y = -tam;
	vertices[4].z = -tam;
	vertices[5].x = tam;
	vertices[5].y = -tam;
	vertices[5].z = -tam;
	vertices[6].x = tam;
	vertices[6].y = tam;
	vertices[6].z = -tam;
	vertices[7].x = -tam;
	vertices[7].y = tam;
	vertices[7].z = -tam;

	// triangulos
	caras.resize(12);
	caras[0]._0 = 0;
	caras[0]._1 = 1;
	caras[0]._2 = 3;
	caras[1]._0 = 3;
	caras[1]._1 = 1;
	caras[1]._2 = 2;
	caras[2]._0 = 1;
	caras[2]._1 = 5;
	caras[2]._2 = 2;
	caras[3]._0 = 5;
	caras[3]._1 = 6;
	caras[3]._2 = 2;
	caras[4]._0 = 5;
	caras[4]._1 = 4;
	caras[4]._2 = 6;
	caras[5]._0 = 4;
	caras[5]._1 = 7;
	caras[5]._2 = 6;
	caras[6]._0 = 0;
	caras[6]._1 = 7;
	caras[6]._2 = 4;
	caras[7]._0 = 0;
	caras[7]._1 = 3;
	caras[7]._2 = 7;
	caras[8]._0 = 3;
	caras[8]._1 = 2;
	caras[8]._2 = 7;
	caras[9]._0 = 2;
	caras[9]._1 = 6;
	caras[9]._2 = 7;
	caras[10]._0 = 0;
	caras[10]._1 = 1;
	caras[10]._2 = 4;
	caras[11]._0 = 1;
	caras[11]._1 = 5;
	caras[11]._2 = 4;

	// normales
	calcular_normales_caras();
	calcular_normales_vertices();

	// colores de las caras
	colors_diffuse_flat(0.8, 0.9, 0.2, 20, 20, 20);
	colors_diffuse_gouraud(0.8, 0.9, 0.2, 20, 20, 20);
}

//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{
	// vertices
	vertices.resize(5);
	vertices[0].x = -tam;
	vertices[0].y = 0;
	vertices[0].z = tam;
	vertices[1].x = tam;
	vertices[1].y = 0;
	vertices[1].z = tam;
	vertices[2].x = tam;
	vertices[2].y = 0;
	vertices[2].z = -tam;
	vertices[3].x = -tam;
	vertices[3].y = 0;
	vertices[3].z = -tam;
	vertices[4].x = 0;
	vertices[4].y = al;
	vertices[4].z = 0;

	caras.resize(6);
	caras[0]._0 = 0;
	caras[0]._1 = 1;
	caras[0]._2 = 4;
	caras[1]._0 = 1;
	caras[1]._1 = 2;
	caras[1]._2 = 4;
	caras[2]._0 = 2;
	caras[2]._1 = 3;
	caras[2]._2 = 4;
	caras[3]._0 = 3;
	caras[3]._1 = 0;
	caras[3]._2 = 4;
	caras[4]._0 = 3;
	caras[4]._1 = 1;
	caras[4]._2 = 0;
	caras[5]._0 = 3;
	caras[5]._1 = 2;
	caras[5]._2 = 1;

	// normales
	calcular_normales_caras();
	calcular_normales_vertices();

	// colores de las caras
	colors_diffuse_flat(0.8, 0.9, 0.2, 20, 20, 20);
	colors_diffuse_gouraud(0.8, 0.9, 0.2, 20, 20, 20);

}

//*************************************************************************
// clase objeto ply
//*************************************************************************

_objeto_ply::_objeto_ply()
{
	// leer lista de coordenadas de vértices y lista de indices de vértices
}

void _objeto_ply::parametros(char *archivo)
{
	int i, n_ver, n_car;

	vector<float> ver_ply;
	vector<int> car_ply;

	_file_ply::read(archivo, ver_ply, car_ply);

	n_ver = ver_ply.size() / 3;
	n_car = car_ply.size() / 3;

	printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

	vertices.resize(n_ver);
	caras.resize(n_car);
	// vértices

	for (i = 0; i < n_ver; i++)
	{
		vertices[i].x = ver_ply[3 * i];
		vertices[i].y = ver_ply[3 * i + 1];
		vertices[i].z = ver_ply[3 * i + 2];
	}

	// vértices
	for (i = 0; i < n_car; i++)
	{
		caras[i].x = car_ply[3 * i];
		caras[i].y = car_ply[3 * i + 1];
		caras[i].z = car_ply[3 * i + 2];
	}

	// normales
	calcular_normales_caras();
	calcular_normales_vertices();

	// colores
	colors_diffuse_flat(0.7, 0.2, 0.6, 20, 20, 20);
	colors_diffuse_gouraud(0.7, 0.2, 0.6, 20, 20, 20);
}

//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{
}

void _rotacion::calcular_normales_esfera()
{
	normales_vertices.resize(vertices.size());

	for (size_t i = 0; i < vertices.size(); i++)
	{
		normales_vertices[i] = _vertex3f(0.0, 0.0, 0.0);
		normales_vertices[i]._0 = vertices[i]._0;
		normales_vertices[i]._1 = vertices[i]._1;
		normales_vertices[i]._2 = vertices[i]._2;
		normales_vertices[i].normalize();
	}
}

void _rotacion::parametros(vector<_vertex3f> perfil, int num,
						   int tapa_in, int tapa_su, int tipo)
{
	int i, j;
	_vertex3f vertice_aux;
	_vertex3i cara_aux;
	int num_aux;
	float radio;

	// tratamiento de los vértice
	radio = sqrt(perfil[0].x * perfil[0].x + perfil[0].y * perfil[0].y);

	num_aux = perfil.size();
	if (tipo == 2)
		num_aux = 1;
	vertices.resize(num_aux * num);

	for (j = 0; j < num; j++)
	{
		for (i = 0; i < num_aux; i++)
		{
			vertice_aux.x = perfil[i].x * cos(2.0 * M_PI * j / (1.0 * num)) +
							perfil[i].z * sin(2.0 * M_PI * j / (1.0 * num));
			vertice_aux.z = -perfil[i].x * sin(2.0 * M_PI * j / (1.0 * num)) +
							perfil[i].z * cos(2.0 * M_PI * j / (1.0 * num));
			vertice_aux.y = perfil[i].y;
			vertices[i + j * num_aux] = vertice_aux;
		}
	}

	// tratamiento de las caras
	if (tipo < 2)
	{
		for (j = 0; j < num; j++)
		{
			for (i = 0; i < num_aux - 1; i++)
			{
				cara_aux._0 = i + j * num_aux;
				cara_aux._1 = i + ((j + 1) % num) * num_aux;
				cara_aux._2 = i + 1 + j * num_aux;
				caras.push_back(cara_aux);

				cara_aux._0 = i + ((j + 1) % num) * num_aux;
				cara_aux._1 = i + 1 + ((j + 1) % num) * num_aux;
				cara_aux._2 = i + 1 + j * num_aux;
				caras.push_back(cara_aux);
			}
		}
	}

	// tapa inferior
	if (tapa_in == 1)
	{
		// punto central de la tapa
		vertice_aux.x = 0.0;
		if (tipo == 1)
			vertice_aux.y = -radio;
		else
			vertice_aux.y = perfil[0].y;
		vertice_aux.z = 0.0;
		vertices.push_back(vertice_aux);
		// caras tapa inferior

		cara_aux._0 = num_aux * num;
		for (j = 0; j < num; j++)
		{
			cara_aux._1 = ((j + 1) % num) * num_aux;
			cara_aux._2 = j * num_aux;
			caras.push_back(cara_aux);
		}
	}

	// tapa superior

	if (tapa_su == 1)
	{
		// punto central de la tapa
		vertice_aux.x = 0.0;
		vertice_aux.y = perfil[num_aux - 1].y;
		if (tipo == 1)
			vertice_aux.y = radio;
		if (tipo == 2)
			vertice_aux.y = perfil[1].y;
		vertice_aux.z = 0.0;
		vertices.push_back(vertice_aux);

		// caras tapa superior
		if (tapa_in == 1)
			cara_aux._0 = num_aux * num + 1;
		else
			cara_aux._0 = num_aux * num;

		for (j = 0; j < num; j++)
		{
			cara_aux._1 = j * num_aux + num_aux - 1;
			cara_aux._2 = ((j + 1) % num) * num_aux + num_aux - 1;
			caras.push_back(cara_aux);
		}
	}

	// normales
	calcular_normales_caras();
	if(tipo==1)
		calcular_normales_esfera();
	else
		calcular_normales_vertices();

	// colores de las caras
	colors_diffuse_flat(0.8, 0.9, 0.2, 20, 20, 20);
	colors_diffuse_gouraud(0.8, 0.9, 0.2, 20, 20, 20);
}

//************************************************************************
// objeto por extrusión
//************************************************************************

_extrusion::_extrusion(vector<_vertex3f> poligono, float x, float y, float z)
{
	int i;
	_vertex3f vertice_aux;
	_vertex3i cara_aux;
	int num_aux;

	// tratamiento de los vértice

	num_aux = poligono.size();
	vertices.resize(num_aux * 2);
	for (i = 0; i < num_aux; i++)
	{
		vertices[2 * i] = poligono[i];
		vertices[2 * i + 1].x = poligono[i].x + x;
		vertices[2 * i + 1].y = poligono[i].y + y;
		vertices[2 * i + 1].z = poligono[i].z + z;
	}

	// tratamiento de las caras

	caras.resize(num_aux * 2);
	int c = 0;
	for (i = 0; i < num_aux; i++)
	{
		caras[c]._0 = i * 2;
		caras[c]._1 = (i * 2 + 2) % (num_aux * 2);
		caras[c]._2 = i * 2 + 1;
		c = c + 1;
		caras[c]._0 = (i * 2 + 2) % (num_aux * 2);
		caras[c]._1 = (i * 2 + 2) % (num_aux * 2) + 1;
		caras[c]._2 = i * 2 + 1;
		c = c + 1;
	}

	// normales
	calcular_normales_caras();
	calcular_normales_vertices();

	// colores de las caras
	colors_diffuse_flat(0.8, 0.9, 0.2, 20, 20, 20);
	colors_diffuse_gouraud(0.8, 0.9, 0.2, 20, 20, 20);
}

//************************************************************************
//************************************************************************

//************************************************************************
// objeto cilindro (caso especial de rotacion)
//************************************************************************

_cilindro::_cilindro(float radio, float altura, int num)
{
	vector<_vertex3f> perfil;
	_vertex3f aux;

	aux.x = radio;
	aux.y = -altura / 2.0;
	aux.z = 0.0;
	perfil.push_back(aux);
	aux.x = radio;
	aux.y = altura / 2.0;
	aux.z = 0.0;
	perfil.push_back(aux);
	parametros(perfil, num, 1, 1, 0);
}

//************************************************************************
// objeto cono (caso especial de rotacion)
//************************************************************************

_cono::_cono(float radio, float altura, int num)
{
	vector<_vertex3f> perfil;
	_vertex3f aux;

	aux.x = radio;
	aux.y = 0;
	aux.z = 0.0;
	perfil.push_back(aux);
	aux.x = 0.0;
	aux.y = altura;
	aux.z = 0.0;
	perfil.push_back(aux);
	parametros(perfil, num, 1, 1, 2);
}

//************************************************************************
// objeto esfera (caso especial de rotacion)
//************************************************************************

_esfera::_esfera(float radio, int latitud, int longitud)
{
	vector<_vertex3f> perfil;
	_vertex3f aux;
	int i;
	for (i = 1; i < latitud; i++)
	{
		aux.x = radio * cos(M_PI * i / (latitud * 1.0) - M_PI / 2.0);
		aux.y = radio * sin(M_PI * i / (latitud * 1.0) - M_PI / 2.0);
		aux.z = 0.0;
		perfil.push_back(aux);
	}
	parametros(perfil, longitud, 1, 1, 1);
	calcular_normales_esfera();
}

//************************************************************************
// rotacion archivo PLY (caso especial de rotacion)
//************************************************************************

_rotacion_PLY::_rotacion_PLY()
{
}

void _rotacion_PLY::parametros_PLY(char *archivo, int num)
{
	int i, n_ver;

	vector<float> ver_ply;
	vector<int> car_ply;

	_file_ply::read(archivo, ver_ply, car_ply);

	n_ver = ver_ply.size() / 3;

	printf("Number of vertices=%d\n", n_ver);

	vertices.resize(n_ver);
	// vértices

	for (i = 0; i < n_ver; i++)
	{
		vertices[i].x = ver_ply[3 * i];
		vertices[i].y = ver_ply[3 * i + 1];
		vertices[i].z = ver_ply[3 * i + 2];
	}

	parametros(vertices, num, 30, 1, 0);

	calcular_normales_caras();
	calcular_normales_vertices();

	// colores de las caras
	colors_diffuse_flat(0.8, 0.9, 0.2, 20, 20, 20);
	colors_diffuse_gouraud(0.8, 0.9, 0.2, 20, 20, 20);
}

//************************************************************************
// objeto montaña fractal
//************************************************************************

float gauss(float ga, float gf)
{
	float sum;
	int i;
	sum = 0.0;
	for (i = 0; i < 4; i++)
		sum = sum + rand() % 32767;
	return gf * sum / 4.0 - ga;
}

_montana::_montana(int nivelmax, float sigma, float h)

{

	if (nivelmax > 8)
		nivelmax = 8;
	int i, j, etapa;
	float ga = sqrt(12.0);
	float gf = 2 * ga / (32767 * 1.0);
	int num = pow(2, nivelmax) + 1;
	srand(time(NULL));

	vertices.resize(num * num);

	for (j = 0; j < num; j++)
		for (i = 0; i < num; i++)
		{
			vertices[i + j * num].x = -0.1 * (num - 1) / 2.0 + i * 0.1;
			vertices[i + j * num].z = -0.1 * (num - 1) / 2.0 + j * 0.1;
			vertices[i + j * num].y = 0.0;
		}

	vertices[0].y = sigma * gauss(ga, gf);
	vertices[num - 1].y = sigma * gauss(ga, gf);
	vertices[num * (num - 1)].y = sigma * gauss(ga, gf);
	vertices[num * num - 1].y = sigma * gauss(ga, gf);

	int d1 = num - 1;
	int d2 = (num - 1) / 2;

	for (etapa = 0; etapa < nivelmax; etapa++)
	{
		sigma = sigma * pow(0.5, 0.5 * h);
		for (j = d2; j < num - d2; j = j + d1)
			for (i = d2; i < num - d2; i = i + d1)
			{
				vertices[i + j * num].y = sigma * gauss(ga, gf) +
										  (vertices[i + d2 + (j + d2) * num].y + vertices[i + d2 + (j - d2) * num].y +
										   vertices[i - d2 + (j + d2) * num].y + vertices[i - d2 + (j - d2) * num].y) /
											  4.0;
			}
		sigma = sigma * pow(0.5, 0.5 * h);
		for (i = d2; i < num - d2; i = i + d1)
		{
			vertices[i].y = sigma * gauss(ga, gf) + (vertices[i + d2].y +
													 vertices[i - d2].y + vertices[i + d2 * num].y) /
														3.0;
			vertices[i + num * (num - 1)].y = sigma * gauss(ga, gf) +
											  (vertices[i + d2 + num * (num - 1)].y +
											   vertices[i - d2 + num * (num - 1)].y +
											   vertices[i + (num - 1 - d2) * num].y) /
												  3.0;
			vertices[i * num].y = sigma * gauss(ga, gf) + (vertices[(i + d2) * num].y +
														   vertices[(i - d2) * num].y + vertices[d2 + i * num].y) /
															  3.0;
			vertices[num - 1 + i * num].y = sigma * gauss(ga, gf) +
											(vertices[num - 1 + (i + d2) * num].y +
											 vertices[num - 1 + (i - d2) * num].y +
											 vertices[num - 1 - d2 + i * num].y) /
												3;
		}

		for (j = d2; j < num - d2; j = j + d1)
			for (i = d1; i < num - d2; i = i + d1)
				vertices[i + j * num].y = sigma * gauss(ga, gf) +
										  (vertices[i + (j + d2) * num].y + vertices[i + (j - d2) * num].y +
										   vertices[i + d2 + j * num].y + vertices[i - d2 + j * num].y) /
											  4.0;
		for (j = d1; j < num - d2; j = j + d1)
			for (i = d2; i < num - d2; i = i + d1)
				vertices[i + j * num].y = sigma * gauss(ga, gf) +
										  (vertices[i + (j + d2) * num].y + vertices[i + (j - d2) * num].y +
										   vertices[i + d2 + j * num].y + vertices[i - d2 + j * num].y) /
											  4.0;

		d1 = (int)d1 / 2;
		d2 = (int)d2 / 2;
	}

	// caras
	caras.resize((num - 1) * (num - 1) * 2);
	int c = 0;
	for (j = 0; j < num - 1; j++)
		for (i = 0; i < num - 1; i++)
		{
			caras[c]._0 = i + j * num;
			caras[c]._1 = i + 1 + j * num;
			caras[c]._2 = i + 1 + (j + 1) * num;
			c = c + 1;
			caras[c]._0 = i + j * num;
			caras[c]._1 = i + 1 + (j + 1) * num;
			caras[c]._2 = i + (j + 1) * num;
			c = c + 1;
		}

	// normales
	calcular_normales_caras();
	calcular_normales_vertices();

	// colores de las caras
	colors_diffuse_flat(0.8, 0.9, 0.2, 20, 20, 20);
	colors_diffuse_gouraud(0.8, 0.9, 0.2, 20, 20, 20);
}

//************************************************************************
// práctica 3, objeto jerárquico articulado
//************************************************************************

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// pala
//************************************************************************

_pala::_pala(float radio, float ancho, int num)
{
	vector<_vertex3f> perfil;
	_vertex3f vertice_aux;
	_vertex3i cara_aux;
	int i, j;

	vertice_aux.x = radio;
	vertice_aux.y = 0;
	vertice_aux.z = -ancho / 2.0;
	perfil.push_back(vertice_aux);
	vertice_aux.z = ancho / 2.0;
	perfil.push_back(vertice_aux);

	// tratamiento de los vértices

	for (j = 0; j <= num; j++)
	{
		for (i = 0; i < 2; i++)
		{
			vertice_aux.x = perfil[i].x * cos(M_PI * j / (1.0 * num)) -
							perfil[i].y * sin(M_PI * j / (1.0 * num));
			vertice_aux.y = perfil[i].x * sin(M_PI * j / (1.0 * num)) +
							perfil[i].y * cos(M_PI * j / (1.0 * num));
			vertice_aux.z = perfil[i].z;
			vertices.push_back(vertice_aux);
		}
	}

	// tratamiento de las caras

	for (j = 0; j < num; j++)
	{
		cara_aux._0 = j * 2;
		cara_aux._1 = (j + 1) * 2;
		cara_aux._2 = (j + 1) * 2 + 1;
		caras.push_back(cara_aux);

		cara_aux._0 = j * 2;
		cara_aux._1 = (j + 1) * 2 + 1;
		cara_aux._2 = j * 2 + 1;
		caras.push_back(cara_aux);
	}

	// tapa inferior
	vertice_aux.x = 0;
	vertice_aux.y = 0;
	vertice_aux.z = -ancho / 2.0;
	vertices.push_back(vertice_aux);

	for (j = 0; j < num; j++)
	{
		cara_aux._0 = j * 2;
		cara_aux._1 = (j + 1) * 2;
		cara_aux._2 = vertices.size() - 1;
		caras.push_back(cara_aux);
	}

	// tapa superior
	vertice_aux.x = 0;
	vertice_aux.y = 0;
	vertice_aux.z = ancho / 2.0;
	vertices.push_back(vertice_aux);

	for (j = 0; j < num; j++)
	{
		cara_aux._0 = j * 2 + 1;
		cara_aux._1 = (j + 1) * 2 + 1;
		cara_aux._2 = vertices.size() - 1;
		caras.push_back(cara_aux);
	}

	calcular_normales_caras();
	calcular_normales_vertices();

	colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}

//************************************************************************
// brazo
//************************************************************************

_brazo::_brazo()
{
	ancho = 0.6;
	alto = 0.1;
	fondo = 0.1;
	colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
};

void _brazo::draw(_modo modo, float r, float g, float b, float grosor)
{
	glPushMatrix();
	glScalef(ancho, alto, fondo);
	glTranslatef(0.5, 0, 0);
	cubo.draw(modo, r, g, b, grosor);
	glPopMatrix();
};

//************************************************************************
// cabina
//************************************************************************

_cabina::_cabina()
{
	ancho = 0.4;
	alto = 0.6;
	fondo = 0.4;
	cubo.colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
};

void _cabina::draw(_modo modo, float r, float g, float b, float grosor)
{
	glPushMatrix();
	glScalef(ancho, alto, fondo);
	cubo.draw(modo, r, g, b, grosor);
	glPopMatrix();
};

//************************************************************************
// sustentación
//************************************************************************

_sustentacion::_sustentacion()
{
	ancho = 1.2;
	alto = 0.3;
	fondo = 0.8;
	radio = 0.15;
	base.colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
};

void _sustentacion::draw(_modo modo, float r, float g, float b, float grosor)
{
	glPushMatrix();
	glTranslatef(2 * ancho / 6, -alto / 2.0, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(radio, fondo / 2.2, radio);
	rueda.draw(modo, r, g, b, grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2 * ancho / 6, -alto / 2.0, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(radio, fondo / 2.2, radio);
	rueda.draw(modo, r, g, b, grosor);
	glPopMatrix();

	glPushMatrix();
	glScalef(ancho, alto, fondo);
	base.draw(modo, r, g, b, grosor);
	glPopMatrix();
};

//************************************************************************
// excavadora (montaje del objeto final)
//************************************************************************

_excavadora::_excavadora()
{
	giro_cabina = 0.0;
	giro_primer_brazo = 0.0;
	giro_primer_brazo_max = 0;
	giro_primer_brazo_min = -90;
	giro_segundo_brazo = 0.0;
	giro_segundo_brazo_max = 30;
	giro_segundo_brazo_min = 0;
	giro_pala = 0.0;
	giro_pala_max = 50.0;
	giro_pala_min = -90.0;

	tamanio_pala = 0.15;
};

void _excavadora::draw(_modo modo, float r, float g, float b, float grosor)
{
	glPushMatrix();

	sustentacion.draw(modo, r, g, b, grosor);

	glTranslatef(0, (cabina.alto + sustentacion.alto) / 2.0, 0);
	glRotatef(giro_cabina, 0, 1, 0);
	cabina.draw(modo, r, g, b, grosor);

	glTranslatef(cabina.ancho / 2.0, 0, 0);
	glRotatef(giro_segundo_brazo, 0, 0, 1);
	brazo.draw(modo, r, g, b, grosor);

	glTranslatef(brazo.ancho, 0, 0);
	glRotatef(giro_primer_brazo, 0, 0, 1);
	brazo.draw(modo, r, g, b, grosor);

	glTranslatef(brazo.ancho, 0, 0);
	glRotatef(giro_pala, 0, 0, 1);
	glTranslatef(tamanio_pala, 0, 0);
	glScalef(tamanio_pala, tamanio_pala, tamanio_pala);
	pala.draw(modo, r, g, b, grosor);

	glPopMatrix();
};

//************************************************************************
// PRÁCTICA - 3, OBJETO JERARQUICO - WALL_E
//************************************************************************

//************************************************************************
// ojo
//************************************************************************

_ojo::_ojo()
{
	radio_menor = 0.55;
	radio_mayor = 0.8;
	altura = 1.7;
	iris.colors_chess(0,0,0,0,0,0);
	pupila.colors_chess(1,1,1,1,1,1);
}

void _ojo::draw(_modo modo, float r, float g, float b, float grosor)
{

	vector<_vertex3f> perfil;
	_vertex3f punto_perfil;

	punto_perfil.x = radio_menor;
	punto_perfil.y = 0.0;
	punto_perfil.z = 0.0;
	perfil.push_back(punto_perfil);

	punto_perfil.x = radio_mayor;
	punto_perfil.y = altura;
	punto_perfil.z = 0.0;
	perfil.push_back(punto_perfil);

	ojo.parametros(perfil, 30, 1, 1, 0);
	ojo.colors_chess(.3, .3, .3, .3, .3, .3);

	glPushMatrix();
	glRotatef(90,1.0,0.0,0.0);
	ojo.draw(modo, r, g, b, grosor);

	glTranslatef(0,1.71,0);
	glScalef(0.65,0.01,0.65);
	pupila.draw(modo,r,g,b,grosor);
	
	glTranslatef(0,1.72,0);
	glScalef(0.6,0.01,0.6);
	iris.draw(modo,r,g,b,grosor);

	glPopMatrix();
}

//************************************************************************
// ojos
//************************************************************************

_ojos::_ojos(){
}

void _ojos::draw(_modo modo, float r, float g, float b, float grosor){
	glPushMatrix();
	glTranslated(-0.85,0.0,0.0);
	ojo2.draw(modo,r,g,b,grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.85,0.0,0.0);
	ojo1.draw(modo,r,g,b,grosor);
	glPopMatrix();
}

//************************************************************************
// cuello
//************************************************************************

_cuello::_cuello(){
	alto = 0.8;	
	soporte_cuello.colors_chess(.3, .3, .3, .3, .3, .3);
	cuello.colors_chess(.15, .15, .15, .15, .15, .15);

}

void _cuello::draw(_modo modo, float r, float g, float b, float grosor){

	glPushMatrix();
	glTranslatef(0.0,alto+0.01,0.0);
	glRotatef(180,0.0,1.0,1.0);
	glScalef(0.5,0.5,0.0);
	soporte_cuello.draw(modo,r,g,b,grosor);
	glPopMatrix();

	glPushMatrix();
	glScalef(0.2,alto,0.2);
	cuello.draw(modo,r,g,b,grosor);
	glPopMatrix();

}

//************************************************************************
// dedo
//************************************************************************

_dedo::_dedo(){
	ancho = 0.3;
	alto = 0.2;
	fondo = 0.7;
	dedo.colors_chess(.1,.1,.1,.1,.1,.1);

}

void _dedo::draw(_modo modo, float r, float g, float b, float grosor){
	glPushMatrix();
	glScalef(ancho,alto,fondo);
	dedo.draw(modo,r,g,b,grosor);
	glPopMatrix();
}


//************************************************************************
// brazo
//************************************************************************

_brazo_e::_brazo_e(){
	ancho = 1.4;
	alto = 0.5;
	fondo = 0.2;
	giro_brazo = 0;
	brazo.colors_chess(0.3,0.3,0.3,0.3,0.3,0.3);
	cilindro_brazo.colors_chess(0.1,0.1,0.1,0.1,0.1,0.1);
	cilindro_anclaje.colors_chess(0.1,0.1,0.1,0.1,0.1,0.1);
}

void _brazo_e::draw(_modo modo, float r, float g, float b, float grosor){
	
	glPushMatrix();
	glScalef(0.25,0.25,0.2);
	glTranslatef(2.4,0,-1.8);
	glRotatef(90,1,0,0);
	cilindro_anclaje.draw(modo,r,g,b,grosor);
	glPopMatrix();

	glPushMatrix();

	glRotatef(giro_brazo,0,0,1);

	glScalef(0.25,0.5,0.25);
	cilindro_brazo.draw(modo,r,g,b,grosor);

	glTranslatef(ancho*4.7,0,0);
	glScalef(ancho*4,alto*2,fondo*4);
	brazo.draw(modo,r,g,b,grosor);

	glTranslatef(1.3,-0.5,0.0);
	dedo3.draw(modo,r,g,b,grosor);
	glTranslatef(-1.3,0.5,0.0);

	glTranslatef(1.3,0.5,0.0);
	dedo2.draw(modo,r,g,b,grosor);
	glTranslatef(-1.3,-0.5,0.0);

	glTranslatef(1.3,0.0,0.0);
	dedo1.draw(modo,r,g,b,grosor);

	glPopMatrix();
	
}

//************************************************************************
// brazos
//************************************************************************

_brazos_e::_brazos_e(){
}

void _brazos_e::draw(_modo modo, float r, float g, float b, float grosor){

	glPushMatrix();
	glTranslatef(2.565,0.8,-0.5);
	glRotatef(180,0.0,0.0,1.0);
	glRotatef(-90,0.0,1.0,0.0);
	brazo1.draw(modo,r,g,b,grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.565,0.8,-0.5);
	glRotatef(-90,0.0,1.0,0.0);
	brazo2.draw(modo,r,g,b,grosor);
	glPopMatrix();
	
}

//************************************************************************
// compuerta
//************************************************************************

_compuerta::_compuerta()
{
	ancho = 1.0;
	alto = 1.0;

	vertices.resize(4);
	vertices[0].x = 0; vertices[0].y = 0; vertices[0].z = 0;
	vertices[1].x = ancho; vertices[1].y = 0; vertices[1].z = 0;
	vertices[2].x = ancho; vertices[2].y = alto; vertices[2].z = 0;
	vertices[3].x = 0; vertices[3].y = alto; vertices[3].z = 0;

	caras.resize(2);
	caras[0]._0 = 0;caras[0]._1 = 1;caras[0]._2 = 2;
	caras[1]._0 = 0;caras[1]._1 = 2;caras[1]._2 = 3;

	calcular_normales_caras();
	calcular_normales_vertices();

	colors_diffuse_flat(0.7, 0.2, 0.6, 20, 20, 20);
	colors_diffuse_gouraud(0.7, 0.2, 0.6, 20, 20, 20);
}

//************************************************************************
// cuerpo
//************************************************************************

_cuerpo::_cuerpo()
{
	ancho = 2;
	alto = 2;
	fondo = 2;
	giro_compuerta = 0.0;
	compuerta.colors_chess(.2, .2, .2, .2, .2, .2);
}

void _cuerpo::draw(_modo modo, float r, float g, float b, float grosor)
{
	cuerpo.vertices.resize(12);
	cuerpo.vertices[8].x = -cuerpo.tam_lado + 0.25;
	cuerpo.vertices[8].y = -cuerpo.tam_lado + 0.25;
	cuerpo.vertices[8].z = cuerpo.tam_lado;
	cuerpo.vertices[9].x = cuerpo.tam_lado - 0.25;
	cuerpo.vertices[9].y = -cuerpo.tam_lado + 0.25;
	cuerpo.vertices[9].z = cuerpo.tam_lado;
	cuerpo.vertices[10].x = cuerpo.tam_lado - 0.25;
	cuerpo.vertices[10].y = cuerpo.tam_lado - 0.25;
	cuerpo.vertices[10].z = cuerpo.tam_lado;
	cuerpo.vertices[11].x = -cuerpo.tam_lado + 0.25;
	cuerpo.vertices[11].y = cuerpo.tam_lado - 0.25;
	cuerpo.vertices[11].z = cuerpo.tam_lado;

	cuerpo.caras.resize(18);
	cuerpo.caras[0]._0 = 0;
	cuerpo.caras[0]._1 = 8;
	cuerpo.caras[0]._2 = 9;
	cuerpo.caras[1]._0 = 0;
	cuerpo.caras[1]._1 = 1;
	cuerpo.caras[1]._2 = 9;

	cuerpo.caras[12]._0 = 1;
	cuerpo.caras[12]._1 = 9;
	cuerpo.caras[12]._2 = 10;
	cuerpo.caras[13]._0 = 1;
	cuerpo.caras[13]._1 = 2;
	cuerpo.caras[13]._2 = 10;

	cuerpo.caras[14]._0 = 2;
	cuerpo.caras[14]._1 = 10;
	cuerpo.caras[14]._2 = 11;
	cuerpo.caras[15]._0 = 2;
	cuerpo.caras[15]._1 = 3;
	cuerpo.caras[15]._2 = 11;

	cuerpo.caras[16]._0 = 3;
	cuerpo.caras[16]._1 = 8;
	cuerpo.caras[16]._2 = 11;
	cuerpo.caras[17]._0 = 0;
	cuerpo.caras[17]._1 = 3;
	cuerpo.caras[17]._2 = 8;

	glPushMatrix();

	glScalef(ancho,alto,fondo);
	cuerpo.draw(modo, r, g, b, grosor);

	glScalef(1.5,1.5,1);
	glTranslatef(-fondo/4,-fondo/4,fondo/2);
	glRotatef(giro_compuerta,1.0,0.0,0.0);
	compuerta.draw(modo,r,g,b,grosor);
	
	glPopMatrix();

	cuerpo.colors_chess(1.0, .67843, 0.003,1.0, .67843, 0.003);	
}

//************************************************************************
// rueda
//************************************************************************

_rueda::_rueda()
{
	r_menor = 1.4;
	r_mayor = 0.2;

}

void _rueda::draw(_modo modo, float r, float g, float b, float grosor)
{
	glPushMatrix();
	glRotatef(90,0.0,0.0,1.0);
	glScalef(r_menor, r_mayor, r_menor);
	rueda.draw(modo, r, g, b, grosor);
	glPopMatrix();

	rueda.colors_chess(.1,.1,.1,.1,.1,.1);

	
}

//************************************************************************
// ruedas
//************************************************************************

_ruedas::_ruedas(){
	giro_ruedas = 0.0;
	
}

void _ruedas::draw(_modo modo, float r, float g, float b, float grosor){

	glPushMatrix();
	glTranslatef(-2.22,-1.7,-0.2);
	glRotatef(giro_ruedas,1.0,0.0,0.0);
	rueda2.draw(modo,r,g,b,grosor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.22,-1.7,-0.2);
	glRotatef(giro_ruedas,1.0,0.0,0.0);
	rueda1.draw(modo,r,g,b,grosor);
	glPopMatrix();

}

//************************************************************************
// WALL-E
//************************************************************************

_wall_e::_wall_e()
{
	giro_ojos = 0.0;
	giro_cuello = 0.0;
	giro_cuerpo = 0.0;
	pos_wall_e = 0.0;
	movimiento_brazos = 0.0;

	cuerpo.cuerpo.calcular_normales_caras();
	cuerpo.cuerpo.ambiente = _vertex4f(0.24725, 0.1995, 0.0745, 1.0);
	cuerpo.cuerpo.difuso = _vertex4f(0.75164, 0.60648, 0.22648, 1.0);
	cuerpo.cuerpo.especular = _vertex4f(0.628181, 0.555802, 0.366065, 1.0);
	cuerpo.cuerpo.brillo = 51.2;

	cuerpo.compuerta.calcular_normales_caras();
	cuerpo.compuerta.ambiente = _vertex4f(0.4725, 0.995, 0.745, 1.0);
	cuerpo.compuerta.difuso = _vertex4f(0.5164, 0.0648, 0.22648, 1.0);
	cuerpo.compuerta.especular = _vertex4f(0.28181, 0.555802, 0.366065, 1.0);
	cuerpo.compuerta.brillo = 10.2;

	brazos.brazo1.brazo.calcular_normales_caras();
	brazos.brazo1.brazo.ambiente = _vertex4f(0.725, 0.995, 0.045, 1.0);
	brazos.brazo1.brazo.difuso = _vertex4f(0.64, 0.48, 0.2648, 1.0);
	brazos.brazo1.brazo.especular = _vertex4f(0.8181, 0.802, 0.065, 1.0);
	brazos.brazo1.brazo.brillo = 32.2;

	brazos.brazo2.brazo.calcular_normales_caras();
	brazos.brazo2.brazo.ambiente = _vertex4f(0.725, 0.995, 0.045, 1.0);
	brazos.brazo2.brazo.difuso = _vertex4f(0.64, 0.48, 0.2648, 1.0);
	brazos.brazo2.brazo.especular = _vertex4f(0.8181, 0.802, 0.065, 1.0);
	brazos.brazo2.brazo.brillo = 32.2;

	ojos.ojo1.pupila.calcular_normales_esfera();
	ojos.ojo1.pupila.ambiente = _vertex4f(0.25,0.25, 0.25, 1.0);
	ojos.ojo1.pupila.difuso = _vertex4f(0.4, 0.4, 0.4, 1.0);
	ojos.ojo1.pupila.especular = _vertex4f(0.774597, 0.774597, 0.774597, 1.0);
	ojos.ojo1.pupila.brillo = 76.8;

	ojos.ojo2.pupila.calcular_normales_esfera();
	ojos.ojo2.pupila.ambiente = _vertex4f(0.25,0.25, 0.25, 1.0);
	ojos.ojo2.pupila.difuso = _vertex4f(0.4, 0.4, 0.4, 1.0);
	ojos.ojo2.pupila.especular = _vertex4f(0.774597, 0.774597, 0.774597, 1.0);
	ojos.ojo2.pupila.brillo = 76.8;

	ojos.ojo1.ojo.calcular_normales_esfera();
	ojos.ojo1.ojo.ambiente = _vertex4f(0.135,0.2225, 0.1575, 0.82);
	ojos.ojo1.ojo.difuso = _vertex4f(0.54, 0.89, 0.63, 0.95);
	ojos.ojo1.ojo.especular = _vertex4f(0.316228, 0.316228, 0.316228, 0.95);
	ojos.ojo1.ojo.brillo = 12.8;

	ojos.ojo2.ojo.calcular_normales_esfera();
	ojos.ojo2.ojo.ambiente = _vertex4f(0.135,0.2225, 0.1575, 0.82);
	ojos.ojo2.ojo.difuso = _vertex4f(0.54, 0.89, 0.63, 0.95);
	ojos.ojo2.ojo.especular = _vertex4f(0.316228, 0.316228, 0.316228, 0.95);
	ojos.ojo2.ojo.brillo = 12.8;

	ruedas.rueda1.rueda.calcular_normales_esfera();
	ruedas.rueda1.rueda.ambiente = _vertex4f(0,0, 0,1.0);
	ruedas.rueda1.rueda.difuso = _vertex4f(0.01, 0.01, 0.01, 1.0);
	ruedas.rueda1.rueda.especular = _vertex4f(0.5, 0.5,0.5, 1.0);
	ruedas.rueda1.rueda.brillo = 32;

	ruedas.rueda2.rueda.calcular_normales_esfera();
	ruedas.rueda2.rueda.ambiente = _vertex4f(0,0, 0,1.0);
	ruedas.rueda2.rueda.difuso = _vertex4f(0.01, 0.01, 0.01, 1.0);
	ruedas.rueda2.rueda.especular = _vertex4f(0.5, 0.5,0.5, 1.0);
	ruedas.rueda2.rueda.brillo = 32;

	


	
}
 
void _wall_e::draw(_modo modo, float r, float g, float b, float grosor)
{

	glPushMatrix();

	glTranslatef(0.0,0.0,pos_wall_e);
	
	ruedas.draw(modo,r,g,b,grosor);
	
	glRotatef(giro_cuerpo,1.0,0.0,0.0);
	cuerpo.draw(modo,r,g,b,grosor);

	glTranslatef(0,0,movimiento_brazos);
	brazos.draw(modo,r,g,b,grosor);
	glTranslatef(0,0,-movimiento_brazos);

	glTranslatef(0.0,cuerpo.alto+cuello.alto,0.0);
	glRotatef(giro_cuello,0.0,1.0,0.0);
	cuello.draw(modo,r,g,b,grosor);	

	glTranslatef(0.0,cuello.alto,-cuello.alto);
	glRotatef(giro_ojos,1.0,0.0,0.0);
	ojos.draw(modo,r,g,b,grosor);

	glPopMatrix();
}