#include <svl/svl.h>
#include <iostream>

using namespace std;

main ()
{
#if 0
	Vec2 v(2, 1);
	Vec2 w(1, 3);

	cout << v << endl;
	cout << len(v - w) << endl;
	cout << acos( dot(v, w)/ len(v) / len(w) ) / vl_pi * 180 << endl;

	Mat2 m(1, 3, 2, -1);
	cout << m << endl;
	cout << det(m) << endl;
	cout << inv(m)*m << endl;
	cout << trans(m) << endl;
#endif
	Vec2 v(2, 1);
	Vec2 vv;

	//translation by (-1, 3)
	vv = v + Vec2(-1, 3);
	cout << vv << endl;

	//rotation
	vv = Rot2(90.0 / 180 * vl_pi) * v;
	cout << vv << endl;

	//homogeneous rotation
	vv = proj( HRot3(90.0/180*vl_pi) * Vec3(v, 1) );
	cout << vv << endl;

	system("pause");
}