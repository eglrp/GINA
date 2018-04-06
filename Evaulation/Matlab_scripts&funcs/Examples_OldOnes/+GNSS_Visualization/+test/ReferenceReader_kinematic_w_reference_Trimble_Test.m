function tests = ReferenceReader_kinematic_w_reference_Trimble_Test ()
    tests = functiontests(localfunctions());
end

% ts = matlab.unittest.TestSuite.fromPackage('GNSS_Visualization.test');
% tss = ts.selectIf('Name',['*ReferenceReader_kinematic_w_referenceTest*'])
% tss.run()

function test_creating_RefReader_kinematic_w_reference_Trimble_Test(testCase)

    ReferenceReader = GNSS_Visualization.ReferenceReader_kinematic_w_reference_Trimble();
    testCase.assertNotEmpty(ReferenceReader);
    testCase.assertTrue(isa(ReferenceReader,'GNSS_Visualization.ReferenceReader_kinematic_w_reference_Trimble'));
    
    testCase.assertThat(@() GNSS_Visualization.ReferenceReader_kinematic_w_reference_Trimble('a'),... 
        matlab.unittest.constraints.Throws('MATLAB:maxrhs'));
    
end

function test_reading_dummy_kinematic_w_reference_Trimble(testCase)
    ReferenceReader = GNSS_Visualization.ReferenceReader_kinematic_w_reference_Trimble();
    myReference = ReferenceReader.Read();
    testCase.assertNotEmpty(myReference);
    testCase.assumeTrue(isa(myReference, 'GNSS_Visualization.NullReference'));
    testCase.assumeTrue(sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(myReference)))>0);

    testCase.assertTrue(isempty(myReference.GetRefGPSSoW()));
    testCase.assertTrue(isempty(myReference.GetRefGPSWeek()));
    testCase.assertTrue(isempty(myReference.GetRefLatRad()));
    testCase.assertTrue(isempty(myReference.GetRefLonRad()));
    testCase.assertTrue(isempty(myReference.GetRefHigh()));
    testCase.assertTrue(isempty(myReference.GetRefLatDeg()));
    testCase.assertTrue(isempty(myReference.GetRefLonDeg()));
    testCase.assertTrue(isempty(myReference.GetRefX()));
    testCase.assertTrue(isempty(myReference.GetRefY()));
    testCase.assertTrue(isempty(myReference.GetRefZ()));
       
end

function test_reading_kinematic_w_reference_Trimble_w_false_arg(testCase)

    ReferenceReader = GNSS_Visualization.ReferenceReader_kinematic_w_reference_Trimble();
    myReference = ReferenceReader.Read('a');
    testCase.assertNotEmpty(myReference);
    testCase.assertTrue(isa(myReference, 'GNSS_Visualization.NullReference'));
    testCase.assertTrue(sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(myReference)))>0);
    
    % Referencreader with faulty Trimble trajectory =========================
    
    ReferenceReader = GNSS_Visualization.ReferenceReader_kinematic_w_reference_Trimble();
    myReference = ReferenceReader.Read(testCase.TestData.filenames{2}); % Faulty Trajectory
    testCase.assertNotEmpty(myReference);
    %
    testCase.assertTrue(isempty(myReference.GetRefGPSSoW()));
    testCase.assertTrue(isempty(myReference.GetRefGPSWeek()));
    testCase.assertTrue(isempty(myReference.GetRefLatRad()));
    testCase.assertTrue(isempty(myReference.GetRefLonRad()));
    testCase.assertTrue(isempty(myReference.GetRefHigh()));
    testCase.assertTrue(isempty(myReference.GetRefLatDeg()));
    testCase.assertTrue(isempty(myReference.GetRefLonDeg()));
    testCase.assertTrue(isempty(myReference.GetRefX()));
    testCase.assertTrue(isempty(myReference.GetRefY()));
    testCase.assertTrue(isempty(myReference.GetRefZ()));
    %
    testCase.assertTrue(isa(myReference, 'GNSS_Visualization.kinematicReference'));
    testCase.assertTrue(sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(myReference)))>0);

end

function test_reading_kinematic_w_reference_w_dummy_trajectory_Trimle(testCase)

    % Short Dummy Trajectory 

    ReferenceReader = GNSS_Visualization.ReferenceReader_kinematic_w_reference_Trimble();
    myReference = ReferenceReader.Read(testCase.TestData.filenames{3});
      
    [latrad, lonrad, height] = ecef2llh(4184398.817, 867946.522, 4719743.618);
    
    testCase.assertEqual(myReference.GetRefGPSSoW(),43200);
    testCase.assertEqual(myReference.GetRefGPSWeek(),1926);
    testCase.assertEqual(myReference.GetRefLatRad(),latrad);
    testCase.assertEqual(myReference.GetRefLonRad(),lonrad);
    testCase.assertEqual(myReference.GetRefHigh(),height);
    testCase.assertEqual(myReference.GetRefLatDeg(),latrad*180/pi);
    testCase.assertEqual(myReference.GetRefLonDeg(),lonrad*180/pi);
%     testCase.assumeTrue(isempty(myReference.GetRefX()));
%     testCase.assumeTrue(isempty(myReference.GetRefY()));
%     testCase.assumeTrue(isempty(myReference.GetRefZ()));
    
    testCase.assertNotEmpty(myReference);
    testCase.assumeTrue(isa(myReference, 'GNSS_Visualization.kinematicReference'));
    testCase.assumeTrue(sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(myReference)))>0);

end

function test_reading_kinematic_w_reference_w_long_trajectory_Trimble(testCase)

    % Short Dummy Trajectory 

    ReferenceReader = GNSS_Visualization.ReferenceReader_kinematic_w_reference_Trimble();
    myReference = ReferenceReader.Read(testCase.TestData.filenames{1});
      
    testCase.assertNotEmpty(myReference);
    testCase.assumeTrue(isa(myReference, 'GNSS_Visualization.kinematicReference'));
    testCase.assumeTrue(sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(myReference)))>0);

end

function setupOnce (testCase)
     testCase.TestData.filenames = {'+GNSS_Visualization\+test\+testdata\Trajectory_Highway.txt','+GNSS_Visualization\+test\+testdata\TrimbleTrajectoryDummyFaulty.txt',...
         '+GNSS_Visualization\+test\+testdata\TrimbleTrajectoryDummy.txt'};
     
     testCase.TestData.testAnswers = [1,2,3,4,5;11,22,33,44,55];
     testCase.TestData.GPSSoW     = [2,22]';
     testCase.TestData.GPSWeek    = [];
     testCase.TestData.LatRad     = [3,33]';
     testCase.TestData.LonRad     = [4,44]';
     testCase.TestData.High       = [5,55]';
     testCase.TestData.LatDeg     = [3,33]'*180/pi;
     testCase.TestData.LonDeg     = [4,44]'*180/pi;
end