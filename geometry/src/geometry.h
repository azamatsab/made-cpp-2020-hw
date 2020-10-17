#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

#include "cmath"

const double EPS = 1e-12;

struct Point {
 public:
  double x;
  double y;

  Point() {}

  Point(double x, double y) : x(x), y(y) {}

  double getX() const { return x; }

  double getY() const { return y; }

  bool operator==(const Point &a) const {
    if ((a.getX() - x) <= EPS && (a.getY() - y) <= EPS) {
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

  double xDist(Point b) { return x - b.getX(); }

  double yDist(Point b) { return y - b.getY(); }

  double xNorm1(Point b) { return fabs(xDist(b)); }

  double yNorm1(Point b) { return fabs(yDist(b)); }

  Point reflexPoint(Point center) {
    auto xx = -xDist(center);
    auto yy = -yDist(center);
    return Point(center.getX() + xx, center.getY() + yy);
  }

  Point scalePoint(Point center, double coefficient) {
    auto x = xDist(center);
    auto y = yDist(center);
    return Point(center.getX() + coefficient * x,
                 center.getY() + coefficient * y);
  }

  double crossSumSubs(Point b) { return x * b.getY() - y * b.getX(); }

  double dist(Point b) {
    auto d1 = xNorm1(b);
    auto d2 = yNorm1(b);
    return sqrt(pow(d1, 2) + pow(d2, 2));
  }

  Point getCenter(Point b) {
    auto xx = (x + b.getX()) / 2;
    auto yy = (y + b.getY()) / 2;
    return Point(xx, yy);
  }

  double rotateX(Point c, double angle) {
    Point temp = Point(xDist(c), yDist(c));
    return temp.getX() * cos(angle) - temp.getY() * sin(angle);
  }

  double rotateY(Point c, double angle) {
    Point temp = Point(xDist(c), yDist(c));
    return temp.getX() * sin(angle) + temp.getY() * cos(angle);
  }

  Point rotatePoint(Point c, double angle) {
    auto x = rotateX(c, angle);
    auto y = rotateY(c, angle);
    return Point(x, y);
  }
};

class Line {
 public:
  Line() {}

  Line(Point a, Point b) {
    angle_ = (b.yDist(a)) / (b.xDist(a) + EPS);
    bias_ = a.getY() - a.getX() * angle_;
    a_ = a;
    b_ = b;
  }

  Line(double a, double b) : angle_(a), bias_(b) {}

  Line(Point a, double angle) {
    angle_ = angle;
    bias_ = a.getY() - a.getX() * angle_;
  }

  double getAngle() const { return angle_; }

  double getBias() const { return bias_; }

  bool operator==(const Line &a) const {
    if (a.getAngle() - angle_ <= EPS && a.getBias() - bias_ <= EPS) {
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

  Point getProjection(Point center) {
    auto a = -angle_;
    auto c = -bias_;
    double b = 1;
    auto x = (b * (b * center.getX() - a * center.getY()) - a * c) /
             (pow(a, 2) + pow(b, 2));
    auto y = (a * (-b * center.getX() + a * center.getY()) - b * c) /
             (pow(a, 2) + pow(b, 2));
    return Point(x, y);
  }

  Point reflexLine(Point a) {
    Point center = getProjection(a);
    return a.reflexPoint(center);
  }

 private:
  double angle_;
  double bias_;
  Point a_;
  Point b_;
};

class Shape {
 public:
  Shape() {}

  virtual double perimeter() = 0;

  virtual double area() = 0;

  virtual bool operator==(const Shape &another) = 0;

  virtual void rotate(Point center, double angle) = 0;

  virtual void reflex(Point center) = 0;

  virtual void reflex(Line axis) = 0;

  virtual void scale(Point center, double coefficient) = 0;
};

class Polygon : public Shape {
 public:
  Polygon() {}

  Polygon(std::vector<Point> vertices) { vertices_ = vertices; }

  int verticesCount() { return vertices_.size(); }

  std::vector<Point> getVertices() { return vertices_; }

  double perimeter() override {
    double result = 0;
    for (int i = 0; i < vertices_.size(); ++i) {
      if (i + 1 == vertices_.size()) {
        result += vertices_[i].dist(vertices_[0]);
      } else {
        result += vertices_[i].dist(vertices_[i + 1]);
      }
    }
    return result;
  }

  double area() override {
    double result = 0;
    for (int i = 0; i < vertices_.size(); ++i) {
      if (i + 1 == vertices_.size()) {
        result += vertices_[i].crossSumSubs(vertices_[0]);
      } else {
        result += vertices_[i].crossSumSubs(vertices_[i + 1]);
      }
    }
    return fabs(result / 2);
  }

  bool operator==(const Shape &another) override {
    Polygon *inp_shape = dynamic_cast<Polygon *>(const_cast<Shape *>(&another));
    if (inp_shape) {
      if (inp_shape->getVertices().size() != vertices_.size()) {
        return false;
      }
      for (auto a : inp_shape->getVertices()) {
        bool found = false;
        for (auto b : vertices_) {
          if (a == b) {
            found = true;
            break;
          }
        }
        if (!found) {
          return false;
        }
      }
      return true;
    }
    return false;
  }

  bool operator!=(const Shape &another) {
    if (*this == another) {
      return false;
    }
    return true;
  }

  void rotate(Point center, double angle) override {
    for (int i = 0; i < vertices_.size(); ++i) {
      vertices_[i] = vertices_[i].rotatePoint(center, angle);
    }
  }

  void reflex(Point center) override {
    for (int i = 0; i < vertices_.size(); ++i) {
      vertices_[i] = vertices_[i].reflexPoint(center);
    }
  }

  void reflex(Line axis) override {
    auto slope = -axis.getAngle();
    for (int i = 0; i < vertices_.size(); ++i) {
      vertices_[i] = axis.reflexLine(vertices_[i]);
    }
  }

  void scale(Point center, double coefficient) override {
    for (int i = 0; i < vertices_.size(); ++i) {
      vertices_[i] = vertices_[i].scalePoint(center, coefficient);
    }
  }

 protected:
  std::vector<Point> vertices_;
};

class Ellipse : public Shape {
 public:
  Ellipse() {}
  Ellipse(Point focus1, Point focus2, double distance)
      : focus1_(focus1), focus2_(focus2), distance_(distance) {
    a_ = distance_ / 2;
    c_ = focus1_.dist(focus2_) / 2;
    b_ = sqrt(pow(a_, 2) - pow(c_, 2));
  }

  std::pair<Point, Point> focuses() {
    return std::pair<Point, Point>(focus1_, focus2_);
  }

  double eccentricity() { return c_ / a_; }

  Point center() { return focus1_.getCenter(focus2_); }

  double perimeter() {
    auto h = pow((a_ - b_), 2) / pow((a_ + b_), 2);
    return M_PI * (a_ + b_) * (1 + 3 * h / (10 + sqrt(4 - 3 * h)));
  }

  double area() { return M_PI * a_ * b_; }

  bool operator==(const Shape &another) override {
    Ellipse *inp_shape = dynamic_cast<Ellipse *>(const_cast<Shape *>(&another));
    if (inp_shape) {
      if (inp_shape->getFocus1() != focus1_ ||
          inp_shape->getFocus2() != focus2_ ||
          inp_shape->getDistance() != distance_) {
        return false;
      }
      return true;
    }
    return false;
  }

  void rotate(Point center, double angle) {
    focus1_ = focus1_.rotatePoint(center, angle);
    focus2_ = focus2_.rotatePoint(center, angle);
  }

  void reflex(Point center) {
    focus1_ = focus1_.reflexPoint(center);
    focus2_ = focus2_.reflexPoint(center);
  }

  void reflex(Line axis) {
    focus1_ = axis.reflexLine(focus1_);
    focus2_ = axis.reflexLine(focus2_);
  }

  void scale(Point center, double coefficient) {
    focus1_ = focus1_.scalePoint(center, coefficient);
    focus2_ = focus1_.scalePoint(center, coefficient);
    distance_ = coefficient * distance_;
    a_ = coefficient * a_;
    b_ = coefficient * b_;
    c_ = coefficient * c_;
  }

  Point getFocus1() { return focus1_; }
  Point getFocus2() { return focus2_; }
  double getDistance() { return distance_; }

 protected:
  Point focus1_;
  Point focus2_;
  double distance_;
  double a_;
  double b_;
  double c_;
};

class Circle : public Ellipse {
 public:
  Circle(Point o, double r) : a_(o), radius_(r) {}

  double radius() { return radius_; }

  Point center() { return a_; }

  double perimeter() { return 2 * M_PI * radius_; }

  double area() { return M_PI * pow(radius_, 2); }

  bool operator==(const Shape &another) override {
    Circle *inp_shape = dynamic_cast<Circle *>(const_cast<Shape *>(&another));
    if (inp_shape) {
      if (inp_shape->radius() == radius_ && inp_shape->center() == a_) {
        return true;
      }
    }
    return false;
  }

  void rotate(Point center, double angle) {
    a_ = a_.rotatePoint(center, angle);
  }

  void reflex(Point center) { a_ = a_.reflexPoint(center); }

  void reflex(Line axis) { a_ = axis.reflexLine(a_); }

  void scale(Point center, double coefficient) {
    a_ = a_.scalePoint(center, coefficient);
    radius_ = coefficient * radius_;
  }

 protected:
  Point a_;
  double radius_;
};

class Rectangle : public Polygon {
 public:
  Rectangle() {}
  Rectangle(Point a, Point b, double r) {
    vertices_.push_back(a);
    vertices_.push_back(b);
    auto length1 = a.dist(b) / sqrt(pow(r, 2) + 1);
    auto length2 = length1 * r;
    min_length_ = std::min(length1, length2);
    max_length_ = std::max(length1, length2);
  }

  Point center() { return getA().getCenter(getB()); }

  virtual std::pair<Line, Line> diagonals() {
    Line d1 = Line(getA(), getB());
    Point center = getA().getCenter(getB());
    double angle = d1.getAngle();
    if (max_length_ - min_length_ <= EPS) {
      angle = -angle;
    }
    Line d2 = Line(center, 1 / (angle + EPS));
    return std::pair<Line, Line>(d1, d2);
  }

  double perimeter() { return 2 * (min_length_ + max_length_); }

  double area() { return min_length_ * max_length_; }

  bool operator==(const Shape &another) override {
    if (Polygon::operator==(another)) {
      Rectangle *inp_shape =
          dynamic_cast<Rectangle *>(const_cast<Shape *>(&another));
      auto d1 = std::get<0>(diagonals());
      auto d2 = std::get<1>(diagonals());
      auto id1 = std::get<0>(inp_shape->diagonals());
      auto id2 = std::get<1>(inp_shape->diagonals());
      if ((d1 == id1 || d1 == id2) && (d2 == id1 || d2 == id2)) {
        return true;
      }
    }
    return false;
  }

  void scale(Point center, double coefficient) {
    Polygon::scale(center, coefficient);
    min_length_ = coefficient * min_length_;
    max_length_ = coefficient * max_length_;
  }

  Point getA() { return vertices_[0]; }

  Point getB() { return vertices_[1]; }
  
  double getMinLength() { return min_length_; }

  double getMaxLength() { return max_length_; }

 protected:
  double min_length_;
  double max_length_;
};

class Square : public Rectangle {
 public:
  Square(Point a, Point b) {
    vertices_.push_back(a);
    vertices_.push_back(b);
    length_ = a.dist(b) / sqrt(2);
  }

  Circle circumscribedCircle() {
    Point center = getA().getCenter(getB());
    auto rad = getA().dist(getB());
    return Circle(center, rad);
  }

  Circle inscribedCircle() {
    Point center = getA().getCenter(getB());
    auto rad = getA().dist(getB()) / sqrt(2);
    return Circle(center, rad);
  }

  std::pair<Line, Line> diagonals() override {
    Line d1 = Line(getA(), getB());
    Point center = getA().getCenter(getB());
    double angle = -d1.getAngle();
    Line d2 = Line(center, angle);
    return std::pair<Line, Line>(d1, d2);
  }

  double perimeter() { return 4 * length_; }

  double area() { return pow(length_, 2); }

  void scale(Point center, double coefficient) {
    Polygon::scale(center, coefficient);
    length_ = coefficient * length_;
  }

 protected:
  double length_;
};

class Triangle : public Polygon {
 public:
  Triangle(Point a, Point b, Point c) {
    vertices_.push_back(a);
    vertices_.push_back(b);
    vertices_.push_back(c);
    c_len_ = a.dist(b);
    b_len_ = a.dist(c);
    a_len_ = b.dist(c);
  }

  Point getA() { return vertices_[0]; }
  Point getB() { return vertices_[1]; }
  Point getC() { return vertices_[2]; }

  double circumCenterHelper(double a, double b, double c) {
    auto a_part1 = pow(a, 2) * (pow(b, 2) + pow(c, 2) - pow(a, 2));
    auto a_part2 = (pow(b + c, 2) - pow(a, 2)) * (pow(a, 2) - pow(b - c, 2));
    return a_part1 / a_part2;
  }

  Point getCircumcenter() {
    auto abc = circumCenterHelper(a_len_, b_len_, c_len_);
    auto bac = circumCenterHelper(b_len_, a_len_, c_len_);
    auto cab = circumCenterHelper(c_len_, a_len_, b_len_);
    auto x = getA().getX() * abc + getB().getX() * bac + getC().getX() * cab;
    auto y = getA().getY() * abc + getB().getY() * bac + getC().getY() * cab;
    return Point(x, y);
  }

  Circle circumscribedCircle() {
    Point center = getCircumcenter();
    double radius = center.dist(getC());
    return Circle(center, radius);
  }

  Point getIncenter() {
    auto p = perimeter();
    auto x = (a_len_ * getA().getX() + b_len_ * getB().getX() +
              c_len_ * getC().getX()) /
             p;
    auto y = (a_len_ * getA().getY() + b_len_ * getB().getY() +
              c_len_ * getC().getY()) /
             p;
    return Point(x, y);
  }

  Circle inscribedCircle() {
    Point center = getIncenter();
    Point touch = Line(getA(), getC()).getProjection(center);
    double radius = center.dist(touch);
    return Circle(center, radius);
  }

  Point centroid() {
    auto x = (getA().getX() + getB().getX() + getC().getX()) / 3;
    auto y = (getA().getY() + getB().getY() + getC().getY()) / 3;
    return Point(x, y);
  }

  double getPerp(Point a, Point b) { return -1 / b.xDist(a); }

  Point orthocenter() {
    double cf = getPerp(getA(), getB());
    double ad = getPerp(getB(), getC());
    auto x = (getA().getY() - getC().getY() + cf * getC().getX() -
              ad * getA().getX()) /
             (cf - ad);
    auto y = cf * (x - getC().getX()) + getC().getY();
    return Point(x, y);
  }

  Line EulerLine() { return Line(ninePointsCircle().center(), centroid()); }

  Circle ninePointsCircle() {
    double radius = circumscribedCircle().radius() / 2;
    auto centr = getCircumcenter();
    auto orthoc = orthocenter();
    auto x = orthoc.xDist(centr) / 2 + centr.getX();
    auto y = orthoc.yDist(centr) / 2 + centr.getY();
    Point center = Point(x, y);
    return Circle(center, radius);
  }

  double perimeter() { return c_len_ + b_len_ + a_len_; }

  double area() override {
    auto s = perimeter() / 2;
    return sqrt(s * (s - a_len_) * (s - b_len_) * (s - c_len_));
  }

  bool operator==(const Shape &another) override {
    Triangle *inp_shape =
        dynamic_cast<Triangle *>(const_cast<Shape *>(&another));
    if (inp_shape) {
      if (inp_shape->getA() == getA() && inp_shape->getB() == getB() &&
          inp_shape->getC() == getC()) {
        return true;
      }
    }
    return false;
  }

 protected:
  double c_len_;
  double b_len_;
  double a_len_;
};
