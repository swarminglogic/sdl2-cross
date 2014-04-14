#ifndef AVERAGER_H
#define AVERAGER_H

#include <cassert>
#include <list>


/**
 * Class that averages values continuously in a circular buffer with N values.
 *
 * Case A: When full:
 *               _                  _
 *           N * X     =  x  +  N * X  - x
 *                n+1      n         n    n-N
 *
 *
 * Case B: When incrementing size (M):
 *                     _                _
 *           (M + 1) * X      = x + M * X
 *                      n+1              n
 *
 * @notes
 *
 * - This gives O(1) time to compute regardless of Case A or Case B.
 *
 * - Has O(1) memory consumption, rather, N (size of the window) doubles.
 *
 * - For use of only Case B, it is not necessary to retain memory of previous
 *   values, only the sum. Consider refactoring implementation for optimization.
 *
 * - Possible to use vector to store values, and use modular arithmetic to swap
 *   out the oldest value, and still preserve cronological order. This is
 *   optimal, as opposed to the current implementation (linked list), algorithm
 *   is still O(1) regardless.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class Averager
{
public:
  Averager(unsigned int N)
    : isFull_(false),
      maxN_(N),
      N_(0),
      MX_n_(0.0),
      x_()
  {
    assert(N > 0 && "Cannot average 0 elements.");
  }
  ~Averager() {}

  void insertValue(double x_n)
  {
    // ---------- IS FULL ------------------
    if (isFull_) {
      const double X_oldest = x_.front();
      x_.pop_front();
      MX_n_ += x_n - X_oldest;
    }

    // ---------- IS INCREMENTING ----------
    else {
      MX_n_ += x_n;
      ++N_;
      isFull_ = (N_ == maxN_);
    }

    x_.push_back(x_n);
  }

  double getAverage() const
  {
    assert(N_ != 0);
    return MX_n_ / static_cast<double>(N_);
  }


private:
  bool isFull_;
  const int maxN_;
  int N_;
  double MX_n_;
  std::list<double> x_;
};

#endif
