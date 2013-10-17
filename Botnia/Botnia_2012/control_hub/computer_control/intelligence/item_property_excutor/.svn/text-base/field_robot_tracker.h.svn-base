#ifndef FIELD_ROBOT_TRACKER_H
#define FIELD_ROBOT_TRACKER_H

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


#include "net/net_vision_interface.h"
#include "vision/vision_kalman_filter.h"

class RobotTracker : private Kalman
{
private:
        int type;
        double latency;

        int reset_on_observation;

        struct rcommand
        {
                double timestamp;
                MyVector3d vs;
        };

        deque<rcommand> cs; // Velocity commands

        rcommand get_command(double time);

protected:
        virtual Matrix& f(const Matrix &x, Matrix &I);	// noiseless dynamics
        virtual Matrix& h(const Matrix &x);				// noiseless observation

        virtual Matrix& Q(const Matrix &x);				// Covariance of propagation noise
        virtual Matrix& R(const Matrix &x);				// Covariance of observation noise

        virtual Matrix& A(const Matrix &x); // Jacobian of f w.r.t. x
        virtual Matrix& W(const Matrix &x); // Jacobian of f w.r.t. noise
        virtual Matrix& H(const Matrix &x); // Jacobian of h w.r.t. x
        virtual Matrix& V(const Matrix &x); // Jacobian of h w.r.t. noise

public:
        RobotTracker(int type = ROBOT_TYPE_OMNI,
                     double _latency = LATENCY_DELAY);
        virtual ~RobotTracker() {}
        void LoadConfig();

        void set_type(int _type)
        {
                type = _type;
                reset();
        }

        void reset()
        {
                reset_on_observation = true;
        }
        void reset(double timestamp, float state[6]);

        void observe(VisionRawInfo obs, double timestamp);
        void command(double timestamp, MyVector3d vs);

        MyVector2d position(double time);
        MyVector2d velocity(double time);
        MyVector2d velocity_raw(double time);
        double direction(double time);
        double angular_velocity(double time);

        double stuck(double time);
};


#endif // FIELD_ROBOT_TRACKER_H
