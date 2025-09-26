#include "numeric_matrix.h"
#include "numeric_vector.h"
#include "slice.h"
#include "../rational/rational.h"
NUMERICS_NAMESPACE_BEGIN
std::ostream& operator<<(std::ostream& stream, const slice& slice)
{
	if (slice.step() != 1)
		stream << slice.begin() << ":" << slice.end() << ":" << slice.step();
	else
		stream << slice.begin() << ":" << slice.end();
	return stream;
};

void template_vector_test()
{
	auto first = template_vector_<I32>();
	first.push_back(11).push_back(22).push_back(33).push_back(44).push_back(55);

	map_values<I32, double> int_to_double = first.map<double>([](const I32& i) { return std::sqrt(i); });

	for (auto const a : int_to_double) std::cout << "ccc: " << a << "\n";

	auto second = template_vector_<double>(int_to_double);

	auto copy_first = template_vector_<double>(int_to_double);

	copy_first.push_back(11).
		push_back(22).
		push_back(33).
		push_back(55);

	auto copy_of_first = template_vector_<double>(second);

	auto zip_vals = zip_values<double, double>(copy_of_first.values(), copy_of_first.values());

	auto diff = [](const double& a, const double& b) { return a - b; };
	auto summ = [](const double& a, const double& b) { return a + b; };

	// combine_values<double, double> combine_vals = combine_values<double, double>(zip_values<double, double>(copy_of_first.values(), copy_of_first.values()), summ);	combine_values<double, double> combine_vals = combine_values<double, double>(zip_values<double, double>(copy_of_first.values(), copy_of_first.values()), summ);
	// combine_values<double, double> combine_vals = combine_values<double, double>(copy_of_first.values(), copy_of_first.values(), summ);	
	combine_values<double, double> combine_vals = combine_values<double, double>(100000.0, copy_of_first.values(), summ);
	//combine_values<double, double> combine_vals = combine_values<double, double>(zip_values<double, double>(copy_of_first.values(), copy_of_first.values()), summ);

	template_vector_<double> combines = template_vector_<double>(combine_vals);

	std::cout << "first                  :" << first << "\n";
	std::cout << "copy_first             :" << copy_first << "\n";
	std::cout << "copy_of_first          :" << copy_of_first << "\n";
	std::cout << "combines               :" << combines << "\n";
	std::cout << "combines               :" << template_vector_<double>(combines.combine<double>(combines, diff)) << "\n";
	std::cout << "first.insert(0, 13)    :" << first.insert(0, 13) << "\n";
	std::cout << "first.insert(2, 13)    :" << first.insert(2, 13) << "\n";
	std::cout << "first.insert(13,13)    :" << first.insert(13, 13) << "\n";

	std::cout << "first.remove_at(0)     :" << first.remove_at(0) << "\n";
	std::cout << "first.remove_at(2)     :" << first.remove_at(2) << "\n";
	std::cout << "first.remove_at(1)     :" << first.remove_at(1) << "\n";
	std::cout << "copy_first             :" << copy_first << "\n";
	auto val = copy_first[slice(3, 6)];
	std::cout << "copy_first[slice(3, 6)]:" << val << "\n";
	// copy_first[slice(3, 6)] = template_vector_<double>({ 0, 0, 0 });
	auto v = copy_first[slice(3, 6)];
	v.apply([](const double v) {return -1.0; });
	std::cout << "copy_first[slice(3, 6)]:" << copy_first[slice(3, 6)] << "\n";
	std::cout << "copy_first             :" << copy_first << "\n";
}

void numeric_vector_test()
{
	vector_f64 lhs({ 1.0, 2.0, 3.0, 4.0, 9.0, 8.0, 7.0, 6.0 });
	vector_f64 rhs({ 9.0, 8.0, 7.0, 6.0, 1.0, 2.0, 3.0, 4.0 });
	// rhs.push_back(9.0).push_back(8.0).push_back(7.0).push_back(6.0);
	std::cout << "lhs            : " << lhs << "\n";
	std::cout << "rhs            : " << rhs << "\n";
	std::cout << "rhs - copy     : " << vector_f64(rhs) << "\n";
	std::cout << "lhs + rhs      : " << lhs + rhs << "\n";
	std::cout << "lhs - rhs      : " << lhs - rhs << "\n";
	std::cout << "lhs * rhs      : " << lhs * rhs << "\n";
	std::cout << "lhs / rhs      : " << lhs / rhs << "\n";

	std::cout << "2 + rhs        : " << 2.0 + rhs << "\n";
	std::cout << "2 - rhs        : " << 2.0 - rhs << "\n";
	std::cout << "2 * rhs        : " << 2.0 * rhs << "\n";
	std::cout << "2 / rhs        : " << 2.0 / rhs << "\n";

	std::cout << "rhs + 2        : " << rhs + 2.0 << "\n";
	std::cout << "rhs - 2        : " << rhs - 2.0 << "\n";
	std::cout << "rhs * 2        : " << rhs * 2.0 << "\n";
	std::cout << "rhs / 2        : " << rhs / 2.0 << "\n";

	std::cout << "lhs += rhs     : " << (lhs += rhs) << "\n";
	std::cout << "lhs -= rhs     : " << (lhs -= rhs) << "\n";
	std::cout << "lhs *= rhs     : " << (lhs *= rhs) << "\n";
	std::cout << "lhs /= rhs     : " << (lhs /= rhs) << "\n";

	std::cout << "lhs += 2.0     : " << (lhs += 2.0) << "\n";
	std::cout << "lhs -= 2.0     : " << (lhs -= 2.0) << "\n";
	std::cout << "lhs *= 2.0     : " << (lhs *= 2.0) << "\n";
	std::cout << "lhs /= 2.0     : " << (lhs /= 2.0) << "\n";
	std::cout << "mag(lhs)       : " << rational_number(lhs.magnitude()) << "\n";
	std::cout << "dot(lhs, rhs)  : " << rational_number(vector_f64::dot(lhs, rhs)) << "\n";
	std::cout << "dir(lhs, rhs)  : " << vector_f64::direction(lhs, rhs) << "\n";
	std::cout << "normalized(lhs): " << lhs.normalized() << "\n";
	std::cout << "normalize (lhs): " << lhs.normalize() << "\n";
	std::cout << "gradient  (lhs): " << vector_f64::gradient(test_f, lhs, 1e-9) << "\n";
	std::cout << "partial2  (lhs): " << rational_number(vector_f64::partial2(test_f, lhs, 0, 0, 1e-9)) << "\n";
	std::cout << "lhs == rhs     : " << (lhs == rhs) << "\n";
	std::cout << "lhs != rhs     : " << (lhs != rhs) << "\n";
	std::cout << "lhs > rhs      : " << (lhs > rhs) << "\n";
	std::cout << "lhs < rhs      : " << (lhs < rhs) << "\n";
	std::cout << "lhs >= rhs     : " << (lhs >= rhs) << "\n";
	std::cout << "lhs <= rhs     : " << (lhs <= rhs) << "\n";
	std::cout << "lhs >= 100     : " << (lhs >= 100.0) << "\n";
	std::cout << "lhs <= 100     : " << (lhs <= 100.0) << "\n";
	std::cout << "lhs            : " << lhs << "\n";
	lhs[slice(3, 6)] = vector_f64({ 13.0, 13.0, 13.0 });
	std::cout << "lhs            : " << lhs << "\n";
}

void numeric_matrix_test()
{
	vector_f64 vec({ 1.0, 2.0, 3.0 });

	matrix_f64 lhs({ 8.0, 1.0, 6.0,
					3.0, 5.0, 7.0,
					4.0, 9.0, 2.0 }, 3, 3);

	matrix_f64 rhs({ 2.0, 9.0, 4.0,
					7.0, 5.0, 3.0 ,
					6.0, 1.0, 8.0 }, 3, 3);
	// rhs.push_back(9.0).push_back(8.0).push_back(7.0).push_back(6.0);
	std::cout << "Test matrices\n";
	std::cout << "lhs       :\n" << lhs << "\n";
	std::cout << "rhs       :\n" << rhs << "\n";
	std::cout << "vec       :" << vec << "\n";
	std::cout << "rhs * vec :\n" << rhs * vec << "\n";
	std::cout << "vec * rhs :\n" << vec * rhs << "\n";

	std::cout << "==========================================\n";
	std::cout << "rhs[:,0]  :\n" << rhs.get_col(0) << "\n";
	std::cout << "rhs[:,1]  :\n" << rhs.get_col(1) << "\n";
	std::cout << "rhs[:,2]  :\n" << rhs.get_col(2) << "\n";
	std::cout << "==========================================\n";
	std::cout << "rhs[0,:]  :\n" << rhs.get_row(0) << "\n";
	std::cout << "rhs[1,:]  :\n" << rhs.get_row(1) << "\n";
	std::cout << "rhs[2,:]  :\n" << rhs.get_row(2) << "\n";
	std::cout << "==========================================\n";
	std::cout << "rhs[:,0]  :\n" << rhs.get_col(0) << "\n";
	std::cout << "rhs[:,1]  :\n" << rhs.get_col(1) << "\n";
	std::cout << "rhs[:,2]  :\n" << rhs.get_col(2) << "\n";
	for (auto v : rhs.cols())
	{
		std::cout << v << " ";
	}
	std::cout << "\n==========================================\n";
	std::cout << "rhs[0,:]  :\n" << rhs.get_row(0) << "\n";
	std::cout << "rhs[1,:]  :\n" << rhs.get_row(1) << "\n";
	std::cout << "rhs[2,:]  :\n" << rhs.get_row(2) << "\n";
	for (auto const& v : rhs.rows())
	{
		std::cout << v << " ";
	}
	// std::cout << "rhs - copy:\n" << matrix_f64(rhs) << "\n";
	std::cout << "\nOperations +,-,*,/ for matrix and matrix\n";
	std::cout << "lhs + rhs :\n" << lhs + rhs << "\n";
	std::cout << "lhs - rhs :\n" << lhs - rhs << "\n";
	std::cout << "lhs * rhs :\n" << lhs * rhs << "\n";
	std::cout << "lhs / rhs :\n" << lhs / lhs << "\n";

	matrix_f64 low(1, 1), up(1, 1);
	matrix_f64::lu(lhs, low, up);
	std::cout << "LU decomposition\n";
	std::cout << "low(lhs)  :\n" << low << "\n";
	std::cout << "up (lhs)  :\n" << up << "\n";

	std::cout << "Operations +,-,*,/ for number and matrix\n";
	// 						
	std::cout << "2 + rhs   :\n" << 2.0 + rhs << "\n";
	std::cout << "2 - rhs   :\n" << 2.0 - rhs << "\n";
	std::cout << "2 * rhs   :\n" << 2.0 * rhs << "\n";
	std::cout << "2 / rhs   :\n" << 2.0 / rhs << "\n";

	std::cout << "Operations +,-,*,/ for matrix and number\n";
	// 						
	std::cout << "rhs + 2   :\n" << rhs + 2.0 << "\n";
	std::cout << "rhs - 2   :\n" << rhs - 2.0 << "\n";
	std::cout << "rhs * 2   :\n" << rhs * 2.0 << "\n";
	std::cout << "rhs / 2   :\n" << rhs / 2.0 << "\n";
	// 						
	std::cout << "Operations +=,-=,*=,/= for matrix and matrix\n";
	std::cout << "lhs += rhs:\n" << (lhs += rhs) << "\n";
	std::cout << "lhs -= rhs:\n" << (lhs -= rhs) << "\n";
	std::cout << "lhs *= rhs:\n" << (lhs *= rhs) << "\n";
	std::cout << "lhs /= rhs:\n" << (lhs /= lhs) << "\n";
	// 						
	std::cout << "Operations +=,-=,*=,/= for matrix and number\n";
	std::cout << "lhs += 2.0:\n" << (lhs += 2.0) << "\n";
	std::cout << "lhs -= 2.0:\n" << (lhs -= 2.0) << "\n";
	std::cout << "lhs *= 2.0:\n" << (lhs *= 2.0) << "\n";
	std::cout << "lhs /= 2.0:\n" << (lhs /= 2.0) << "\n";

	std::cout << "lhs == rhs     : " << (lhs == rhs) << "\n";
	std::cout << "lhs != rhs     : " << (lhs != rhs) << "\n";
	std::cout << "lhs > rhs      : " << (lhs > rhs) << "\n";
	std::cout << "lhs < rhs      : " << (lhs < rhs) << "\n";
	std::cout << "lhs >= rhs     : " << (lhs >= rhs) << "\n";
	std::cout << "lhs <= rhs     : " << (lhs <= rhs) << "\n";
	std::cout << "lhs >= 100     : " << (lhs >= 100.0) << "\n";
	std::cout << "lhs <= 100     : " << (lhs <= 100.0) << "\n";
	std::cout << "lhs            :\n" << lhs << "\n";
	//lhs[slice(3, 6)] = vector_f64({ 13.0, 13.0, 13.0 });
	std::cout << "lhs            :\n" << lhs << "\n";
	matrix_f64 big_mat =
		matrix_f64({ 0.8147, 0.4218, 0.2769, 0.7094, 0.8909, 0.3517, 0.5688, 0.2290, 0.2599, 0.4018, 0.7803, 0.6491, 0.4868, 0.6225, 0.9049, 0.0855,
					   0.9058, 0.9157, 0.0462, 0.7547, 0.9593, 0.8308, 0.4694, 0.9133, 0.8001, 0.0760, 0.3897, 0.7317, 0.4359, 0.5870, 0.9797, 0.2625,
					   0.1270, 0.7922, 0.0971, 0.2760, 0.5472, 0.5853, 0.0119, 0.1524, 0.4314, 0.2399, 0.2417, 0.6477, 0.4468, 0.2077, 0.4389, 0.8010,
					   0.9134, 0.9595, 0.8235, 0.6797, 0.1386, 0.5497, 0.3371, 0.8258, 0.9106, 0.1233, 0.4039, 0.4509, 0.3063, 0.3012, 0.1111, 0.0292,
					   0.6324, 0.6557, 0.6948, 0.6551, 0.1493, 0.9172, 0.1622, 0.5383, 0.1818, 0.1839, 0.0965, 0.5470, 0.5085, 0.4709, 0.2581, 0.9289,
					   0.0975, 0.0357, 0.3171, 0.1626, 0.2575, 0.2858, 0.7943, 0.9961, 0.2638, 0.2400, 0.1320, 0.2963, 0.5108, 0.2305, 0.4087, 0.7303,
					   0.2785, 0.8491, 0.9502, 0.1190, 0.8407, 0.7572, 0.3112, 0.0782, 0.1455, 0.4173, 0.9421, 0.7447, 0.8176, 0.8443, 0.5949, 0.4886,
					   0.5469, 0.9340, 0.0344, 0.4984, 0.2543, 0.7537, 0.5285, 0.4427, 0.1361, 0.0497, 0.9561, 0.1890, 0.7948, 0.1948, 0.2622, 0.5785,
					   0.9575, 0.6787, 0.4387, 0.9597, 0.8143, 0.3804, 0.1656, 0.1067, 0.8693, 0.9027, 0.5752, 0.6868, 0.6443, 0.2259, 0.6028, 0.2373,
					   0.9649, 0.7577, 0.3816, 0.3404, 0.2435, 0.5678, 0.6020, 0.9619, 0.5797, 0.9448, 0.0598, 0.1835, 0.3786, 0.1707, 0.7112, 0.4588,
					   0.1576, 0.7431, 0.7655, 0.5853, 0.9293, 0.0759, 0.2630, 0.0046, 0.5499, 0.4909, 0.2348, 0.3685, 0.8116, 0.2277, 0.2217, 0.9631,
					   0.9706, 0.3922, 0.7952, 0.2238, 0.3500, 0.0540, 0.6541, 0.7749, 0.1450, 0.4893, 0.3532, 0.6256, 0.5328, 0.4357, 0.1174, 0.5468,
					   0.9572, 0.6555, 0.1869, 0.7513, 0.1966, 0.5308, 0.6892, 0.8173, 0.8530, 0.3377, 0.8212, 0.7802, 0.3507, 0.3111, 0.2967, 0.5211,
					   0.4854, 0.1712, 0.4898, 0.2551, 0.2511, 0.7792, 0.7482, 0.8687, 0.6221, 0.9001, 0.0154, 0.0811, 0.9390, 0.9234, 0.3188, 0.2316,
					   0.8003, 0.7060, 0.4456, 0.5060, 0.6160, 0.9340, 0.4505, 0.0844, 0.3510, 0.3692, 0.0430, 0.9294, 0.8759, 0.4302, 0.4242, 0.4889,
					   0.1419, 0.0318, 0.6463, 0.6991, 0.4733, 0.1299, 0.0838, 0.3998, 0.5132, 0.1112, 0.1690, 0.7757, 0.5502, 0.1848, 0.5079, 0.6241 }, 16, 16);
	std::cout << "big_mat             :\n" << big_mat << "\n";
	std::cout << "big_mat^-1          :\n" << matrix_f64::invert(big_mat) << "\n";
	std::cout << "big_mat * big_mat^-1:\n" << big_mat / big_mat << "\n";

	std::cout << "rhs + col:\n" << rhs.add_row({ 1, 2, 3 }) << "\n";
	std::cout << "rhs + col:\n" << rhs.add_col({ 100, 100, 100, 100 }) << "\n";
	auto row = rhs.get_row(0);
	// rhs.get_row(0) -= row;
	// std::cout << "rhs - rhs[0:]:\n" << rhs<< "\n";
	rhs.get_row(1) -= row;
	std::cout << "rhs - rhs[1:]:\n" << rhs << "\n";
	rhs.get_row(2) -= row;
	std::cout << "rhs - rhs[2:]:\n" << rhs << "\n";
	rhs.get_row(3) -= row;
	std::cout << "rhs - rhs[3:]:\n" << rhs << "\n";
}
NUMERICS_NAMESPACE_END