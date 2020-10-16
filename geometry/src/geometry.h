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
    std::vector<Point> new_vertices(vertices_.size());
    for (int i = 0; i < vertices_.size(); ++i) {
      new_vertices[i] = vertices_[i].rotatePoint(center, angle);
    }
    vertices_ = new_vertices;
  }

  void reflex(Point center) override {
    std::vector<Point> new_vertices(vertices_.size());
    for (int i = 0; i < vertices_.size(); ++i) {
      new_vertices[i] = vertices_[i].reflexPoint(center);
    }
    vertices_ = new_vertices;
  }

  void reflex(Line axis) override {
    auto slope = -axis.getAngle();
    std::vector<Point> new_vertices(vertices_.size());
    for (int i = 0; i < vertices_.size(); ++i) {
      new_vertices[i] = axis.reflexLine(vertices_[i]);
    }
    vertices_ = new_vertices;
  }

  void scale(Point center, double coefficient) override {
    std::vector<Point> new_vertices(vertices_.size());
    for (int i = 0; i < vertices_.size(); ++i) {
      new_vertices[i] = vertices_[i].scalePoint(center, coefficient);
    }
    vertices_ = new_vertices;
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

  Point center() { return a_; }

 protected:
  Point a_;
  double radius_;
};

class Rectangle : public Polygon {
 public:
  Rectangle() {}
  Rectangle(Point a, Point b, double r) {
    a_ = a;
    b_ = b;
    vertices_.push_back(a_);
    vertices_.push_back(b_);
    ratio_ = r;
    d1_ = Line(a_, b_);
    d2_ = Line(-d1_.getAngle(), d1_.getBias());
    auto length1 = a_.dist(b_) / sqrt(pow(ratio_, 2) + 1);
    auto length2 = length1 * ratio_;
    min_length_ = std::min(length1, length2);
    max_length_ = std::max(length1, length2);
  }

  Point center() { return a_.getCenter(b_); }

  std::pair<Line, Line> diagonals() { return std::pair<Line, Line>(d1_, d2_); }

  double perimeter() { return 2 * (min_length_ + max_length_); }

  double area() { return min_length_ * max_length_; }

  void rotate(Point center, double angle) {
    a_ = a_.rotatePoint(center, angle);
    b_ = b_.rotatePoint(center, angle);
    d1_ = Line(a_, b_);
    d2_ = Line(-d1_.getAngle(), d1_.getBias());
  }

  void reflex(Point center) {
    a_ = a_.reflexPoint(center);
    b_ = b_.reflexPoint(center);
    d1_ = Line(a_, b_);
    d2_ = Line(-d1_.getAngle(), d1_.getBias());
  }

  void reflex(Line axis) {
    a_ = axis.reflexLine(a_);
    b_ = axis.reflexLine(b_);
    d1_ = Line(a_, b_);
    d2_ = Line(-d1_.getAngle(), d1_.getBias());
  }

  void scale(Point center, double coefficient) {
    a_ = a_.scalePoint(center, coefficient);
    b_ = b_.scalePoint(center, coefficient);
    d1_ = Line(a_, b_);
    d2_ = Line(-d1_.getAngle(), d1_.getBias());
    min_length_ = coefficient * min_length_;
    max_length_ = coefficient * max_length_;
  }

  Point getVertice1() { return a_; }
  Point getVertice2() { return b_; }
  double getRatio() { return ratio_; }

 protected:
  Point a_;
  Point b_;
  double ratio_;
  double min_length_;
  double max_length_;
  Line d1_;
  Line d2_;
};

class Square : public Rectangle {
 public:
  Square(Point a, Point b) : a_(a), b_(b) {
    length_ = a_.dist(b_) / sqrt(2);
    vertices_.push_back(a_);
    vertices_.push_back(b_);
  }

  Circle circumscribedCircle() {
    Point center = a_.getCenter(b_);
    auto rad = a_.dist(b_);
    return Circle(center, rad);
  }

  Circle inscribedCircle() {
    Point center = a_.getCenter(b_);
    auto rad = a_.dist(b_) / sqrt(2);
    return Circle(center, rad);
  }

  double perimeter() { return 4 * length_; }

  double area() { return pow(length_, 2); }

  bool operator==(const Shape &another) override {
    Square *inp_shape = dynamic_cast<Square *>(const_cast<Shape *>(&another));
    if (inp_shape) {
      if (inp_shape->getVertice1() != a_ || inp_shape->getVertice2() != b_) {
        return false;
      }
      return true;
    }
    return false;
  }

  void rotate(Point center, double angle) {
    a_ = a_.rotatePoint(center, angle);
    b_ = b_.rotatePoint(center, angle);
  }

  void reflex(Point center) {
    a_ = a_.reflexPoint(center);
    b_ = b_.reflexPoint(center);
  }

  void reflex(Line axis) {
    a_ = axis.reflexLine(a_);
    b_ = axis.reflexLine(b_);
  }

  void scale(Point center, double coefficient) {
    a_ = a_.scalePoint(center, coefficient);
    b_ = b_.scalePoint(center, coefficient);
    length_ = coefficient * length_;
  }
  Point getVertice1() { return a_; }
  Point getVertice2() { return b_; }
  double getRatio() { return ratio_; }

 protected:
  Point a_;
  Point b_;
  double length_;
  double ratio_ = 1;
};

class Triangle : public Polygon {
 public:
  Triangle(Point a, Point b, Point c) : a_(a), b_(b), c_(c) {
    c_len_ = a_.dist(b_);
    b_len_ = a_.dist(c_);
    a_len_ = b_.dist(c_);
  }

  double circumCenterHelper(double a, double b, double c) {
    auto a_part1 = pow(a, 2) * (pow(b, 2) + pow(c, 2) - pow(a, 2));
    auto a_part2 = (pow(b + c, 2) - pow(a, 2)) * (pow(a, 2) - pow(b - c, 2));
    return a_part1 / a_part2;
  }

  Point getCircumcenter() {
    auto abc = circumCenterHelper(a_len_, b_len_, c_len_);
    auto bac = circumCenterHelper(b_len_, a_len_, c_len_);
    auto cab = circumCenterHelper(c_len_, a_len_, b_len_);
    auto x = a_.getX() * abc + b_.getX() * bac + c_.getX() * cab;
    auto y = a_.getY() * abc + b_.getY() * bac + c_.getY() * cab;
    return Point(x, y);
  }

  Circle circumscribedCircle() {
    Point center = getCircumcenter();
    double radius = center.dist(c_);
    return Circle(center, radius);
  }

  Point getIncenter() {
    auto p = perimeter();
    auto x = (a_len_ * a_.getX() + b_len_ * b_.getX() + c_len_ * c_.getX()) / p;
    auto y = (a_len_ * a_.getY() + b_len_ * b_.getY() + c_len_ * c_.getY()) / p;
    return Point(x, y);
  }

  Circle inscribedCircle() {
    Point center = getIncenter();
    Point touch = Line(a_, c_).getProjection(center);
    double radius = center.dist(touch);
    return Circle(center, radius);
  }

  Point centroid() {
    auto x = (a_.getX() + b_.getX() + c_.getX()) / 3;
    auto y = (a_.getY() + b_.getY() + c_.getY()) / 3;
    return Point(x, y);
  }

  double getPerp(Point a, Point b) { return -1 / b.xDist(a); }

  Point orthocenter() {
    double cf = getPerp(a_, b_);
    double ad = getPerp(b_, c_);
    auto x =
        (a_.getY() - c_.getY() + cf * c_.getX() - ad * a_.getX()) / (cf - ad);
    auto y = cf * (x - c_.getX()) + c_.getY();
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
      if (inp_shape->getA() == a_ && inp_shape->getB() == b_ &&
          inp_shape->getC() == c_) {
        return true;
      }
    }
    return false;
  }

  void rotate(Point center, double angle) {
    a_ = a_.rotatePoint(center, angle);
    b_ = b_.rotatePoint(center, angle);
    c_ = c_.rotatePoint(center, angle);
  }

  void reflex(Point center) {
    a_ = a_.reflexPoint(center);
    b_ = b_.reflexPoint(center);
    c_ = c_.reflexPoint(center);
  }

  void reflex(Line axis) {
    a_ = axis.reflexLine(a_);
    b_ = axis.reflexLine(b_);
    c_ = axis.reflexLine(c_);
  }

  void scale(Point center, double coefficient) {
    a_ = a_.scalePoint(center, coefficient);
    b_ = b_.scalePoint(center, coefficient);
    c_ = c_.scalePoint(center, coefficient);
  }
  Point getA() { return a_; }
  Point getB() { return b_; }
  Point getC() { return c_; }

 protected:
  Point a_;
  Point b_;
  Point c_;
  double c_len_;
  double b_len_;
  double a_len_;
};
