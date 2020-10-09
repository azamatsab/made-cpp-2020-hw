#pragma once
#include <iostream>
#include <vector>

using std::abs;
using std::cout;
using std::endl;
using std::istream;
using std::ostream;
using std::vector;

namespace task {
vector<double> operator+(const vector<double> &opn1,
                         const vector<double> &opn2) {
  vector<double> summ(opn1.size());
  for (size_t i = 0; i < opn1.size(); ++i) {
    summ[i] = opn1[i] + opn2[i];
  }
  return summ;
}

vector<double> operator-(const vector<double> &opn1,
                         const vector<double> &opn2) {
  vector<double> subs(opn1.size());
  for (size_t i = 0; i < opn1.size(); ++i) {
    subs[i] = opn1[i] - opn2[i];
  }
  return subs;
}

vector<double> operator+(const vector<double> &opn1) {
  return opn1;
  }

vector<double> operator-(const vector<double> &opn1) {
  vector<double> inverse(opn1.size());
  for (size_t i = 0; i < opn1.size(); ++i) {
    inverse[i] = -opn1[i];
  }
  return inverse;
}

double operator*(const vector<double> &opn1, const vector<double> &opn2) {
  double dot_prod = 0;
  for (size_t i = 0; i < opn1.size(); ++i) {
    dot_prod += opn1[i] * opn2[i];
  }
  return dot_prod;
}

vector<double> operator%(const vector<double> &opn1,
                         const vector<double> &opn2) {
  vector<double> vector_prod(3);
  vector_prod[0] = opn1[1] * opn2[2] - opn1[2] * opn2[1];
  vector_prod[1] = opn1[2] * opn2[0] - opn1[0] * opn2[2];
  vector_prod[2] = opn1[0] * opn2[1] - opn1[1] * opn2[0];
  return vector_prod;
}

bool operator||(const vector<double> &opn1, const vector<double> &opn2) {
  double eps = 1e-12;
  for (size_t i = 0; i < opn1.size() - 1; ++i) {
    double mul1 = opn1[i] * opn2[i + 1];
    double mul2 = opn1[i + 1] * opn2[i];
    if (abs(mul1 - mul2) > eps) {
      return false;
    }
  }
  return true;
}

bool operator&&(const vector<double> &opn1, const vector<double> &opn2) {
  if (opn1 || opn2) {
    for (size_t i = 0; i < opn1.size(); ++i) {
      if ((opn1[i] < 0) && (opn2[i] >= 0) || (opn1[i] >= 0) && (opn2[i] < 0)) {
        return false;
      }
    }
    return true;
  }
  return false;
}

vector<int> operator|(const vector<int> &opn1, const vector<int> &opn2) {
  vector<int> result_vector(opn1.size());
  for (size_t i = 0; i < opn1.size(); ++i) {
    result_vector[i] = opn1[i] | opn2[i];
  }
  return result_vector;
}

vector<int> operator&(const vector<int> &opn1, const vector<int> &opn2) {
  vector<int> result_vector(opn1.size());
  for (size_t i = 0; i < opn1.size(); ++i) {
    result_vector[i] = opn1[i] & opn2[i];
  }
  return result_vector;
}

ostream &operator<<(ostream &out, const vector<double> &opn) {
  for (size_t i = 0; i < opn.size(); ++i) {
    out << opn[i] << " ";
  }
  out << endl;
  return out;
}

istream &operator>>(istream &in, vector<double> &opn) {
  size_t n;
  in >> n;
  opn.clear();
  opn.resize(n);
  for (size_t i = 0; i < n; ++i) {
    in >> opn[i];
  }
  return in;
}

void reverse(vector<double> &opn) {
  size_t size = opn.size();
  for (size_t i = 0; i < size / 2; ++i) {
    iter_swap(opn.begin() + i, opn.end() - i - 1);
  }
}

}