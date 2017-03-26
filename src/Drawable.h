#ifndef DRAWABLE_H
#define DRAWABLE_H

class Drawable {

public:

	Drawable() {}
	~Drawable() {}

	virtual void draw() = 0;
	void setMVPUniform();

};

#endif // DRAWABLE_H