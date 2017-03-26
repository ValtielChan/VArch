#ifndef HEIGHTMAPFILTER_H
#define HEIGHTMAPFILTER_H

class HeightMap;

class HeightMapFilter
{
public:
	HeightMapFilter();
	~HeightMapFilter();

	static void binarize(HeightMap* hm, float threshold);

private:

};

#endif // !HEIGHTMAPFILTER_H