#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

#include "cmath"


const double EPS = pow(10, -12);

struct Point {
 public:
  Point() {}
  double x;
  double y;

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
};

double x_dist(Point a, Point b) { return a.getX() - b.getX(); }

double y_dist(Point a, Point b) { return a.getY() - b.getY(); }

double x_1norm(Point a, Point b) { return fabs(x_dist(a, b)); }

double y_1norm(Point a, Point b) { return fabs(y_dist(a, b)); }

Point lin_equation(double a1, double b1, double a2, double b2) {
  auto x = (b2 - b1) / (a1 - a2);
  auto y = a2 * x + b2;
  return Point(x, y);
}

class Line {
 public:
  Line() {}

  Line(Point a, Point b) {
    angle_ = (y_dist(b, a)) / (x_dist(b, a) + EPS);
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

 private:
  double angle_;
  double bias_;
  Point a_;
  Point b_;
};

Point reflex_point(Point a, Point center) {
  auto x = x_dist(center, a);
  auto y = y_dist(center, a);
  return Point(center.getX() + x, center.getY() + y);
}

Point reflex_line(Line axis, Point a) {
  double slope = -axis.getAngle();
  double bias = a.getY() - slope * a.getX();
  Point center = lin_equation(axis.getAngle(), axis.getBias(), slope, bias);
  return reflex_point(a, center);
}

Point scale_point(Point a, Point center, double coefficient) {
  auto x = x_dist(a, center);
  auto y = y_dist(a, center);
  return Point(center.getX() + coefficient * x,
               center.getY() + coefficient * y);
}

double cross_sum_subs(Point a, Point b) {
  return a.getX() * b.getY() - a.getY() * b.getX();
}

double pow_sum_sqrt(Point a, Point b) {
  auto d1 = x_1norm(a, b);
  auto d2 = y_1norm(a, b);
  return sqrt(pow(d1, 2) + pow(d2, 2));
}

Point get_center(Point a, Point b) {
  auto x = (a.getX() + b.getX()) / 2;
  auto y = (a.getY() + b.getY()) / 2;
  return Point(x, y);
}

double rotate_x(Point a, Point c, double angle) {
  Point temp = Point(x_dist(a, c), y_dist(a, c));
  return temp.getX() * cos(angle) - temp.getY() * sin(angle);
}

double rotate_y(Point a, Point c, double angle) {
  Point temp = Point(x_dist(a, c), y_dist(a, c));
  return temp.getX() * sin(angle) + temp.getY() * cos(angle);
}

Point rotate_point(Point a, Point c, double angle) {
  auto x = rotate_x(a, c, angle);
  auto y = rotate_y(a, c, angle);
  return Point(x, y);
}

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

  //   Polygon(std::vector<Point> vertices) : vertices_(vertices) {}
  Polygon(std::vector<Point> vertices) { vertices_ = vertices; }

  int verticesCount() { return vertices_.size(); }

  std::vector<Point> getVertices() { return vertices_; }

  double perimeter() override {
    double result = 0;
    for (int i = 0; i < vertices_.size(); ++i) {
      if (i + 1 == vertices_.size()) {
        result += pow_sum_sqrt(vertices_[i], vertices_[0]);
      } else {
        result += pow_sum_sqrt(vertices_[i], vertices_[i + 1]);
      }
    }
    return result;
  }

  double area() override {
    double result = 0;
    for (int i = 0; i < vertices_.size(); ++i) {
      if (i + 1 == vertices_.size()) {
        result += cross_sum_subs(vertices_[i], vertices_[0]);
      } else {
        result += cross_sum_subs(vertices_[i], vertices_[i + 1]);
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
        //   if (std::find(vertices_.begin(), vertices_.end(), a) !=
        //   vertices_.end()) {
        //     return false;
        //   }

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
      new_vertices[i] = rotate_point(vertices_[i], center, angle);
    }
    vertices_ = new_vertices;
  }

  void reflex(Point center) override {
    std::vector<Point> new_vertices(vertices_.size());
    for (int i = 0; i < vertices_.size(); ++i) {
      new_vertices[i] = reflex_point(vertices_[i], center);
    }
    vertices_ = new_vertices;
  }

  void reflex(Line axis) override {
    auto slope = -axis.getAngle();
    std::vector<Point> new_vertices(vertices_.size());
    for (int i = 0; i < vertices_.size(); ++i) {
      new_vertices[i] = reflex_line(axis, vertices_[i]);
    }
    vertices_ = new_vertices;
  }

  void scale(Point center, double coefficient) override {
    std::vector<Point> new_vertices(vertices_.size());
    for (int i = 0; i < vertices_.size(); ++i) {
      new_vertices[i] = scale_point(vertices_[i], center, coefficient);
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
    c_ = pow_sum_sqrt(focus1_, focus2_) / 2;
    b_ = sqrt(pow(a_, 2) - pow(c_, 2));
  }

  std::pair<Point, Point> focuses() {
    return std::pair<Point, Point>(focus1_, focus2_);
  }

  double eccentricity() { return c_ / a_; }

  Point center() { return get_center(focus1_, focus2_); }

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
    focus1_ = rotate_point(focus1_, center, angle);
    focus2_ = rotate_point(focus2_, center, angle);
  }

  void reflex(Point center) {
    focus1_ = reflex_point(focus1_, center);
    focus2_ = reflex_point(focus2_, center);
  }

  void reflex(Line axis) {
    focus1_ = reflex_line(axis, focus1_);
    focus2_ = reflex_line(axis, focus2_);
  }

  void scale(Point center, double coefficient) {
    focus1_ = scale_point(focus1_, center, coefficient);
    focus2_ = scale_point(focus2_, center, coefficient);
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
    a_ = rotate_point(a_, center, angle);
  }

  void reflex(Point center) { a_ = reflex_point(a_, center); }

  void reflex(Line axis) { a_ = reflex_line(axis, a_); }

  void scale(Point center, double coefficient) {
    a_ = scale_point(a_, center, coefficient);
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
    auto length1 = pow_sum_sqrt(a_, b_) / sqrt(pow(ratio_, 2) + 1);
    auto length2 = length1 * ratio_;
    min_length_ = std::min(length1, length2);
    max_length_ = std::max(length1, length2);
  }

  Point center() { return get_center(a_, b_); }

  std::pair<Line, Line> diagonals() { return std::pair<Line, Line>(d1_, d2_); }

  double perimeter() { return 2 * (min_length_ + max_length_); }

  double area() { return min_length_ * max_length_; }

  // bool operator==(const Shape &another) override {
  //   Rectangle *inp_shape =
  //       dynamic_cast<Rectangle *>(const_cast<Shape *>(&another));
  //   if (inp_shape) {
  //     std::cout << inp_shape->getVertice1().getX() << " "
  //               << inp_shape->getVertice1().getY() << std::endl;
  //     std::cout << this->getVertice1().getX() << " "
  //               << this->getVertice1().getY() << std::endl;

  //     std::cout << inp_shape->getVertice2().getX() << " "
  //               << inp_shape->getVertice2().getY() << std::endl;
  //     std::cout << this->getVertice2().getX() << " "
  //               << this->getVertice2().getY() << std::endl;

  //     std::cout << inp_shape->getRatio() << " " << inp_shape->getRatio()
  //               << std::endl;
  //     std::cout << this->getRatio() << " " << this->getRatio() << std::endl;

  //     if (inp_shape->getVertice1() != a_ || inp_shape->getVertice2() != b_ ||
  //         inp_shape->getRatio() != ratio_) {
  //       return false;
  //     }
  //     return true;
  //   }
  //   return false;
  // }

  void rotate(Point center, double angle) {
    a_ = rotate_point(a_, center, angle);
    b_ = rotate_point(b_, center, angle);
    d1_ = Line(a_, b_);
    d2_ = Line(-d1_.getAngle(), d1_.getBias());
  }

  void reflex(Point center) {
    a_ = reflex_point(a_, center);
    b_ = reflex_point(b_, center);
    d1_ = Line(a_, b_);
    d2_ = Line(-d1_.getAngle(), d1_.getBias());
  }

  void reflex(Line axis) {
    a_ = reflex_line(axis, a_);
    b_ = reflex_line(axis, b_);
    d1_ = Line(a_, b_);
    d2_ = Line(-d1_.getAngle(), d1_.getBias());
  }

  void scale(Point center, double coefficient) {
    a_ = scale_point(a_, center, coefficient);
    b_ = scale_point(b_, center, coefficient);
    d1_ = Line(a_, b_);
    d2_ = Line(-d1_.getAngle(), d1_.getBias());
    min_length_ = coefficient * min_length_;
    max_length_ = coefficient * max_length_;
  }

  Point getVertice1() { return a_; }
  Point getVertice2() { return b_; }
  double getRatio() { return ratio_; }

 protected:
  // std::vector<Point> vertices_;
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
    length_ = pow_sum_sqrt(a_, b_) / sqrt(2);
    vertices_.push_back(a_);
    vertices_.push_back(b_);
  }

  Circle circumscribedCircle() {
    Point center = get_center(a_, b_);
    auto rad = pow_sum_sqrt(a_, b_);
    return Circle(center, rad);
  }

  Circle inscribedCircle() {
    Point center = get_center(a_, b_);
    auto rad = pow_sum_sqrt(a_, b_) / sqrt(2);
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
    a_ = rotate_point(a_, center, angle);
    b_ = rotate_point(b_, center, angle);
  }

  void reflex(Point center) {
    a_ = reflex_point(a_, center);
    b_ = reflex_point(b_, center);
  }

  void reflex(Line axis) {
    a_ = reflex_line(axis, a_);
    b_ = reflex_line(axis, b_);
  }

  void scale(Point center, double coefficient) {
    a_ = scale_point(a_, center, coefficient);
    b_ = scale_point(b_, center, coefficient);
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
    c_len_ = pow_sum_sqrt(a_, b_);
    b_len_ = pow_sum_sqrt(a_, c_);
    a_len_ = pow_sum_sqrt(b_, c_);
  }

  double centerHelper(double a, double b, double c) {
    auto a_part1 = pow(a, 2) * (pow(b, 2) + pow(c, 2) - pow(a, 2));
    auto a_part2 = (pow(b + c, 2) - pow(a, 2)) * (pow(a, 2) - pow(b - c, 2));
    return a_part1 / a_part2;
  }

  Point getCircumcenter() {
    auto x = a_.getX() * centerHelper(a_len_, b_len_, c_len_) +
             b_.getX() * centerHelper(b_len_, a_len_, c_len_) +
             c_.getX() * centerHelper(c_len_, a_len_, b_len_);
    auto y = a_.getY() * centerHelper(a_len_, b_len_, c_len_) +
             b_.getY() * centerHelper(b_len_, a_len_, c_len_) +
             c_.getY() * centerHelper(c_len_, a_len_, b_len_);
    return Point(x, y);
  }

  Circle circumscribedCircle() {
    Point center = getCircumcenter();
    double radius = pow_sum_sqrt(center, c_);
    return Circle(center, radius);
  }

  Point getIncenter() {
    auto p = perimeter();
    auto x = (a_len_ * a_.getX() + b_len_ * b_.getX() + c_len_ * c_.getX()) / p;
    auto y = (a_len_ * a_.getY() + b_len_ * b_.getY() + c_len_ * c_.getY()) / p;
    return Point(x, y);
  }

  Point getProjection(Point center, Line ab) {
    auto a = -ab.getAngle();
    auto c = -ab.getBias();
    double b = 1;
    auto x = (b * (b * center.getX() - a * center.getY()) - a * c) /
             (pow(a, 2) + pow(b, 2));
    auto y = (a * (-b * center.getX() + a * center.getY()) - b * c) /
             (pow(a, 2) + pow(b, 2));
    return Point(x, y);
  }

  Circle inscribedCircle() {
    Point center = getIncenter();
    Point touch = getProjection(center, Line(a_, c_));
    double radius = pow_sum_sqrt(center, touch);
    return Circle(center, radius);
  }

  Point centroid() {
    auto x = (a_.getX() + b_.getX() + c_.getX()) / 3;
    auto y = (a_.getY() + b_.getY() + c_.getY()) / 3;
    return Point(x, y);
  }

  double getPerp(Point a, Point b) { return -1 / x_dist(b, a); }

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
    auto x = x_dist(orthoc, centr) / 2 + centr.getX();
    auto y = y_dist(orthoc, centr) / 2 + centr.getY();
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
    a_ = rotate_point(a_, center, angle);
    b_ = rotate_point(b_, center, angle);
    c_ = rotate_point(c_, center, angle);
  }

  void reflex(Point center) {
    a_ = reflex_point(a_, center);
    b_ = reflex_point(b_, center);
    c_ = reflex_point(c_, center);
  }

  void reflex(Line axis) {
    a_ = reflex_line(axis, a_);
    b_ = reflex_line(axis, b_);
    c_ = reflex_line(axis, c_);
  }

  void scale(Point center, double coefficient) {
    a_ = scale_point(a_, center, coefficient);
    b_ = scale_point(b_, center, coefficient);
    c_ = scale_point(c_, center, coefficient);
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
