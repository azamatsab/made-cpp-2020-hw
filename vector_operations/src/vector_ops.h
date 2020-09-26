#pragma once
#include <vector>
#include <iostream>


namespace task {
    using std::vector;
    using std::abs;
    using std::endl;

    vector<double> operator + (const vector<double>& a, const vector<double>& b) {
        vector<double> c;
        for (size_t i = 0; i < a.size(); ++ i) {
            c.push_back(a[i] + b[i]);
        }
        return c;
    }

    vector<double> operator - (const vector<double>& a, const vector<double>& b) {
        vector<double> c;
        for (size_t i = 0; i < a.size(); ++ i) {
            c.push_back(a[i] - b[i]);
        }
        return c;
    }

    vector<double> operator + (const vector<double>& a) {
        return a;
    }

    vector<double> operator - (const vector<double>& a) {
        vector<double> c;
        for (size_t i = 0; i < a.size(); ++ i) {
            c.push_back(-a[i]);
        }
        return c;
    }

    double operator * (const vector<double>& a, const vector<double>& b) {
        double dot_prod = 0;
        for (size_t i = 0; i < a.size(); ++ i) {
            dot_prod += a[i] * b[i];
        }
        return dot_prod;
    }

    vector<double> operator % (const vector<double>& a, const vector<double>& b) {
        vector<double> c;
        c.push_back(a[1] * b[2] - a[2] * b[1]);
        c.push_back(a[2] * b[0] - a[0] * b[2]);
        c.push_back(a[0] * b[1] - a[1] * b[0]);
        return c;
    }

    bool operator || (const vector<double>& a, const vector<double>& b) {
        vector<double> n;
        double eps = 1e-12;
        for (size_t i = 0; i < a.size(); ++ i) {
            if (a[i] != 0 && b[i] != 0) {
                n.push_back(abs(a[i] / b[i]));
            }
        }
        for (size_t i = 0; i < n.size() - 1; ++ i) {
            if ((n[i] - n[i + 1] > eps) || (n[i + 1] - n[i] > eps)) {
                return false;
            }
        }
        return true;
    }

    bool operator && (const vector<double>& a, const vector<double>& b) {
        vector<double> n;
        double eps = 1e-12;
        for (size_t i = 0; i < a.size(); ++ i) {
            if (a[i] != 0 && b[i] != 0) {
                n.push_back(a[i] / b[i]);
            }
        }
        for (size_t i = 0; i < n.size() - 1; ++ i) {
            if (( n[i] - n[i + 1] > eps ) || ( n[i + 1] - n[i] > eps ) ) {
                return false;
            }
            if ((n[i] < 0) || (n[i + 1] < 0)) {
                return false;
            }
        }
        return true;
    }

    vector<int> operator | (const vector<int>& a, const vector<int>& b) {
        vector<int> c;
        for (size_t i = 0; i < a.size(); ++ i) {
            c.push_back(a[i] | b[i]);
        }
        return c;
    }

    vector<int> operator & (const vector<int>& a, const vector<int>& b) {
        vector<int> c;
        for (size_t i = 0; i < a.size(); ++ i) {
            c.push_back(a[i] & b[i]);
        }
        return c;
    }

    std::ostream& operator << (std::ostream &out, const vector<double>& a) {
        for (size_t i = 0; i < a.size(); ++ i) {
            out << a[i] << " ";
        }
        out << endl;
        return out;
    }

    std::istream& operator >> (std::istream &in, vector<double>& a) {
        a.clear();
        size_t n; 
        in >> n; 
        for (size_t i = 0; i < n; ++ i) {
            double number;
            in >> number;
            a.push_back(number);
        }
        return in;
    }

     void reverse(vector<double>& a) {
        size_t size = a.size();
        for (size_t i = 0; i < size / 2; ++ i) {
            iter_swap(a.begin() + i, a.end() - i - 1);
        }
    }   

}  // namespace task
