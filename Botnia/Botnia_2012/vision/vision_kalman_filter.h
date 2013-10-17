#ifndef FIELD_KALMANFILTER_H
#define FIELD_KALMANFILTER_H


/* LICENSE:
  =========================================================================
    CMDragons'02 RoboCup F180 Source Code Release
  -------------------------------------------------------------------------
    Copyright (C) 2002 Manuela Veloso, Brett Browning, Mike Bowling,
                       James Bruce; {mmv, brettb, mhb, jbruce}@cs.cmu.edu
    School of Computer Science, Carnegie Mellon University
  -------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ------------------------------------------------------------------------- */
/**********************************************************************
 *
 * Kwun Han <kwunh@cs.cmu.edu>
 * March 1997
 *
 * Michael Bowling <mhb@cs.cmu.edu>
 * 1998-2002
 *
 * Determinant and inverse code is copied from mtrxmath under the GPL.
 *
 **********************************************************************/
// kalman.h
//
// Generic extended kalman-bucy filter code.  Filters can be
// implemented by deriving a class and defining the 8 virtual
// functions that return appropriate dynamics and covariances.
//
// Can handle step sizes different from sensing frequency.
// Predictions are cached to reduce unecessary repeated computation.
//
// Created by:  Michael Bowling (mhb@cs.cmu.edu)
//
/* LICENSE:
  =========================================================================
    CMDragons'02 RoboCup F180 Source Code Release
  -------------------------------------------------------------------------
    Copyright (C) 2002 Manuela Veloso, Brett Browning, Mike Bowling,
                       James Bruce; {mmv, brettb, mhb, jbruce}@cs.cmu.edu
    School of Computer Science, Carnegie Mellon University
  -------------------------------------------------------------------------
    This software is distributed under the GNU General Public License,
    version 2.  If you do not have a copy of this licence, visit
    www.gnu.org, or write: Free Software Foundation, 59 Temple Place,
    Suite 330 Boston, MA 02111-1307 USA.  This program is distributed
    in the hope that it will be useful, but WITHOUT ANY WARRANTY,
    including MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ------------------------------------------------------------------------- */

#include <deque>
using namespace std;

#include "kalman_filter_tools/kalman_filter_matrix.h"

const int PREDICT_BUF_SIZE=500;

class Kalman {
protected:
  int state_n_, obs_n_;		// Number of state and observation variables
  double stepsize_;
        unsigned int predict_count_;
        Matrix xs[PREDICT_BUF_SIZE];	// State vector. [0] is current state.             位置、速度等
        Matrix Ps[PREDICT_BUF_SIZE];	// Covariance matrix.  [0] is current covariance.  协方差
        Matrix Is[PREDICT_BUF_SIZE];	// Information matrix. [0] is current information. 信息

  double stepped_time;	// Time of the last state in the future queue.     最后一个状态的时间
  double time;					// Time of the first state in the future queue.      第一个状态的时间

  // Kalman Error
  Matrix prediction_x;
  double prediction_time;
  double prediction_lookahead;

  Matrix errors;
  int errors_n;

protected:
  virtual Matrix& f(const Matrix &x, Matrix &I) = 0;	// noiseless dynamics
  virtual Matrix& h(const Matrix &x) = 0;							// noiseless observation

  virtual Matrix& Q(const Matrix &x) = 0;							// Covariance of propagation noise
  virtual Matrix& R(const Matrix &x) = 0;							// Covariance of observation noise

  virtual Matrix& A(const Matrix &x) = 0;							// Jacobian of f w.r.t. x
  virtual Matrix& W(const Matrix &x) = 0;							// Jacobian of f w.r.t. noise
  virtual Matrix& H(const Matrix &x) = 0;							// Jacobian of h w.r.t. x
  virtual Matrix& V(const Matrix &x) = 0;							// Jacobian of h w.r.t. noise

  void propagate();

public:
  Kalman(int state_n_, int obs_n_, double _stepsize);
  virtual ~Kalman() {}

  void initial(double t, Matrix &x, Matrix &P);

  void update(const Matrix &z);
  void tick(double dt);

  Matrix predict(double dt);
  Matrix predict_cov(double dt);
  Matrix predict_info(double dt);

  //Matrix predict_fast(double dt);

  double obs_likelihood(double dt, Matrix &z);

  Matrix error_mean();
  void error_reset();
  double error_time_elapsed();
};

#endif
