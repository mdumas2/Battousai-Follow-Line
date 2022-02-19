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
 * Resumo: Apaga as seções salvas
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
 * 1° parâmetro ponto x em m.
 * 2° parâmetro ponto y em m.
 * 3° parâmetro angulação que o robô está apontando em deg.
 * 4° parâmetro seção de onde esse ponto pertence (marcações da esquerda).
 * 5° parâmetro salva se é a ultima seção.
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
 * Resumo: Adiciona as caracteristicas da seção a estrutura
 * 1° parâmetro raio em m.
 * 2° parâmetro tamanho da seção em m.
 * 3° parâmetro seção de onde essas caracteristicas salvas pertencem (marcações da esquerda).
 */
void add_section_size(float radius, float distance, uint8_t section)
{

	sections[section].radius = radius;
	sections[section].distance = distance;

}

/**
 * Resumo: Adiciona as caracteristicas da seção a estrutura
 * 1° parâmetro velocidade linear maxima da seção em m/s.
 * 2° parâmetro velocidade angular maxima da seção em deg/s.
 * 3° parâmetro seção de onde essas caracteristicas salvas pertencem (marcações da esquerda).
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
 * Resumo: Calcula o raio e o tamanho da seção
 * 1° parâmetro seção que deve ser calculada.
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
