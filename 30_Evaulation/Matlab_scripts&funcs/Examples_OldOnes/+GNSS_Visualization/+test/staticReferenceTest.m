function tests = staticReferenceTest ()
    tests = functiontests(localfunctions());
end

% ts = matlab.unittest.TestSuite.fromPackage('GNSS_Visualization.test');
% tss = ts.selectIf('Name',['*staticReferenceTest*'])
% tss.run()


function test_creating_the_staticReference_object_with_various_false_arg (testCase)

    staticRef_with_zero_arg = GNSS_Visualization.staticReference();
    testCase.assertNotEmpty(staticRef_with_zero_arg)
    testCase.assumeTrue(isa(staticRef_with_zero_arg,'GNSS_Visualization.staticReference'))
    
    staticRef_with_false_arg_1 = GNSS_Visualization.staticReference('aa');
    testCase.assertNotEmpty(staticRef_with_false_arg_1);
    testCase.assumeTrue(isa(staticRef_with_false_arg_1,'GNSS_Visualization.staticReference'))
    
    staticRef_with_false_arg_2 = GNSS_Visualization.staticReference([0,1,3,4,5]);
    testCase.assertNotEmpty(staticRef_with_false_arg_2);
    testCase.assumeTrue(isa(staticRef_with_false_arg_2,'GNSS_Visualization.staticReference'))
    
    staticRef_with_false_arg_3 = GNSS_Visualization.staticReference({0,'aa',3,'b',5});
    testCase.assertNotEmpty(staticRef_with_false_arg_3);  
    testCase.assumeTrue(isa(staticRef_with_false_arg_3,'GNSS_Visualization.staticReference'))
    
    staticRef_with_false_arg_4 = GNSS_Visualization.staticReference(1,2);
    testCase.assertNotEmpty(staticRef_with_false_arg_4);
    testCase.assumeTrue(isa(staticRef_with_false_arg_4,'GNSS_Visualization.staticReference'))
    
end



function test_creating_the_staticReference_object_with_various_right_arg (testCase)

    staticRef = GNSS_Visualization.staticReference([0,1,2]);
    
    testCase.assumeTrue(staticRef.GetRefX() == 0)
    testCase.assumeTrue(staticRef.GetRefY() == 1)
    testCase.assumeTrue(staticRef.GetRefZ() == 2)
    
    staticRef = GNSS_Visualization.staticReference('0,1,2');
    
    testCase.assumeTrue(staticRef.GetRefX() == 0)
    testCase.assumeTrue(staticRef.GetRefY() == 1)
    testCase.assumeTrue(staticRef.GetRefZ() == 2)
    
    staticRef = GNSS_Visualization.staticReference('0,1,2','aa');
    
    testCase.assumeTrue(staticRef.GetRefX() == 0)
    testCase.assumeTrue(staticRef.GetRefY() == 1)
    testCase.assumeTrue(staticRef.GetRefZ() == 2)
    
    r = sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(staticRef)));
    testCase.assumeTrue(r == 1)
    
end

function setupOnce (testCase)
%     testCase.TestData.skipall = false;
%     testCase.TestData.MeasInfo = filehandler.FileHandler.LoadMeasurementInfo;
end
