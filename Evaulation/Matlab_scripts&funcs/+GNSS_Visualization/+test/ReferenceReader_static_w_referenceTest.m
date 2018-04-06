function tests = ReferenceReader_static_w_referenceTest ()
    tests = functiontests(localfunctions());
end

% ts = matlab.unittest.TestSuite.fromPackage('GNSS_Visualization.test');
% tss = ts.selectIf('Name',['*ReferenceReader_static_w_referenceTest*'])
% tss.run()


function test_creating_ReferenceReader_static_w_reference_with_zero_arg (testCase)

    ReferenceReader_static_w_reference_with_zero_arg = GNSS_Visualization.ReferenceReader_static_w_reference();
    testCase.assertNotEmpty(ReferenceReader_static_w_reference_with_zero_arg);
    testCase.assumeTrue(isa(ReferenceReader_static_w_reference_with_zero_arg,'GNSS_Visualization.IReferenceReader'));
    
    ReferenceReader_static_w_reference_with_zero_arg = GNSS_Visualization.ReferenceReader_static_w_reference('a');
    testCase.assertNotEmpty(ReferenceReader_static_w_reference_with_zero_arg);
    testCase.assumeTrue(isa(ReferenceReader_static_w_reference_with_zero_arg,'GNSS_Visualization.IReferenceReader'));
end


function test_reading_ReferenceReader_static_w_reference_with_zero_arg (testCase)

    ReferenceReader_static_w_reference = GNSS_Visualization.ReferenceReader_static_w_reference();
    myReference = ReferenceReader_static_w_reference.Read();
    testCase.assertNotEmpty(myReference);
    testCase.assumeTrue(isa(myReference, 'GNSS_Visualization.staticReference'));

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


function test_reading_ReferenceReader_static_w_reference_w_correct_arg (testCase)
    
    ReferenceReader_static_w_reference = GNSS_Visualization.ReferenceReader_static_w_reference();
    myReference = ReferenceReader_static_w_reference.Read([0,1,2]);
    testCase.assertNotEmpty(myReference);
    testCase.assumeTrue(isa(myReference, 'GNSS_Visualization.staticReference'));

    testCase.assumeTrue(myReference.GetRefX() == 0);
    testCase.assumeTrue(myReference.GetRefY() == 1);
    testCase.assumeTrue(myReference.GetRefZ() == 2);
    
    myReference = ReferenceReader_static_w_reference.Read('0,1,2');
    testCase.assumeTrue(myReference.GetRefX() == 0);
    testCase.assumeTrue(myReference.GetRefY() == 1);
    testCase.assumeTrue(myReference.GetRefZ() == 2);
    
    
end

function setupOnce (testCase)
     testCase.TestData.filenames = {'+tesdata\wrong_matfile_test_1','+tesdata\wrong_matfile_test_2','+tesdata\right_matfile_test'};
     testCase.TestData.testAnswers = [1,2,3,4,5;11,22,33,44,55];
     testCase.TestData.GPSSoW     = [2,22]';
     testCase.TestData.GPSWeek    = [];
     testCase.TestData.LatRad     = [3,33]';
     testCase.TestData.LonRad     = [4,44]';
     testCase.TestData.High       = [5,55]';
     testCase.TestData.LatDeg     = [3,33]'*180/pi;
     testCase.TestData.LonDeg     = [4,44]'*180/pi;
                
%     testCase.TestData.MeasInfo = filehandler.FileHandler.LoadMeasurementInfo;
end
