#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TrackBall
{
public:
	TrackBall() { init(); }

	void trackball(float p1x, float p1y, float p2x, float p2y);

	void init();

	void refresh();

	void negate_quat(float* q, float* qn);

	void add_quats(float* q1, float* q2, float* dest);

	void axis_to_quat(float a[3], float phi, float q[4]);

	float tb_project_to_sphere(float r, float x, float y);

	void normalize_quat(float[4]);

	void update_quat();
	
	void build_rotmatrix(const float q[4]);

	void update_rotmatrix();
	
	glm::mat4 get_m()const { return m; }
	
	static void vzero(float* v) {
		v[0] = 0.0;
		v[1] = 0.0;
		v[2] = 0.0;
	}

	static void vset(float* v, float x, float y, float z) {
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}

	static void vsub(const float* src1, const float* src2, float* dst) {
		dst[0] = src1[0] - src2[0];
		dst[1] = src1[1] - src2[1];
		dst[2] = src1[2] - src2[2];
	}

	static void vcopy(const float* v1, float* v2) {
		register int i;
		for (i = 0; i < 3; i++)
			v2[i] = v1[i];
	}

	static void vcross(const float* v1, const float* v2, float* cross) {
		float temp[3];

		temp[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
		temp[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
		temp[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
		vcopy(temp, cross);
	}

	static float vlength(const float* v)
	{
		return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}

	static void vscale(float* v, float div) {
		v[0] *= div;
		v[1] *= div;
		v[2] *= div;
	}

	static void vnormal(float* v)
	{
		vscale(v, 1.0 / vlength(v));
	}

	static float vdot(const float* v1, const float* v2) {
		return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	}

	static void vadd(const float* src1, const float* src2, float* dst) {
		dst[0] = src1[0] + src2[0];
		dst[1] = src1[1] + src2[1];
		dst[2] = src1[2] + src2[2];
	}
	
	static void quat_trans(glm::vec3 p1, glm::vec3 p2, float q[4])
	{
		p2.x = p1.x * (q[0]* q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]) +
			p1.y * 2 * (q[1] * q[2] - q[0] * q[3]) +
			p1.z * 2 * (q[0] * q[2] + q[1] * q[3]);

		p2.y = p1.x * 2 * (q[0] * q[3] + q[1] * q[2]) +
			p1.y * (q[0] * q[0] - q[1] * q[1] + q[2] * q[2] - q[3] * q[3]) +
			p1.z * 2 * (q[2] * q[3] - q[0] * q[1]);

		p2.z = p1.x * 2 * (q[1] * q[3] - q[0] * q[2]) +
			p1.y * 2 * (q[0] * q[1] + q[2] * q[3]) +
			p1.z * (q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);
	}
private:
	glm::mat4 m;

	const float TRACKBALLSIZE = 0.8, RENORMCOUNT = 97;
	float curr_quat[4] = { 0 }, prev_quat[4] = { 0 };
};