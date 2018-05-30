
IMU Generator params

	dt = 0.1;
	startTime = 0;
	time = 0.0;
	endTime = 150.0;
	startWeek = 1956;
	endWeek = 1956;

	ab << 0.0, 0.0, 0;
	wb << 0, 0, 0.;
	llh <<	47.464405,
			19.154166,// / 360.0*2.0*EIGEN_PI,
			0;

	rollpitchyaw << 0.0 / 180.0*EIGEN_PI, 
					0.0 / 180.0*EIGEN_PI, 
					60.0/ 180.0*EIGEN_PI;
	Vb << 1., 0.0, 0.0;