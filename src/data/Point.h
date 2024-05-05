#pragma once


class Point{
  public:
    int x;
    int y;
  Point(int x, int y): x(x), y(y){};
  Point(): x(0), y(0){};

  Point& operator=(const Point& other){
    x = other.x;
    y = other.y;
    return *this;
  }
};