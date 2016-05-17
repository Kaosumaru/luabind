#include <iostream>
#include <chrono>
#include "benchpress.hpp"

using namespace std;
benchpress::registration* benchpress::registration::d_this;



int main (int argc, char * argv[])
{
	std::chrono::high_resolution_clock::time_point bp_start = std::chrono::high_resolution_clock::now();
	benchpress::options bench_opts;
	benchpress::run_benchmarks(bench_opts);
	float duration = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now() - bp_start
		).count() / 1000.f;
	std::cout << argv[0] << " " << duration << "s" << std::endl;
	return 0;
}
