#ifndef TEMPLATE_VECTOR2D_HEADER_FILE
#define TEMPLATE_VECTOR2D_HEADER_FILE

template <typename T>
class Vector2D {
	private:

	public:
		T x, y;

		Vector2D(): x(0), y(0) { }
		Vector2D(T x, T y): x(x), y(y) { }

		Vector2D<T> operator+(Vector2D<T> const &V) {
			return Vector2D<T>(x + V.x, y + V.y);
		}

		bool operator==(Vector2D<T> const &V) {
			return (x == V.x) && (y == V.y);
		}
};

#endif