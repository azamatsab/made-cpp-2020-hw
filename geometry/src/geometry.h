#pragma once
#include <cmath>

struct Point {
 public:
  double x;
  double y;
  
  Point(double x, double y): x{x}, y{y} {}

  double getX() const {
      return x;
  }

  double getY() const {
      return y;
  }

  bool operator==(const Point &a) {
      if (a.getX() == x && a.getY() == y) {
          return true;
      }
      return false;
  }

  bool operator!=(const Point &a) {
      if (a.getX() != x || a.getY() != y) {
          return true;
      }
      return false;
  }
};

class Line {
    double angle_;
    double bias_;
    
    Line(Point a, Point b) {
        angle_ = atan2((b.getY() - a.getY()), (b.getX() - a.getX()));
    }
    
    Line(double a, double b): angle_(a), bias_(b) {}
};