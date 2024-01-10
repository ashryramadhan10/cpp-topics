struct Point {
    int x;
    int y;
};

Point* createPoint(const int x, const int y) {
    Point* point = new Point;
    point->x = x;
    point->y = y;


    return point;
}

void deletePointObject(Point** point) {
    delete (*point);
}