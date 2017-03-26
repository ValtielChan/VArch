#include "HeightMapFilter.h"

#include "HeightMap.h"

HeightMapFilter::HeightMapFilter()
{
}

HeightMapFilter::~HeightMapFilter()
{
}

void HeightMapFilter::binarize(HeightMap* hm, float threshold)
{
	for (int x = 0; x < hm->width(); x++) {
		for (int y = 0; y < hm->height(); y++) {

			if (hm->get(x, y) < threshold)
				hm->set(x, y, 0);
		}
	}
}