#pragma once
#include <vector>
#include <iostream>

using std::vector;
using std::abs;
using std::endl;
using std::cout;
using std::ostream;
using std::istream;

namespace task {
  vector <double> operator + (const vector <double> & a,
    const vector <double> & b) {
    vector <double> summ;
    for (size_t i = 0; i < a.size(); ++i) {
      summ.push_back(a[i] + b[i]);
    }
    return summ;
  }

  vector <double> operator - (const vector <double> & a,
    const vector <double> & b) {
    vector <double> subs;
    for (size_t i = 0; i < a.size(); ++i) {
      subs.push_back(a[i] - b[i]);
    }
    return subs;
  }

  vector <double> operator + (const vector <double> & a) {
    return a;
  }

  vector <double> operator - (const vector <double> & a) {
    vector <double> inverse;
    for (size_t i = 0; i < a.size(); ++i) {
      inverse.push_back(-a[i]);
    }
    return inverse;
  }

  double operator * (const vector <double> & a,
    const vector <double> & b) {
    double dot_prod = 0;
    for (size_t i = 0; i < a.size(); ++i) {
      dot_prod += a[i] * b[i];
    }
    return dot_prod;
  }

  vector <double> operator % (const vector <double> & a,
    const vector <double> & b) {
    vector <double> vector_prod;
    vector_prod.push_back(a[1] * b[2] - a[2] * b[1]);
    vector_prod.push_back(a[2] * b[0] - a[0] * b[2]);
    vector_prod.push_back(a[0] * b[1] - a[1] * b[0]);
    return vector_prod;
  }

  bool operator || (const vector <double> & a,
    const vector <double> & b) {
    double eps = 1e-12;
    for (size_t i = 0; i < a.size() - 1; ++i) {
      double mul1 = a[i] * b[i + 1];  
      double mul2 = a[i + 1] * b[i];  
      if (abs(mul1 - mul2) > eps) {
        return false;
      }
    }
    return true;
  }

  bool operator && (const vector <double> & a,
    const vector <double> & b) {
    if (a || b) {
      for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] / b[i] < 0) {
          return false;
        }
      }
      return true;
    } 
    return false;
  }

  vector <int> operator | (const vector <int> & a,
    const vector <int> & b) {
    vector <int> result_vector;
    for (size_t i = 0; i < a.size(); ++i) {
      result_vector.push_back(a[i] | b[i]);
    }
    return result_vector;
  }

  vector <int> operator & (const vector <int> & a,
    const vector <int> & b) {
    vector <int> result_vector;
    for (size_t i = 0; i < a.size(); ++i) {
      result_vector.push_back(a[i] & b[i]);
    }
    return result_vector;
  }

  ostream & operator << (ostream & out,
    const vector <double> & a) {
    for (size_t i = 0; i < a.size(); ++i) {
      out << a[i] << " ";
    }
    out << endl;
    return out;
  }

  istream & operator >> (istream & in , vector <double> & a) {
    size_t n; 
    in >> n;
    a.clear();
    for (size_t i = 0; i < n; ++i) {
      double number; in >> number;
      a.push_back(number);
    }
    return in;
  }

  void reverse(vector <double> & a) {
    size_t size = a.size();
    for (size_t i = 0; i < size / 2; ++i) {
      iter_swap(a.begin() + i, a.end() - i - 1);
    }
  }

} // namespace task
