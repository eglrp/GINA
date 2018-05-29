
IMU Generator params

	dt = 0.001;
	startTime = 0;
	time = 0.0;
	endTime = 150.0;
	startWeek = 1956;
	endWeek = 1956;
	
	ab << 0.0, 0.0, 0;
	wb << 0, 0, 0.05;
	llh <<	0,
			0,
			0;
	
	rollpitchyaw << 0.0 / 360.0*2.0*EIGEN_PI, 
					0.0 / 360.0*2.0*EIGEN_PI, 
					0.0/ 360.0*2.0*EIGEN_PI;
	Vb << 1., 0.0, 0.0;