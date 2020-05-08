#ifndef LAPS
#define LAPS value

#include <stdlib.h>

/**
 * struct car_t - defines a car
 *
 * @id: id of the car.
 * @laps: laps of the car.
 */
typedef struct car_t
{
	int id;
	int laps;
} Car;

void race_state(int *id, size_t size);
void new_car(int *ids, int *laps, int id, int size);

#endif /* ifndef LAPS */
