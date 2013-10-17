#include "vision_kalman_filter.h"

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

// kalman.cc
//
// Generic extended kalman-bucy filter code.
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

//#define KALMAN_DEBUG 1

#include <stdio.h>
#include <math.h>
#include "control_hub/computer_control/intelligence/world_analysor/field_analysor_robot.h"
#include "vision_kalman_filter.h"
#include <QtDebug>

Kalman::Kalman(int _state_n, int _obs_n, double _stepsize)
{
        state_n_ = _state_n;
        obs_n_ = _obs_n;
        stepsize_ = _stepsize;

        predict_count_=1;
        xs[0]=Matrix(state_n_, 1);														// State vector. [0] is current state.
        Ps[0]=Matrix(state_n_, state_n_);											// Covariance matrix.  [0] is current covariance
        Is[0]=Matrix();																			// Information matrix. [0] is current information.

        prediction_lookahead = 0.0;
        prediction_time = 0.0;
        errors = Matrix(_state_n, 1);
}

//卡尔曼滤波初始化
void Kalman::initial(double t, Matrix &x, Matrix &P)
{
        predict_count_=1;
        xs[0]=x;
        Ps[0]=P;
        Is[0]=Matrix();

        //前一状态
        stepped_time = time = t;
}


//传播过程函数
void Kalman::propagate()
{
  Matrix x = xs[predict_count_-1];
  Matrix P = Ps[predict_count_-1];
  Matrix &_A = A(x);
  Matrix &_W = W(x);
  Matrix &_Q = Q(x);
  Matrix I;

#if KALMAN_DEBUG
        //if(x.nrows()>4)
        //{
        //	fprintf(stderr, "PROPAGATE:\n");
        //	fprintf(stderr, "x =\n");
        //	x.print();
        //	fprintf(stderr, "P =\n");
        //	P.print();
        //}
#endif
  //-----------------------------------------
  //Time update equations:
  //begin
  x = f(x, I);
  P = _A * P * transpose(_A) + _W * _Q * transpose(_W);
  //end
  //-----------------------------------------

        xs[predict_count_]=x;
        Ps[predict_count_]=P;
        Is[predict_count_]=I;
        predict_count_++;
#if KALMAN_DEBUG
        //if(x.nrows()>4)
        //{
        //	fprintf(stderr, "=============>\nx =\n");
        //	x.print();
        //	fprintf(stderr, "P =\n");
        //	P.print();
        //	fprintf(stderr, "\n");
        //}
#endif
  //步长时间累加
  stepped_time += stepsize_;
}

//observation 观察
//更新函数
void Kalman::update(const Matrix &z)
{
  Matrix x = xs[0];
  Matrix P = Ps[0];
  Matrix I = Is[0];
  Matrix &_H = H(x);
        //robot
  //static Matrix H("[ 1, 0,  0,  0, 0, 0, 0; "
  //                "  0, 1,  0,  0, 0, 0, 0; "
  //                "  0, 0,  1,  0, 0, 0, 0 ]");
  Matrix &_V = V(x);
  //static Matrix V("[ 1, 0, 0; "
  //                "  0, 1, 0; "
  //                "  0, 0, 1 ]");
  Matrix &_R = R(x);
  //if (!R.nrows()) {
  //  R.identity(3);
  //  R.e(0,0) = (DVAR(ROBOT_POSITION_VARIANCE));
  //  R.e(1,1) = (DVAR(ROBOT_POSITION_VARIANCE));
  //  R.e(2,2) = (DVAR(ROBOT_THETA_VARIANCE));
  //}

  // We clear the prediction list because we have a new observation.
        predict_count_=0;
        stepped_time = time;

  //--------------------------------------------------------
  //Measurement update equations
  Matrix K = P * transpose(_H) *
    inverse(_H * P * transpose(_H) + _V * _R * transpose(_V));

  Matrix error = K * (z - h(x));

#if KALMAN_DEBUG
        //if(x.nrows()>4)
        //{
        //	fprintf(stderr, "UPDATE:\n");
        //	fprintf(stderr, "x =\n");
        //	x.print();
        //	fprintf(stderr, "z =\n");
        //	z.print();
        //	fprintf(stderr, "P =\n");
        //	P.print();
        //	fprintf(stderr, "K =\n");
        //	K.print();
        //	fprintf(stderr, "error =\n");
        //	error.print();
        //}
#endif

  x = x + error;
  P = (Matrix(P.nrows()) - K * _H) * P;
  //--------------------------------------------------------
  // Add the current state back onto the prediction list.
        xs[predict_count_]=x;
        Ps[predict_count_]=P;
        Is[predict_count_]=I;
        predict_count_++;

  if (prediction_lookahead > 0.0) {
    if (time - prediction_time >= prediction_lookahead) {

      if (prediction_time > 0.0) {
                                Matrix error = x - prediction_x;

                                for(int i=0; i < error.nrows(); i++)
                                        errors.e(i, 0) += fabs(error.e(i, 0));
                                errors_n++;
                        }
      prediction_x = predict(prediction_lookahead);
      prediction_time = time;
    }
  }

#if KALMAN_DEBUG
        //if(x.nrows()>4)
        //{
        //	fprintf(stderr, "=============>\nx =\n");
        //	x.print();
        //	fprintf(stderr, "P =\n");
        //	P.print();
        //	fprintf(stderr, "\n");
        //}
#endif
}

//dt为相对于起点时间time的变化量
void Kalman::tick(double dt)
{
  uint nsteps = (int) rint(dt / stepsize_);
  while(predict_count_ - 1 < nsteps)
        {
                propagate();
        }

        for(unsigned int i=0;i<nsteps;i++)
        {
                if(i+nsteps<predict_count_)
                {
                        xs[i]=xs[i+nsteps];
                        Ps[i]=Ps[i+nsteps];
                        Is[i]=Is[i+nsteps];
                }
        }
        predict_count_-=nsteps;
        if(predict_count_<0)predict_count_=0;

  time += dt;
}

//先进行预测，然后返回x
Matrix Kalman::predict(double dt)
{
  uint nsteps = (int) rint(dt / stepsize_);
  while(predict_count_ - 1 < nsteps)
        {
                propagate();
        }

  return xs[nsteps];
}

//先进行预测，然后返回P
Matrix Kalman::predict_cov(double dt)
{
  uint nsteps = (int) rint(dt / stepsize_);

  while(predict_count_ - 1 < nsteps)
        {
                propagate();
        }

  return Ps[nsteps];
}

//先进行预测，然后返回信息
Matrix Kalman::predict_info(double dt)
{
  uint nsteps = (int) rint(dt / stepsize_);

  while(predict_count_ - 1 < nsteps)
        {
                propagate();
        }
  return Is[nsteps];
}
/*
Matrix Kalman::predict_fast(double dt)
{
  uint nsteps = (int) rint(dt / stepsize_);
  double orig_stepsize = stepsize_;

  if (xs.size() - 1 >= nsteps) return xs[nsteps];

  stepsize_ = dt - (stepped_time - time);
  propagate();

  Matrix rv = xs.back();

  stepped_time -= stepsize_;
  stepsize_ = orig_stepsize;
  xs.pop_back();
  Ps.pop_back();
  Is.pop_back();

  return rv;
}
*/

//检查观测数据
double Kalman::obs_likelihood(double dt, Matrix &z)
{
  Matrix x = predict(dt);
  Matrix P = predict_cov(dt);
  Matrix _hx = h(x);
  Matrix &_H = H(x);

  Matrix C = _H * P * transpose(_H);

  Matrix D = z - _hx;

  double likelihood = 1.0;

  for(int i=0; i<D.nrows(); i++)
    likelihood *= exp( - (D.e(i, 0) * D.e(i, 0)) / (2 * C.e(i, i)) );

  return likelihood;
}

Matrix Kalman::error_mean()
{
  return errors.scale(1.0 / (double) errors_n);
}

void Kalman::error_reset()
{
  errors = errors.scale(0.0);
  errors_n = 0;
}

double Kalman::error_time_elapsed()
{
  return errors_n * prediction_lookahead;
}

