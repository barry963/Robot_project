#ifndef FIELD_BALL_TRACKER_H
#define FIELD_BALL_TRACKER_H

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

class ObjectTracker;

class BallTracker : private Kalman
{
public:
        enum OccludeFlag { Visible, MaybeOccluded, Occluded };
        void LoadConfig();
private:
        bool _reset;

        double velocity_variance(const Matrix &x);
        bool check_for_collision(const Matrix &x, MyVector2d &cp, MyVector2d &cv,
                                 int &team, int &robot);

        //检查球是否被机器人捕捉住
        bool check_occlusion();

        void tick_occlusion(double dt);

        MyVector2d occluded_position(double time);
        MyVector2d occluded_velocity(double time);

        // Pointer up to the complete set of trackers... for collisions,
        // occlusions, and such.
        ObjectTracker *tracker;

        // Occlusion Information
        OccludeFlag occluded_;
        char occluding_team;
        char occluding_robot;
        MyVector2d occluding_offset;
        double occluded_last_obs_time;

        friend class ObjectTracker;

protected:
        virtual Matrix& f(const Matrix &x, Matrix &I); // noiseless dynamics
        virtual Matrix& h(const Matrix &x); // noiseless observation

        virtual Matrix& Q(const Matrix &x); // Covariance of propagation noise
        virtual Matrix& R(const Matrix &x); // Covariance of observation noise

        virtual Matrix& A(const Matrix &x); // Jacobian of f w.r.t. x
        virtual Matrix& W(const Matrix &x); // Jacobian of f w.r.t. noise
        virtual Matrix& H(const Matrix &x); // Jacobian of h w.r.t. x
        virtual Matrix& V(const Matrix &x); // Jacobian of h w.r.t. noise

public:
        BallTracker();
        virtual ~BallTracker() {}

        void set_tracker(ObjectTracker *t)
        {
                tracker = t;
        }

        void reset()
        {
                _reset = true;
        }
        void reset(double timestamp,MyVector2d pos);
        void reset(double timestamp, float state[4], float variances[16],
                   OccludeFlag occluded_,
                   char occluding_team, char occluding_robot,
                   vector2f occluding_offset);

        void observe(VisionRawInfo obs, double timestamp);

        MyVector2d position(double time);
        MyVector2d velocity(double time);
        Matrix covariances(double time);

        bool collision(double time, int &team, int &robot);
};

#include "field_tracker.h"



#endif // FIELD_BALL_TRACKER_H
