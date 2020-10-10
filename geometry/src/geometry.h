#pragma once

struct Point {
 public:
  double x;
  double y;

  Point(double x, double y) : x{x}, y{y} {}

  double getX() const { return x; }

  double getY() const { return y; }

  bool operator==(const Point &a) const {
    if (a.getX() == x && a.getY() == y) {
      return true;
    }
    return false;
  }

  bool operator!=(const Point &a) const {
    if (a == *this) {
      return false;
    }
    return true;
  }
};

class Line {
 public:
  Line(Point a, Point b) {
    angle_ = (b.getX() - a.getX()) / (b.getY() - a.getY());
    bias_ = a.getY() - a.getX() * angle_;
  }

  Line(double a, double b) : angle_(a), bias_(b) {}

  Line(Point a, double angle) {
    angle_ = angle_;
    bias_ = a.getY() - a.getX() * angle_;
  }

  double getAngle() const { return angle_; }

  double getBias() const { return bias_; }

  bool operator==(const Line &a) const {
    if (a.getAngle() == angle_ && a.getBias() == bias_) {
      return true;
    }
    return false;
  }

  bool operator!=(const Line &a) const {
    if (a == *this) {
      return false;
    }
    return true;
  }

 private:
  double angle_;
  double bias_;
};

class Shape {
 public:
  virtual Shape() {}
};