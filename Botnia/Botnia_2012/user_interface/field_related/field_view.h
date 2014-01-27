#ifndef FIELD_VIEW_H
#define FIELD_VIEW_H

#include "field_constants.h"
#include "field_scene.h"
#include "net/net_vision_server.h"
#include "net/net_vision_client.h"
#ifndef MUTEX
#define MUTEX
extern QMutex draw_mutex;
extern QMutex display_update_mutex;
extern QMutex graphicsMutex;
extern QMutex display_set_mutex;

#endif

#include <QGraphicsView>




class FieldView : public QGraphicsView
{
    Q_OBJECT
public slots:

    void tickslot();

private slots:

//    void redraw();

signals:

//    void postRedraw();

public:
    QString debug_string_;
    double line_width_;
    double field_length_;
    double field_width_;
    double boundary_width_;
    double referee_width_;
    double goal_width_;
    double goal_depth_;
    double goal_wall_width_;
    double center_circle_radius_;
    double defense_radius_;
    double defense_stretch_;
    double free_kick_from_defense_dist_;
    double penalty_spot_from_field_line_dist_;
    double penalty_line_from_spot_dist_;

    RobotItem* temp_robot;
    SSL_DetectionFrame detection;
    FieldScene* scene;

    FieldView(QWidget * parent = 0);
    ~FieldView();
    void Initilize();
    void AddRobot ( RobotItem* robot );
    void BuildRobotAndBall();
    void SetRobot ( int team,int id,bool bVisible );
    void AddBall  ( BallItem *ball ,int cameraID);
    void LoadFieldGeometry();
    void LoadFieldGeometry ( SSL_GeometryFieldSize &fieldSize );
    void UpdateViewScale();
    void UpdateView();
    void repaint ( int x, int y, int w, int h );
    void repaint ( const QRect & rect );
    void repaint ( const QRegion & rgn );
    void tick ();

protected:

    void wheelEvent ( QWheelEvent *event );
    virtual void drawForeground ( QPainter * painter, const QRectF & rect );
    void ScaleView ( qreal scaleFactor );
    float draw_scale;
    bool scaling_requested;

private:
    int conf;
    double tLastRedraw;
    void ConstructField();
    QVector<RobotItem*> robots;
    QVector < QVector<BallItem*> > balls_;
    QPainterPath *field_painter;
    QGraphicsPathItem *field_item;
    QBrush *field_brush, *ball_brush;
    QPen *field_pen, *field_line_pen, *ball_pen;
    QGLWidget* gl_widget;
    bool shutdownfield_view;
    void UpdateRobotsDisplay();
    void UpdateDebugPath ();
    void ConstructDebug();
    QPainterPath *debugPath;
    QGraphicsPathItem *debugPathItem;


//    bool leftButton;
//    bool midButton;
//    bool rightButton;
//    int mouseStartX;
//    int mouseStartY;

    static const double MinRedrawInterval = 0.016;


};

#endif // FIELD_VIEW_H
