#ifndef __MAPPING_H
#define __MAPPING_H

#include <stdint.h>
#include <stdbool.h>

static typedef struct st_points
{
	float x;
	float y;
	float ang;
	uint8_t number_of_section;
	bool final;
} Point;
Point points[6000];

static typedef struct st_section
{
	float radius;
	float distance;
	float max_vel_ang;
	float max_vel_lin;
	float final_vel_ang;
	float final_vel_lin;
} Section;
Section sections[255];

void clear_points();
void clear_sections();
void add_path(float x, float y, float ang, uint8_t section, bool final, int i);
void add_section_size(float radius, float distance, uint8_t section);
void update_step_map(uint8_t section);
uint8_t get_section_size(void);

#endif /* __MAPPING_H */

