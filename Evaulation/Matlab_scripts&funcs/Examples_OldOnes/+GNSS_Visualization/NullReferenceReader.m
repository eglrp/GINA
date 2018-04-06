classdef NullReferenceReader < GNSS_Visualization.IReferenceReader
    
    
    methods (Access = public)
        function this = NullReferenceReader(varargin)
            
        end
        function Ref = Read(this,varargin)
            Ref = GNSS_Visualization.NullReference();
        end
    end
end