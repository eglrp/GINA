classdef ReferenceFactory 
    
    methods (Static)
        function ReferenceReader = Create(mode)
        	try
            	ReferenceReader = eval(['GNSS_Visualization.ReferenceReader_',mode]);
        	catch ME
              	ReferenceReader = GNSS_Visualization.NullReferenceReader();  
            	warning(ME.message)
              	warning('We have given back a nullfactory.')
          	end
            
            if ~isa(ReferenceReader,'GNSS_Visualization.IReferenceReader')
                ReferenceReader = GNSS_Visualization.NullReferenceReader(); 
                warning('We have given back a nullfactory.')
            end
            
            
        end
    end
end