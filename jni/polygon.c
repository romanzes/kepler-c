#include "math.h"
#include "float.h"
#include "polygon.h"

int polyContainsV(Vector2 *polygon, int offset, int count, Vector2 *vec) {
	int intersects = 0;
	int limit = offset + count;
	int i, j;
	for (i = limit - 1, j = offset; j < limit; i = j++) {
		float x1 = polygon[i].x;
		float y1 = polygon[i].y;
		float x2 = polygon[j].x;
		float y2 = polygon[j].y;
		if (((y1 <= vec->y && vec->y < y2) || (y2 <= vec->y && vec->y < y1))
				&& vec->x < ((x2 - x1) / (y2 - y1) * (vec->y - y1) + x1)) intersects++;
	}
	return (intersects & 1) == 1;
}

int polygonsIntersect(Vector2 *polygon1, int offset1, int count1, Vector2 *polygon2, int offset2, int count2) {
	float overlap = FLT_MAX;
	float smallestAxisX = 0;
	float smallestAxisY = 0;

	int end1 = offset1 + count1;
	int end2 = offset2 + count2;

	// Get polygon1 axes
	int i;
	for (i = offset1; i < end1; i++) {
		float x1 = polygon1[i].x;
		float y1 = polygon1[i].y;
		float x2 = polygon1[(i + 1) % count1].x;
		float y2 = polygon1[(i + 1) % count1].y;

		float axisX = y1 - y2;
		float axisY = -(x1 - x2);

		float length = sqrt(axisX * axisX + axisY * axisY);
		axisX /= length;
		axisY /= length;

		// -- Begin check for separation on this axis --//

		// Project polygon1 onto this axis
		float min1 = axisX * polygon1[0].x + axisY * polygon1[0].x;
		float max1 = min1;
		int j;
		for (j = offset1; j < end1; j++) {
			float p = axisX * polygon1[j].x + axisY * polygon1[j].y;
			if (p < min1) {
				min1 = p;
			} else if (p > max1) {
				max1 = p;
			}
		}

		// Project polygon2 onto this axis
		float min2 = axisX * polygon2[0].x + axisY * polygon2[0].y;
		float max2 = min2;
		for (j = offset2; j < end2; j++) {
			float p = axisX * polygon2[j].x + axisY * polygon2[j].y;
			if (p < min2) {
				min2 = p;
			} else if (p > max2) {
				max2 = p;
			}
		}

		if (!((min1 <= min2 && max1 >= min2) || (min2 <= min1 && max2 >= min1))) {
			return 0;
		} else {
			float o = fmin(max1, max2) - fmax(min1, min2);
			if ((min1 < min2 && max1 > max2) || (min2 < min1 && max2 > max1)) {
				float mins = fabs(min1 - min2);
				float maxs = fabs(max1 - max2);
				if (mins < maxs) {
					axisX = -axisX;
					axisY = -axisY;
					o += mins;
				} else {
					o += maxs;
				}
			}
			if (o < overlap) {
				overlap = o;
				smallestAxisX = axisX;
				smallestAxisY = axisY;
			}
		}
		// -- End check for separation on this axis --//
	}

	// Get polygon2 axes
	for (i = offset2; i < end2; i++) {
		float x1 = polygon2[i].x;
		float y1 = polygon2[i].y;
		float x2 = polygon2[(i + 1) % count2].x;
		float y2 = polygon2[(i + 1) % count2].y;

		float axisX = y1 - y2;
		float axisY = -(x1 - x2);

		float length = sqrt(axisX * axisX + axisY * axisY);
		axisX /= length;
		axisY /= length;

		// -- Begin check for separation on this axis --//

		// Project polygon1 onto this axis
		float min1 = axisX * polygon1[0].x + axisY * polygon1[0].y;
		float max1 = min1;
		int j;
		for (j = offset1; j < end1; j++) {
			float p = axisX * polygon1[j].x + axisY * polygon1[j].y;
			if (p < min1) {
				min1 = p;
			} else if (p > max1) {
				max1 = p;
			}
		}

		// Project polygon2 onto this axis
		float min2 = axisX * polygon2[0].x + axisY * polygon2[0].y;
		float max2 = min2;
		for (j = offset2; j < end2; j++) {
			float p = axisX * polygon2[j].x + axisY * polygon2[j].y;
			if (p < min2) {
				min2 = p;
			} else if (p > max2) {
				max2 = p;
			}
		}

		if (!((min1 <= min2 && max1 >= min2) || (min2 <= min1 && max2 >= min1))) {
			return 0;
		} else {
			float o = fmin(max1, max2) - fmax(min1, min2);

			if ((min1 < min2 && max1 > max2) || (min2 < min1 && max2 > max1)) {
				float mins = fabs(min1 - min2);
				float maxs = fabs(max1 - max2);
				if (mins < maxs) {
					axisX = -axisX;
					axisY = -axisY;
					o += mins;
				} else {
					o += maxs;
				}
			}

			if (o < overlap) {
				overlap = o;
				smallestAxisX = axisX;
				smallestAxisY = axisY;
			}
		}
		// -- End check for separation on this axis --//
	}
	return 1;
}
