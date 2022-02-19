 /**
 * @file mapping.c
 * @author Mateus L.
 * @author Ricardo D.
 * @date 2021/4
 */

#include <Trace/mapping.h>
#include <stdlib.h>
#include <math.h>
#include <Peripheral/Config.h>

uint8_t section_size;

/**
 * Resumo: Apaga os pontos salvos
 */
void clear_points(void)
{

	for (int i = 0; i < 6000 - 1; i++)
	{
		points[i].x = 0;
		points[i].y = 0;
		points[i].ang = 0;
		points[i].number_of_section = 0;
		points[i].final = 0;
	}

}

/**
 * Resumo: Apaga as se��es salvas
 */
void clear_sections(void)
{

	for (int i = 0; i < 255 - 1; i++)
	{
		sections[i].radius = 0;
		sections[i].distance = 0;
		sections[i].max_vel_ang = 0;
		sections[i].max_vel_lin = 0;
		sections[i].final_vel_ang = 0;
		sections[i].final_vel_lin = 0;
	}

}

/**
 * Resumo: Adiciona um ponto da pista a estrutura
 * 1� par�metro ponto x em m.
 * 2� par�metro ponto y em m.
 * 3� par�metro angula��o que o rob� est� apontando em deg.
 * 4� par�metro se��o de onde esse ponto pertence (marca��es da esquerda).
 * 5� par�metro salva se � a ultima se��o.
 */
void add_path(float x, float y, float ang, uint8_t section, bool final,
		int point)
{

	points[point].x = x;
	points[point].y = y;
	points[point].ang = ang;
	points[point].number_of_section = section;
	points[point].final = final;

	if (final == 1)
	{
		section_size = section;
	}

}

/**
 * Resumo: Adiciona as caracteristicas da se��o a estrutura
 * 1� par�metro raio em m.
 * 2� par�metro tamanho da se��o em m.
 * 3� par�metro se��o de onde essas caracteristicas salvas pertencem (marca��es da esquerda).
 */
void add_section_size(float radius, float distance, uint8_t section)
{

	sections[section].radius = radius;
	sections[section].distance = distance;

}

/**
 * Resumo: Adiciona as caracteristicas da se��o a estrutura
 * 1� par�metro velocidade linear maxima da se��o em m/s.
 * 2� par�metro velocidade angular maxima da se��o em deg/s.
 * 3� par�metro se��o de onde essas caracteristicas salvas pertencem (marca��es da esquerda).
 */
void add_section_vel(float max_vel_lin, float max_vel_ang, float final_vel_lin,
		float final_vel_ang, uint8_t section)
{

	sections[section].max_vel_ang = max_vel_ang;
	sections[section].max_vel_lin = max_vel_lin;
	sections[section].final_vel_ang = final_vel_ang;
	sections[section].final_vel_lin = final_vel_lin;

}

/**
 * Resumo: Calcula o raio e o tamanho da se��o
 * 1� par�metro se��o que deve ser calculada.
 */
void update_step_map(uint8_t section)
{

	float x1, x2;
	float y1, y2;
	float ang;
	float distance = 0;
	float radius;
	float ang_t = 0;
	float ang_rad;

	for (int i = 0; i < 6000 - 1; i++)
	{
		if (points[i].number_of_section == section)
		{
			if (points[i + 1].number_of_section == section)
			{
				x1 = points[i].x;
				y1 = points[i].y;
				ang = points[i].ang;

				x2 = points[i + 1].x;
				y2 = points[i + 1].y;

				distance += sqrt((x2 - x1) * (x2 - x1))
						+ ((y2 - y1) * (y2 - y1));
				ang_t += abs(ang);
			}
		}
	}
	ang_rad = (ang_t * PI) / 180;
	radius = distance / ang_rad;
	add_section_size(radius, distance, section);
}

uint8_t get_section_size(void)
{
	return section_size;
}
