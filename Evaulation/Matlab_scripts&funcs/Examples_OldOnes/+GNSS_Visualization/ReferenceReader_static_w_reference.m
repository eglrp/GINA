classdef ReferenceReader_static_w_reference < GNSS_Visualization.IReferenceReader
    
    
    methods 
        
        function this = ReferenceReader_static_w_reference(varargin)
            
        end
        
        function Ref = Read(this,varargin)
            if nargin >=2 
                Ref = read_static_ref(this,varargin{1});
            else
                Ref = read_static_ref(this,[]);
            end
           	if sum(cellfun(@(x) strcmp('GNSS_Visualization.IReference',x),superclasses(Ref)))==0
                Ref = GNSS_Visualization.NullReference();
            end
        end
        
    end
    
    methods (Access = private)
        function staticRef = read_static_ref(this,varargin)
            staticRef = GNSS_Visualization.staticReference(varargin{1});
        end
        
    end
end