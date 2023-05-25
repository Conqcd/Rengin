#include "trackball.h"

void TrackBall::trackball(float p1x, float p1y, float p2x, float p2y)
{
    float a[3];
    float phi;
    float p1[3], p2[3], d[3];
    float t;

    if (p1x == p2x && p1y == p2y) {

        vzero(prev_quat);
        prev_quat[3] = 1.0;
        return;
    }

    vset(p1, p1x, p1y, tb_project_to_sphere(TRACKBALLSIZE, p1x, p1y));
    vset(p2, p2x, p2y, tb_project_to_sphere(TRACKBALLSIZE, p2x, p2y));

    vcross(p2, p1, a);

    vsub(p1, p2, d);
    t = vlength(d) / (2.0 * TRACKBALLSIZE);

    if (t > 1.0)
        t = 1.0;
    if (t < -1.0)
        t = -1.0;
    phi = 2.0 * asin(t);

    axis_to_quat(a, phi, prev_quat);
}

void TrackBall::init()
{
    vzero(curr_quat);
    curr_quat[3] = 1.0;
}

void TrackBall::refresh()
{
    vzero(prev_quat);
    prev_quat[3] = 1.0;
}

void TrackBall::negate_quat(float* q, float* qn)
{
	
}

void TrackBall::add_quats(float* q1, float* q2, float* dest)
{

    static int count = 0;
    float t1[4], t2[4], t3[4];
    float tf[4];

    vcopy(q1, t1);
    vscale(t1, q2[3]);

    vcopy(q2, t2);
    vscale(t2, q1[3]);

    vcross(q2, q1, t3);
    vadd(t1, t2, tf);
    vadd(t3, tf, tf);
    tf[3] = q1[3] * q2[3] - vdot(q1, q2);

    dest[0] = tf[0];
    dest[1] = tf[1];
    dest[2] = tf[2];
    dest[3] = tf[3];

    if (++count > RENORMCOUNT) {
        count = 0;
        normalize_quat(dest);
    }
}

void TrackBall::axis_to_quat(float a[3], float phi, float q[4])
{
    vnormal(a);
    vcopy(a, q);
    vscale(q, sin(phi / 2.0));
    q[3] = cos(phi / 2.0);
}

float TrackBall::tb_project_to_sphere(float r, float x, float y)
{
    float d, t, z;
    d = sqrt(x * x + y * y);
    if (d < r * 0.70710678118654752440) {
        z = sqrt(r * r - d * d);
    }
    else {
        t = r / 1.41421356237309504880;
        z = t * t / d;
    }
    return z;
}

void TrackBall::normalize_quat(float q[4])
{
    int i;
    float mag;

    mag = (q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
    for (i = 0; i < 4; i++)
        q[i] /= mag;
}

void TrackBall::update_quat()
{
    add_quats(prev_quat, curr_quat, curr_quat);
}

void TrackBall::build_rotmatrix(const float q[4])
{
    m[0][0] = 1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]);
    m[0][1] = 2.0 * (q[0] * q[1] - q[2] * q[3]);
    m[0][2] = 2.0 * (q[2] * q[0] + q[1] * q[3]);
    m[0][3] = 0.0;

    m[1][0] = 2.0 * (q[0] * q[1] + q[2] * q[3]);
    m[1][1] = 1.0 - 2.0 * (q[2] * q[2] + q[0] * q[0]);
    m[1][2] = 2.0 * (q[1] * q[2] - q[0] * q[3]);
    m[1][3] = 0.0;

    m[2][0] = 2.0 * (q[2] * q[0] - q[1] * q[3]);
    m[2][1] = 2.0 * (q[1] * q[2] + q[0] * q[3]);
    m[2][2] = 1.0 - 2.0 * (q[1] * q[1] + q[0] * q[0]);
    m[2][3] = 0.0;

    m[3][0] = 0.0;
    m[3][1] = 0.0;
    m[3][2] = 0.0;
    m[3][3] = 1.0;
}

void TrackBall::update_rotmatrix()
{
    build_rotmatrix(curr_quat);
}
