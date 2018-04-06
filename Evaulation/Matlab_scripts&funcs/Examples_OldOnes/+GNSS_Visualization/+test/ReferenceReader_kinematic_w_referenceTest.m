function tests = ReferenceReader_kinematic_w_referenceTest ()
    tests = functiontests(localfunctions());
end

% ts = matlab.unittest.TestSuite.fromPackage('GNSS_Visualization.test');
% tss = ts.selectIf('Name',['*ReferenceReader_kinematic_w_referenceTest*'])
% tss.run()

function test_creating_ReferenceReader_kinematic_w_referenceTest(testCase)

    ReferenceReader = GNSS_Visualization.ReferenceReader_kinematic_w_reference();
    testCase.assertNotEmpty(ReferenceReader);
    testCase.assumeTrue(isa(ReferenceReader,'GNSS_Visualization.ReferenceReader_kinematic_w_reference'));
    
    ReferenceReader = GNSS_Visualization.ReferenceReader_kinematic_w_reference('a');
    testCase.assertNotEmpty(ReferenceReader);
    testCase.assumeTrue(isa(ReferenceReader,'GNSS_Visualization.ReferenceReader_kinematic_w_reference'));
    
end

function test_reading_ReferenceReader_kinematic_w_reference(testCase)
    ReferenceReader = GNSS_Visualization.ReferenceReader_kinematic_w_reference();
    myReference = ReferenceReader.Read();
    testCase.assertNotEmpty(myReference);
    testCase.assumeTrue(isa(myReference, 'GNSS_Visualization.NullReference'));
    testCase.assumeTrue(sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(myReference)))>0);

    testCase.assumeTrue(isempty(myReference.GetRefGPSSoW()));
    testCase.assumeTrue(isempty(myReference.GetRefGPSWeek()));
    testCase.assumeTrue(isempty(myReference.GetRefLatRad()));
    testCase.assumeTrue(isempty(myReference.GetRefLonRad()));
    testCase.assumeTrue(isempty(myReference.GetRefHigh()));
    testCase.assumeTrue(isempty(myReference.GetRefLatDeg()));
    testCase.assumeTrue(isempty(myReference.GetRefLonDeg()));
    testCase.assumeTrue(isempty(myReference.GetRefX()));
    testCase.assumeTrue(isempty(myReference.GetRefY()));
    testCase.assumeTrue(isempty(myReference.GetRefZ()));
       
end

function test_reading_ReferenceReader_kinematic_w_reference_w_false_arg(testCase)
    ReferenceReader = GNSS_Visualization.ReferenceReader_kinematic_w_reference();
    myReference = ReferenceReader.Read('a');
    testCase.assertNotEmpty(myReference);
    testCase.assumeTrue(isa(myReference, 'GNSS_Visualization.NullReference'));
    testCase.assumeTrue(sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(myReference)))>0);
    
    %Referencreader with wrong matlab file 1 =========================
    ReferenceReader = GNSS_Visualization.ReferenceReader_kinematic_w_reference();
    myReference = ReferenceReader.Read(testCase.TestData.filenames{1});
    testCase.assertNotEmpty(myReference);
    %
    testCase.assumeTrue(isempty(myReference.GetRefGPSSoW()));
    testCase.assumeTrue(isempty(myReference.GetRefGPSWeek()));
    testCase.assumeTrue(isempty(myReference.GetRefLatRad()));
    testCase.assumeTrue(isempty(myReference.GetRefLonRad()));
    testCase.assumeTrue(isempty(myReference.GetRefHigh()));
    testCase.assumeTrue(isempty(myReference.GetRefLatDeg()));
    testCase.assumeTrue(isempty(myReference.GetRefLonDeg()));
    testCase.assumeTrue(isempty(myReference.GetRefX()));
    testCase.assumeTrue(isempty(myReference.GetRefY()));
    testCase.assumeTrue(isempty(myReference.GetRefZ()));
    %
    testCase.assumeTrue(isa(myReference, 'GNSS_Visualization.kinematicReference'));
    testCase.assumeTrue(sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(myReference)))>0);

    
    %Referencreader with wrong matlab file 2 =========================
    ReferenceReader = GNSS_Visualization.ReferenceReader_kinematic_w_reference();
    myReference = ReferenceReader.Read(testCase.TestData.filenames{2});
    testCase.assertNotEmpty(myReference);
    
    testCase.assumeTrue(isempty(myReference.GetRefGPSSoW()));
    testCase.assumeTrue(isempty(myReference.GetRefGPSWeek()));
    testCase.assumeTrue(isempty(myReference.GetRefLatRad()));
    testCase.assumeTrue(isempty(myReference.GetRefLonRad()));
    testCase.assumeTrue(isempty(myReference.GetRefHigh()));
    testCase.assumeTrue(isempty(myReference.GetRefLatDeg()));
    testCase.assumeTrue(isempty(myReference.GetRefLonDeg()));
    testCase.assumeTrue(isempty(myReference.GetRefX()));
    testCase.assumeTrue(isempty(myReference.GetRefY()));
    testCase.assumeTrue(isempty(myReference.GetRefZ()));
    
    testCase.assumeTrue(isa(myReference, 'GNSS_Visualization.kinematicReference'));
    testCase.assumeTrue(sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(myReference)))>0);
        
end

function test_reading_ReferenceReader_kinematic_w_reference_w_right_arg(testCase)

    ReferenceReader = GNSS_Visualization.ReferenceReader_kinematic_w_reference();
    myReference = ReferenceReader.Read(testCase.TestData.filenames{3});
      
    testCase.assumeTrue(isequal(myReference.GetRefGPSSoW(),[2;22]));
    testCase.assumeTrue(isempty(myReference.GetRefGPSWeek()));
    testCase.assumeTrue(isequal(myReference.GetRefLatRad(),[3,33]'));
    testCase.assumeTrue(isequal(myReference.GetRefLonRad(),[4,44]'));
    testCase.assumeTrue(isequal(myReference.GetRefHigh(),[5,55]'));
    testCase.assumeTrue(isequal(myReference.GetRefLatDeg(),[3,33]'*180/pi));
    testCase.assumeTrue(isequal(myReference.GetRefLonDeg(),[4,44]'*180/pi));
    testCase.assumeTrue(isempty(myReference.GetRefX()));
    testCase.assumeTrue(isempty(myReference.GetRefY()));
    testCase.assumeTrue(isempty(myReference.GetRefZ()));
    
    testCase.assertNotEmpty(myReference);
    testCase.assumeTrue(isa(myReference, 'GNSS_Visualization.kinematicReference'));
    testCase.assumeTrue(sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(myReference)))>0);

end

function setupOnce (testCase)
     testCase.TestData.filenames = {'+GNSS_Visualization\+test\+testdata\wrong_matfile_test_1.mat','+GNSS_Visualization\+test\+testdata\wrong_matfile_test_2.mat','+GNSS_Visualization\+test\+testdata\right_matfile_test.mat'};
     testCase.TestData.testAnswers = [1,2,3,4,5;11,22,33,44,55];
     testCase.TestData.GPSSoW     = [2,22]';
     testCase.TestData.GPSWeek    = [];
     testCase.TestData.LatRad     = [3,33]';
     testCase.TestData.LonRad     = [4,44]';
     testCase.TestData.High       = [5,55]';
     testCase.TestData.LatDeg     = [3,33]'*180/pi;
     testCase.TestData.LonDeg     = [4,44]'*180/pi;
end