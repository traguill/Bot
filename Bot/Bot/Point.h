#ifndef __POINT_H__
#define __POINT_H__

template<class TYPE>
class Point
{
public:

	Point<TYPE>() : x(0), y(0)
	{}

	Point<TYPE>(TYPE x, TYPE y) : x(x), y(y)
	{}

	Point<TYPE> operator + (const Point<TYPE>& p2)const
	{
		Point<TYPE> p;
		p.x = this->x + p2.x;
		p.y = this->y + p2.y;

		return p;
	}
	
	Point<TYPE> operator - (const Point<TYPE>& p2)const
	{
		Point<TYPE> p;
		p.x = this->x - p2.x;
		p.y = this->y - p2.y;

		return p;
	}

	const Point<TYPE>& operator += (const Point<TYPE>& p2)
	{
		x += p2.x;
		y += p2.y;

		return (*this);
	}

	const Point<TYPE>& operator -= (const Point<TYPE>& p2)
	{
		x -= p2.x;
		y -= p2.y;

		return (*this);
	}

	Point<TYPE> operator * (const TYPE& u)const
	{
		Point<TYPE> p;
		p.x = this->x * u;
		p.y = this->y * u;

		return p;
	}

	int Lengthi()const
	{
		return sqrt(x*x + y*y);
	}

	float Lengthf()const
	{
		return sqrt(x*x + y*y);
	}

	TYPE DistanceTo(const Point<TYPE>& p2)const
	{
		TYPE w = p2.x - x;
		TYPE h = p2.y - y;

		return sqrt(w*w + h*h);
	}

public:
	TYPE x;
	TYPE y;
};

#endif // !__POINT_H__



